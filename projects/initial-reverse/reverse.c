#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct Node Node;
struct Node {
  char* line;
  Node* prev;
};

FILE* open_file(char const* path, char const* mode, struct stat* st) {
  FILE* fp = fopen(path, mode);
  if (!fp) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", path);
    exit(1);
  }

  int fildes = fileno(fp);
  if (fstat(fildes, st) != 0) {
    fprintf(stderr, "fstat failed for '%s'\n", path);
    exit(1);
  }

  return fp;
}

int main(int argc, char* argv[argc+1]) {
  if (argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  struct stat instat;
  struct stat outstat;

  FILE* in  = argc > 1 ? open_file(argv[1], "r", &instat)  : stdin;
  FILE* out = argc > 2 ? open_file(argv[2], "w", &outstat) : stdout;

  if (instat.st_dev == outstat.st_dev &&
      instat.st_ino == outstat.st_ino) {
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
  }

  Node* node = 0;
  Node* prevnode = 0;

  for (;;) {
    char* line = 0;
    size_t linecap = 0;
    if (getline(&line, &linecap, in) <= 0) {
      break;
    }
    node = malloc(sizeof *node);
    if (!node) {
      fprintf(stderr, "malloc failed\n");
      exit(1);
    }
    node->line = line;
    node->prev = prevnode;
    prevnode = node;
  }

  while (node) {
    if (fputs(node->line, out) < 0) {
      fprintf(stderr, "write failed\n");
      exit(1);
    }
    node = node->prev;
  }
}
