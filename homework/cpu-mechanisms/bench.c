#define _GNU_SOURCE
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <sched.h>

#define SYSCALL_ITERATIONS 100000
#define CTX_ITERATIONS 1000

int main(void) {
  int fd = open("./bench.c", O_RDONLY);
  if (fd < 0) {
    perror("open error");
    exit(1);
  }

  struct timeval start, end;

  if (gettimeofday(&start, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }
  for (size_t i = 0; i < SYSCALL_ITERATIONS; ++i) {
    read(fd, 0, 0);
  }
  if (gettimeofday(&end, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }

  double elapsed_usecs = (end.tv_sec - start.tv_sec)*1E6 + (double)(end.tv_usec - start.tv_usec);
  double syscall_usecs = elapsed_usecs / SYSCALL_ITERATIONS;
  printf("syscall: %g usec\n", syscall_usecs);

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);

  int pipe1[2];
  if (pipe(pipe1) != 0) {
    perror("pipe error");
    exit(1);
  }
  int pipe2[2];
  if (pipe(pipe2) != 0) {
    perror("pipe error");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork error");
    exit(1);
  }

  unsigned char buf = 0;

  if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    for (size_t i = 0; i < CTX_ITERATIONS; ++i) {
      read(pipe1[0], &buf, 1);
      write(pipe2[1], &buf, 1);
    }
    exit(0);
  }

  if (gettimeofday(&start, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }
  for (size_t i = 0; i < CTX_ITERATIONS; ++i) {
    write(pipe1[1], &buf, 1);
    read(pipe2[0], &buf, 1);
  }
  if (gettimeofday(&end, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }

  double elapsed_usecs2 = (end.tv_sec - start.tv_sec)*1E6 + (double)(end.tv_usec - start.tv_usec);
  double ctx_usecs = (elapsed_usecs2 / CTX_ITERATIONS) - 3.0*syscall_usecs;
  printf("context switch: %g usec\n", ctx_usecs);
}
