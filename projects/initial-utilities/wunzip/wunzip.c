#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    exit(1);
  }
  for (size_t i = 1; i < argc; ++i) {
    FILE* fp = fopen(argv[i], "r");
    if (!fp) {
      printf("wunzip: cannot open file\n");
      exit(1);
    }
    int n;
    while (fread(&n, sizeof(int), 1, fp) == 1) {
      int c = fgetc(fp);
      if (c == EOF) {
        printf("failed to read a character\n");
        exit(1);
      }
      for (int i = 0; i < n; ++i) {
        if (fputc(c, stdout) != c) {
          printf("failed to output a character\n");
          exit(1);
        }
      }
    }
    if (fclose(fp) != 0) {
      perror(0);
      exit(1);
    }
  }
}
