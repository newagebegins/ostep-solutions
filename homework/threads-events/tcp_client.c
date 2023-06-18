#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) handle_error("socket");
  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(9002),
    .sin_addr.s_addr = INADDR_ANY,
  };
  printf("Connecting...\n");
  if (connect(sock, (struct sockaddr*) &address, sizeof(address)) != 0) handle_error("connect");
  char response[512];
  printf("Receiving...\n");
  if (recv(sock, response, sizeof(response), 0) == -1) handle_error("recv");
  printf("Server response: %s\n", response);
  close(sock);
  return EXIT_SUCCESS;
}
