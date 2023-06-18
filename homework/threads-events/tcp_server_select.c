#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

char* get_current_time() {
  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);
  return asctime(timeinfo);
}

int main(void) {
  int listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sd == -1)
    handle_error("socket");

  // Allow socket descriptor to be reusable
  int on = 1;
  if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    handle_error("setsockopt");

  // Set socket to be nonblocking
  if (ioctl(listen_sd, FIONBIO, &on) < 0)
    handle_error("ioctl");

  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(9002),
    .sin_addr.s_addr = INADDR_ANY,
  };
  if (bind(listen_sd, (struct sockaddr*) &address, sizeof(address)) != 0)
    handle_error("bind");
  if (listen(listen_sd, 32) != 0)
    handle_error("listen");

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(listen_sd, &readfds);

  fd_set writefds;
  FD_ZERO(&writefds);

  for (;;) {
    fd_set readfds_copy = readfds;
    fd_set writefds_copy = writefds;
    printf("Waiting on select()...\n");
    int select_res = select(FD_SETSIZE, &readfds_copy, &writefds_copy, 0, 0);
    if (select_res < 0)
      handle_error("select");
    assert(select_res != 0);
    printf("# of ready descriptors: %d\n", select_res);
    for (int i = 0; i < FD_SETSIZE; ++i) {
      if (FD_ISSET(i, &readfds_copy)) {
        assert(i == listen_sd);
        printf("Listening socket is readable\n");
        // Accept all incoming connections
        for (;;) {
          int new_sd = accept(listen_sd, 0, 0);
          if (new_sd < 0) {
            if (errno == EWOULDBLOCK) {
              break;
            } else {
              handle_error("accept");
            }
          }
          printf("Accepted %d\n", new_sd);
          FD_SET(new_sd, &readfds);
          FD_SET(new_sd, &writefds);
        }
      } else if (FD_ISSET(i, &writefds_copy)) {
        assert(i != listen_sd);
        printf("Descriptor %d is writable\n", i);
        char* t = get_current_time();
        if (send(i, t, strlen(t)+1, 0) == -1)
          handle_error("send");
        close(i);
        FD_CLR(i, &readfds);
        FD_CLR(i, &writefds);
      }
    }
  }
  close(listen_sd);
  return EXIT_SUCCESS;
}
