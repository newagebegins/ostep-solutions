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
  ssize_t num_read = 0;

  struct timeval start;
  Gettimeofday(&start, 0);

  /*
    https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4#CRC-16

    Name  : CRC-16 CCITT
    Poly  : 0x1021    x^16 + x^12 + x^5 + 1
    Init  : 0xFFFF
    Revert: false
    XorOut: 0x0000
    Check : 0x29B1 ("123456789")
    MaxLen: 4095 bytes (32767 bits)
  */
  unsigned short crc = 0xFFFF;

  while ((num_read = read(fd, buf, BUF_SIZE)) > 0) {
    for (ssize_t i = 0; i < num_read; ++i) {
      crc ^= buf[i] << 8;
      for (size_t i = 0; i < 8; ++i) {
        crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
      }
    }
  }

  struct timeval end;
  Gettimeofday(&end, 0);

  if (num_read < 0) {
    perror("read failed");
    return EXIT_FAILURE;
  }

  printf("0x%X\n", crc);

  double secs = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
  printf("time = %g secs\n", secs);

  return EXIT_SUCCESS;
}
