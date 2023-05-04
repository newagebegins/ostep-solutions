#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[argc+1]) {
  if (argc != 2) {
    printf("usage: q4 <exec func>\n");
    exit(1);
  }
  int pid = fork();
  if (pid < 0) {
    perror(0);
    exit(1);
  } else if (pid == 0) {
    if (strcmp(argv[1], "execl") == 0) {
      execl("/bin/ls", "ls", "-la", 0);
    } else if (strcmp(argv[1], "execle") == 0) {
      char* envp[] = {"LANG=ru_RU", 0};
      execle("/bin/ls", "ls", "-la", 0, envp);
    } else if (strcmp(argv[1], "execlp") == 0) {
      execlp("ls", "ls", "-la", 0);
    } else if (strcmp(argv[1], "execv") == 0) {
      char* args[] = {"ls", "-la", 0};
      execv("/bin/ls", args);
    } else if (strcmp(argv[1], "execvp") == 0) {
      char* args[] = {"ls", "-la", 0};
      execvp("ls", args);
    } else {
      printf("unknown exec func\n");
      exit(1);
    }
    perror(0);
    exit(1);
  } else {
    int wc = wait(0);
    if (wc < 0) {
      perror(0);
      exit(1);
    }
  }
}
