#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char* argv[argc+1]) {
  if (argc != 2) {
    printf("usage: file_client <file-path>\n");
    exit(EXIT_FAILURE);
  }
  char* file_path = argv[1];
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) handle_error("socket");
  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(9002),
    .sin_addr.s_addr = INADDR_ANY,
  };
  printf("Connecting...\n");
  if (connect(sock, (struct sockaddr*) &address, sizeof(address)) != 0)
    handle_error("connect");
  printf("Sending file path...\n");
  if (send(sock, file_path, strlen(file_path)+1, 0) < 0)
    handle_error("send");
  char buf[64];
  printf("Receiving...\n");
  for (;;) {
    ssize_t read_bytes = read(sock, buf, sizeof(buf));
    if (read_bytes < 0) {
      handle_error("read");
    } else if (read_bytes == 0) {
      printf("EOF\n");
      break;
    } else {
      ssize_t written_bytes = write(STDOUT_FILENO, buf, read_bytes);
      if (written_bytes < 0) {
        handle_error("write");
      } else {
        assert(written_bytes == read_bytes);
      }
    }
  }  
  close(sock);
  return EXIT_SUCCESS;
}
