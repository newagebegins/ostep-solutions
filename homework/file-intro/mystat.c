#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[argc+1]) {
  if (argc != 2) {
    printf("usage: mystat <path>\n");
    return EXIT_FAILURE;
  }

  struct stat st;
  if (stat(argv[1], &st) != 0) {
    perror(0);
    return EXIT_FAILURE;
  }

  printf("File type: ");
  if (st.st_mode & S_IFDIR) {
    printf("Directory");
  } else if (st.st_mode & S_IFREG) {
    printf("Regular File");
  } else {
    printf("???");
  }
  printf("\n");

  printf("File size: %lld\n", st.st_size);
  printf("Blocks allocated: %lld\n", st.st_blocks);
  printf("Block size: %d\n", st.st_blksize);
  printf("Links: %hu\n", st.st_nlink);
  printf("Device: %d\n", st.st_dev);
  printf("Inode: %llu\n", st.st_ino);
  printf("Uid: %d\n", st.st_uid);
  printf("Gid: %d\n", st.st_gid);
  printf("Mode: %d%d%d\n", (st.st_mode >> 6) & 7, (st.st_mode >> 3) & 7, st.st_mode & 7);
  printf("Access: %s", ctime(&st.st_atimespec.tv_sec));
  printf("Modify: %s", ctime(&st.st_mtimespec.tv_sec));
  printf("Change: %s", ctime(&st.st_ctimespec.tv_sec));
  printf("Birth: %s", ctime(&st.st_birthtimespec.tv_sec));

  return EXIT_SUCCESS;
}
