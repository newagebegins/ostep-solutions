#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void) {
  int fd = open("./q2.out", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  if (fd < 0) {
    perror(0);
    exit(1);
  }
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    char buf[] = "child\n";
    for (size_t i = 0; i < 1000; ++i) {
      ssize_t written = write(fd, buf, sizeof(buf)-1);
      if (written != sizeof(buf)-1) {
        perror(0);
        exit(1);
      }
    }
  } else {
    char buf[] = "parent\n";
    for (size_t i = 0; i < 1000; ++i) {
      ssize_t written = write(fd, buf, sizeof(buf)-1);
      if (written != sizeof(buf)-1) {
        perror(0);
        exit(1);
      }
    }
  }
}

/*
  Both the child and parent can access the file descriptor returned by
  open().
  Child and parent share the current file position.
  The output is interleaved.
 */
