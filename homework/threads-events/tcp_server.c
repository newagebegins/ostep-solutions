#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

char* get_current_time() {
  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);
  return asctime(timeinfo);
}

int main(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) handle_error("socket");
  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(9002),
    .sin_addr.s_addr = INADDR_ANY,
  };
  if (bind(sock, (struct sockaddr*) &address, sizeof(address)) != 0) handle_error("bind");
  if (listen(sock, 5) != 0) handle_error("listen");
  for (;;) {
    int client_sock = accept(sock, 0, 0);
    if (client_sock == -1) handle_error("accept");
    char* t = get_current_time();
    if (send(client_sock, t, strlen(t)+1, 0) == -1) handle_error("send");
  }
  close(sock);
  return EXIT_SUCCESS;
}
