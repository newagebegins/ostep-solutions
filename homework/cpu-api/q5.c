#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    printf("(%d) hello from child\n", getpid());
    pid_t wait_pid = wait(0);
    printf("(%d) wait_pid in child is %d\n", getpid(), wait_pid);
    if (wait_pid < 0) {
      perror(0);
    }
  } else {
    pid_t wait_pid = wait(0);
    printf("(%d) hello from parent, wait_pid is %d\n", getpid(), wait_pid);
  }
}
