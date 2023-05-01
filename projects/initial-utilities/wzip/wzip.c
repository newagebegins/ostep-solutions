#include <stdio.h>
#include <stdlib.h>

void write_run(int n, char c) {
  if (fwrite(&n, sizeof(int), 1, stdout) != 1) {
    perror(0);
    exit(1);
  }
  if (fputc(c, stdout) != c) {
    perror(0);
    exit(1);
  }
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    exit(1);
  }
  int c = 0;
  int prevc = 0;
  int n = 0;
  for (size_t i = 1; i < argc; ++i) {
    FILE* fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wzip: cannot open file\n");
      exit(1);
    }
    while ((c = fgetc(fp)) != EOF) {
      if (c == prevc || !prevc) {
        ++n;
      } else {
        write_run(n, prevc);
        n = 1;
      }
      prevc = c;
    }
    if (fclose(fp) != 0) {
      perror(0);
      exit(1);
    }
  }
  if (n > 0) {
    write_run(n, prevc);
  }
}
