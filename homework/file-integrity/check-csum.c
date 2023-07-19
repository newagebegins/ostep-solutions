#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE (4*1024)
#define CRCS_SIZE (4*1024)

int main(int argc, char* argv[argc+1]) {
  if (argc != 3) {
    printf("usage: check-csum <input-file> <checksum-file>");
    return EXIT_FAILURE;
  }

  int in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }

  int crcs_fd = open(argv[2], O_RDONLY);
  if (crcs_fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }

  unsigned char* crcs = malloc(CRCS_SIZE);
  size_t num_crcs = 0;
  size_t crc_i = 0;
  size_t block_i = 0;

  unsigned char* block = malloc(BLOCK_SIZE);
  ssize_t num_read = 0;

  while ((num_read = read(in_fd, block, BLOCK_SIZE)) > 0) {
    unsigned char crc = 0;
    for (ssize_t i = 0; i < num_read; ++i) {
      crc ^= block[i];
      for (size_t i = 0; i < 8; ++i) {
        crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
      }
    }
    if (num_crcs == 0 || crc_i >= CRCS_SIZE) {
      ssize_t bytes_read = read(crcs_fd, crcs, CRCS_SIZE);
      assert(bytes_read > 0);
      num_crcs = bytes_read;
      crc_i = 0;
    }
    if (crcs[crc_i] != crc) {
      printf("Block %zu is corrupted: CRC expected 0x%X, CRC actual 0x%X\n", block_i, crcs[crc_i], crc);
      return EXIT_FAILURE;
    }
    ++crc_i;
    ++block_i;
  }

  if (num_read < 0) {
    perror("read failed");
    return EXIT_FAILURE;
  }

  printf("OK\n");

  return EXIT_SUCCESS;
}
