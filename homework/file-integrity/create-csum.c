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
    printf("usage: create-csum <input-file> <checksum-file>");
    return EXIT_FAILURE;
  }

  int in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }

  int crcs_fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if (crcs_fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }

  unsigned char* crcs = malloc(CRCS_SIZE);
  size_t num_crcs = 0;

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
    crcs[num_crcs] = crc;
    ++num_crcs;
    if (num_crcs >= CRCS_SIZE) {
      ssize_t num_written = write(crcs_fd, crcs, num_crcs);
      assert(num_written == num_crcs);
      num_crcs = 0;
    }
  }

  if (num_read < 0) {
    perror("read failed");
    return EXIT_FAILURE;
  }

  if (num_crcs > 0) {
    ssize_t num_written = write(crcs_fd, crcs, num_crcs);
    assert(num_written == num_crcs);
  }

  return EXIT_SUCCESS;
}
