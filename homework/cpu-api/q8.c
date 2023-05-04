#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Emulation of
// cat q8.c | wc

int main(void) {
  // write to fildes[1]
  // read from fildes[0]
  printf("(%d) parent start\n", getpid());
  int fildes[2];
  if (pipe(fildes) != 0) {
    perror("pipe failed");
    exit(1);
  }

  printf("(%d) fork cat\n", getpid());
  int cat_pid = fork();
  if (cat_pid < 0) {
    perror("cat fork failed");
    exit(1);
  }
  if (cat_pid == 0) {
    printf("(%d) cat child start\n", getpid());
    if (close(fildes[0]) != 0) {
      perror("close failed");
      exit(1);
    }
    if (dup2(fildes[1], STDOUT_FILENO) != STDOUT_FILENO) {
      perror("dup2 stdout failed");
      exit(1);
    }
    execlp("cat", "cat", "./q8.c", 0);
    perror("exec cat failed");
    exit(1);
  }

  printf("(%d) fork wc\n", getpid());
  int wc_pid = fork();
  if (wc_pid < 0) {
    perror("wc fork failed");
    exit(1);
  }
  if (wc_pid == 0) {
    printf("(%d) wc child start\n", getpid());
    if (close(fildes[1]) != 0) {
      perror("close failed");
      exit(1);
    }
    if (dup2(fildes[0], STDIN_FILENO) != STDIN_FILENO) {
      perror("wc dup2 failed");
      exit(1);
    }
    execlp("wc", "wc", 0);
    perror("exec wc failed");
    exit(1);
  }

  printf("(%d) parent wait\n", getpid());
  wait(0);
  printf("(%d) parent exit\n", getpid());
}
