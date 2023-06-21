#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) {
      printf("wzip: cannot open file\n");
      exit(1);
    }
    struct stat st;
    if (fstat(fd, &st) != 0) {
      perror("fstat");
      exit(1);
    }
    size_t size = st.st_size;
    unsigned char* start = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (start == MAP_FAILED) {
      perror("mmap");
      exit(1);
    }
    unsigned char* end = start + size;
    for (unsigned char* p = start; p < end; ++p) {
      c = *p;
      if (c == prevc || !prevc) {
        ++n;
      } else {
        write_run(n, prevc);
        n = 1;
      }
      prevc = c;
    }
    if (munmap(start, size) != 0) {
      perror("munmap");
      exit(1);
    }
    if (close(fd) != 0) {
      perror("close");
      exit(1);
    }
  }
  if (n > 0) {
    write_run(n, prevc);
  }
}
