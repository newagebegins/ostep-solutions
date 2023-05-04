#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define HELLO_FILE "./q3.hello"

int main(void) {
  remove(HELLO_FILE);
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    printf("hello\n");
    int fd = open(HELLO_FILE, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    if (fd < 0) {
      perror(0);
      exit(1);
    }
  } else {
    while (access(HELLO_FILE, F_OK) != 0) {
      // do nothing
    }
    printf("goodbye\n");
  }
}
