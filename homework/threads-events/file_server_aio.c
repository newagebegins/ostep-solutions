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

#define exit_with_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct io_request io_request;
struct io_request {
  struct aiocb cb;
  int client_fd;
};

#define MAX_REQUESTS 8
#define IO_BUFSIZE 1024*1024

io_request requests[MAX_REQUESTS] = {0};
fd_set readfds;

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
      ssize_t bytes_written = write(p->client_fd, (void*)p->cb.aio_buf, bytes_read);
      if (bytes_written < 0) {
        perror("write");
      } else {
        assert(bytes_written == bytes_read);
        printf("Bytes written: %zd\n", bytes_written);
      }
    }
    printf("Closing client: %d\n", p->client_fd);
    close(p->client_fd);
    FD_CLR(p->client_fd, &readfds);
    printf("Closing file descriptor: %d\n", p->cb.aio_fildes);
    close(p->cb.aio_fildes);
    free((void*)p->cb.aio_buf);
    memset(p, 0, sizeof(*p));
  }
}

int main(void) {
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
                p->cb.aio_fildes = fd;
                p->cb.aio_buf = malloc(IO_BUFSIZE);
                p->cb.aio_nbytes = IO_BUFSIZE;
                if (aio_read(&p->cb) != 0) {
                  exit_with_error("aio_read");
                }
                break;
              }
              assert(j < MAX_REQUESTS);
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
