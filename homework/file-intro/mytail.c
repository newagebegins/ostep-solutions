#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <stdbool.h>

#define USAGE "usage: mytail -n file\n"

#define BLOCK_SIZE 64

struct block {
  unsigned char buf[BLOCK_SIZE];
  size_t len;
  struct block* next;
};

struct block* head = 0;

int main(int argc, char* argv[argc+1]) {
  if (argc != 3) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  if (argv[1][0] != '-') {
    printf(USAGE);
    return EXIT_FAILURE;
  }

  int n = atoi(argv[1] + 1);
  if (n <= 0) {
    printf("n must be positive\n");
    return EXIT_FAILURE;
  }

  int fd = open(argv[2], O_RDONLY);
  if (fd < 0) {
    perror("open failed");
    return EXIT_FAILURE;
  }

  struct stat st;
  if (fstat(fd, &st) != 0) {
    perror("fstat failed");
    return EXIT_FAILURE;
  }

  size_t block = 1;
  size_t newlines = 0;
  off_t prev_offset = st.st_size;

  while (prev_offset) {
    off_t offset = prev_offset - block * BLOCK_SIZE;
    if (offset < 0) {
      offset = 0;
    }

    if (lseek(fd, offset, SEEK_SET) != offset) {
      perror("lseek failed");
      return EXIT_FAILURE;
    }

    struct block* block = malloc(sizeof *block);
    block->len = prev_offset - offset;
    if (read(fd, block->buf, block->len) != block->len) {
      perror("read failed");
      return EXIT_FAILURE;
    }

    size_t i = block->len - 1;
    while (true) {
      if (block->buf[i] == '\n') {
        ++newlines;
        if (newlines >= n) {
          size_t to_write = block->len - i - 1;
          if (write(STDOUT_FILENO, &block->buf[i+1], to_write) != to_write) {
            perror("write failed");
            return EXIT_FAILURE;
          }
          for (struct block* p = head; p; p = p->next) {
            if (write(STDOUT_FILENO, p->buf, p->len) != p->len) {
              perror("write failed");
              return EXIT_FAILURE;
            }
          }
          return EXIT_SUCCESS;
        }
      }
      if (i == 0) {
        break;
      }
      --i;
    }

    block->next = head;
    head = block;

    prev_offset = offset;
  }

  for (struct block* p = head; p; p = p->next) {
    if (write(STDOUT_FILENO, p->buf, p->len) != p->len) {
      perror("write failed");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
