#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <aio.h>
#include <signal.h>

#define exit_with_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct cache_entry cache_entry;
struct cache_entry {
  char* path;
  char* content;
};

#define CACHE_SIZE 16
cache_entry cache[CACHE_SIZE] = {0};

cache_entry* cache_get(char const* path) {
  for (int i = 0; i < CACHE_SIZE; ++i) {
    if (cache[i].path && strcmp(cache[i].path, path) == 0) {
      return &cache[i];
    }
  }
  return 0;
}

void cache_set(char const* path, char const* content) {
  for (int i = 0; i < CACHE_SIZE; ++i) {
    cache_entry* p = &cache[i];
    if (!p->path) {
      p->path = strdup(path);
      assert(p->path);
      p->content = strdup(content);
      assert(p->content);
      return;
    }
  }
}

void cache_clear() {
  printf("Clearing the cache...\n");
  for (int i = 0; i < CACHE_SIZE; ++i) {
    cache_entry* p = &cache[i];
    if (p->path) {
      free(p->path);
      p->path = 0;
    }
    if (p->content) {
      free(p->content);
      p->content = 0;
    }
  }
}

void handle_signal(int x) {
  cache_clear();
}

typedef struct io_request io_request;
struct io_request {
  struct aiocb cb;
  char* file_path;
  int client_fd;
};

#define MAX_REQUESTS 8
#define IO_BUFSIZE 1024*1024

io_request requests[MAX_REQUESTS] = {0};
fd_set readfds;

void send_file(int client_fd, const char* content) {
  size_t len = strlen(content);
  ssize_t bytes_written = write(client_fd, (void*)content, len);
  if (bytes_written < 0) {
    perror("write");
  } else {
    assert(bytes_written == len);
    printf("Bytes written: %zd\n", bytes_written);
  }
}

void process_io() {
  for (int i = 0; i < MAX_REQUESTS; ++i) {
    io_request* p = &requests[i];
    if (!p->client_fd) {
      continue;
    }
    if (aio_error(&p->cb) == EINPROGRESS) {
      printf("IO in progress: %d\n", p->cb.aio_fildes);
      continue;
    }
    printf("IO complete: %d\n", p->cb.aio_fildes);
    ssize_t bytes_read = aio_return(&p->cb);
    if (bytes_read < 0) {
      perror("aio_read");
    } else if (bytes_read == 0) {
      printf("EOF\n");
    } else {
      printf("Bytes read: %zd\n", bytes_read);
      cache_set(p->file_path, (const char*) p->cb.aio_buf);
      send_file(p->client_fd, (const char*) p->cb.aio_buf);
    }
    printf("Closing client: %d\n", p->client_fd);
    close(p->client_fd);
    FD_CLR(p->client_fd, &readfds);
    printf("Closing file descriptor: %d\n", p->cb.aio_fildes);
    close(p->cb.aio_fildes);
    free((void*)p->cb.aio_buf);
    free(p->file_path);
    memset(p, 0, sizeof(*p));
  }
}

int main(void) {
  printf("Server pid: %d\n", getpid());
  if (signal(SIGUSR1, handle_signal) == SIG_ERR)
    exit_with_error("signal");

  int listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sd == -1)
    exit_with_error("socket");

  // Allow socket descriptor to be reusable
  int on = 1;
  if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    exit_with_error("setsockopt");

  // Set socket to be nonblocking
  if (ioctl(listen_sd, FIONBIO, &on) < 0)
    exit_with_error("ioctl");

  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(9002),
    .sin_addr.s_addr = INADDR_ANY,
  };
  if (bind(listen_sd, (struct sockaddr*) &address, sizeof(address)) != 0)
    exit_with_error("bind");
  if (listen(listen_sd, 32) != 0)
    exit_with_error("listen");

  FD_ZERO(&readfds);
  FD_SET(listen_sd, &readfds);

  for (;;) {
    process_io();

    fd_set readfds_copy = readfds;
    struct timeval timeout = {0};
    int select_res = select(FD_SETSIZE, &readfds_copy, 0, 0, &timeout);
    if (select_res < 0) {
      exit_with_error("select");
    }
    if (select_res == 0) {      
      continue;
    }
    printf("# of ready descriptors: %d\n", select_res);
    for (int i = 0; i < FD_SETSIZE; ++i) {
      if (FD_ISSET(i, &readfds_copy)) {
        if (i == listen_sd) {
          printf("Listening socket is readable\n");
          // Accept all incoming connections
          for (;;) {
            int new_sd = accept(listen_sd, 0, 0);
            if (new_sd < 0) {
              if (errno == EWOULDBLOCK) {
                break;
              } else {
                exit_with_error("accept");
              }
            }
            printf("Accepted %d\n", new_sd);
            FD_SET(new_sd, &readfds);
          }
        } else {
          bool close_conn = false;
          printf("Descriptor %d is readable\n", i);
          char file_path[128];
          printf("Receiving file path...\n");
          ssize_t nbytes = recv(i, file_path, sizeof(file_path), 0);
          if (nbytes < 0) {
            exit_with_error("recv");
          } else if (nbytes == 0) {
            printf("Descriptor %d disconnected\n", i);
            close_conn = true;
          } else {
            assert(nbytes <= sizeof(file_path));
            printf("File path: %s\n", file_path);
            cache_entry* ce = cache_get(file_path);
            if (ce) {
              printf("Cache hit\n");
              send_file(i, ce->content);
              close_conn = true;
            } else {
              printf("Cache miss\n");
              int fd = open(file_path, O_RDONLY);
              if (fd < 0) {
                perror("open");
                close_conn = true;
              } else {
                printf("Open file: %d\n", fd);
                int j;
                for (j = 0; j < MAX_REQUESTS; ++j) {
                  io_request* p = &requests[j];
                  if (p->client_fd) continue;
                  p->client_fd = i;
                  p->file_path = strdup(file_path);
                  assert(p->file_path);
                  p->cb.aio_fildes = fd;
                  p->cb.aio_buf = malloc(IO_BUFSIZE);
                  assert(p->cb.aio_buf);
                  p->cb.aio_nbytes = IO_BUFSIZE;
                  if (aio_read(&p->cb) != 0) {
                    exit_with_error("aio_read");
                  }
                  break;
                }
                assert(j < MAX_REQUESTS);
              }
            }
          }
          if (close_conn) {
            printf("Closing %d\n", i);
            close(i);
            FD_CLR(i, &readfds);
          }
        }
      }
    }
  }
  close(listen_sd);
  return EXIT_SUCCESS;
}
