#include <stdio.h>

enum { buf_size = 512 };

int main(int argc, char* argv[argc+1]) {
  for (size_t i = 1; i < argc; ++i) {
    FILE* fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wcat: cannot open file\n");
      return 1;
    }
    char buf[buf_size];
    while (fgets(buf, buf_size, fp)) {
      fputs(buf, stdout);
    }
    fclose(fp);
  }
  return 0;
}
