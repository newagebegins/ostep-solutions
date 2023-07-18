#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE (4*1024)

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
  while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
    for (ssize_t i = 0; i < num_read; ++i) {
      fletcher_a = (fletcher_a + buf[i]) % 255;
      fletcher_b = (fletcher_b + fletcher_a) % 255;
    }
  }
  if (num_read < 0) {
    perror("read failed");
    return EXIT_FAILURE;
  }
  printf("%u %u\n", fletcher_a, fletcher_b);
  return EXIT_SUCCESS;
}
