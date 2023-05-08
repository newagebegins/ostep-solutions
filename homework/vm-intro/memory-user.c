#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[argc+1]) {
  if (argc != 2) {
    fprintf(stderr, "usage: memory-user <size-in-mb>\n");
    return 1;
  }
  int szmb = atoi(argv[1]);
  if (szmb <= 0) {
    fprintf(stderr, "size must be positive\n");
    return 1;
  }
  size_t szb = (size_t)szmb * 1024 * 1024;
  printf("szmb = %d, szb = %zu, SIZE_MAX = %zu\n", szmb, szb, SIZE_MAX);
  unsigned long* buf = malloc(szb);
  if (!buf) {
    fprintf(stderr, "malloc failed\n");
    return 1;
  }
  size_t len = szb / sizeof(unsigned long);
  while (1) {
    for (size_t i = 0; i < len; ++i) {
      buf[i] = 255;
    }
    sleep(1);
  }
}
