#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void print_dir(char* dir, size_t len) {
  DIR* dp = opendir(dir);
  if (!dp) {
    perror("opendir failed");
    exit(EXIT_FAILURE);
  }
  struct dirent* d;
  while ((d = readdir(dp))) {
    if (d->d_namlen == 1 && d->d_name[0] == '.') {
      printf("%s\n", dir);
      continue;
    }
    if (d->d_namlen == 2 && d->d_name[0] == '.' && d->d_name[1] == '.') {
      continue;
    }
    if (d->d_type & DT_DIR) {
      size_t subdir_len = len + d->d_namlen + 1;
      char* subdir = malloc(subdir_len + 1);
      assert(subdir);
      int rc = snprintf(subdir, subdir_len + 1, "%s/%s", dir, d->d_name);
      assert(rc > 0);
      print_dir(subdir, subdir_len);
      free(subdir);
    } else {
      printf("%s/%s\n", dir, d->d_name);
    }
  }
  closedir(dp);
}

int main(int argc, char* argv[argc+1]) {
  if (argc > 2) {
    printf("usage: myfind [directory]\n");
    exit(EXIT_FAILURE);
  }
  char* dir = argc == 2 ? argv[1] : ".";
  print_dir(dir, strlen(dir));
  return EXIT_SUCCESS;
}
