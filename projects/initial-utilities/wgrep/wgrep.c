#include <stdio.h>
#include <string.h>

void grep(char* searchterm, FILE* fp, char** line) {
  size_t linecap = 0;
  ssize_t linelen = 0;
  while ((linelen = getline(line, &linecap, fp)) > 0) {
    if (strstr(*line, searchterm)) {
      fputs(*line, stdout);
    }
  }
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    return 1;
  }
  char* line = 0;
  char* searchterm = argv[1];
  if (argc == 2) {
    grep(searchterm, stdin, &line);
  } else {
    for (size_t i = 2; i < argc; ++i) {
      FILE* fp = fopen(argv[i], "r");
      if (!fp) {
        printf("wgrep: cannot open file\n");
        return 1;
      }
      grep(searchterm, fp, &line);
      fclose(fp);
    }
  }
  return 0;
}
