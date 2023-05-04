#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("(%d) start\n", getpid());
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    printf("(%d) hello from child\n", getpid());
    if (close(STDOUT_FILENO) != 0) {
      perror("close failed");
      exit(1);
    }
    if (printf("(%d) after close\n", getpid()) < 0) {
      perror("printf failed");
      exit(1);
    }
  } else {
    printf("(%d) hello from parent\n", getpid());
    if (wait(0) < 0) {
      perror("wait failed");
      exit(1);
    }
    printf("(%d) parent exits\n", getpid());
  }
}
