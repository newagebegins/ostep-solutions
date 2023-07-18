#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUF_SIZE (4*1024)
#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

int main(int argc, char* argv[argc+1]) {
  assert(argc == 2);
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }
  unsigned char buf[BUF_SIZE];
  unsigned char fletcher_a = 0;
  unsigned char fletcher_b = 0;
  ssize_t num_read = 0;

  struct timeval start;
  Gettimeofday(&start, 0);

  while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
    for (ssize_t i = 0; i < num_read; ++i) {
      fletcher_a = (fletcher_a + buf[i]) % 255;
      fletcher_b = (fletcher_b + fletcher_a) % 255;
    }
  }

  struct timeval end;
  Gettimeofday(&end, 0);

  if (num_read < 0) {
    perror("read failed");
    return EXIT_FAILURE;
  }

  printf("%u %u\n", fletcher_a, fletcher_b);

  double secs = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
  printf("time = %g secs\n", secs);

  return EXIT_SUCCESS;
}
