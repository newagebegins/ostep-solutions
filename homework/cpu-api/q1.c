#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int x = 100;
  printf("initial x = %d\n", x);
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    printf("after fork x in child = %d\n", x);
    x = 17;
    printf("new x in child = %d\n", x);
  } else {
    printf("after fork x in parent = %d\n", x);
    x = 99;
    printf("new x in parent = %d\n", x);
  }
}

/*
  The child and parent have their own copies of the x variable.
  Changing the variable in the child doesn't affect the parent and
  vice versa.
 */
