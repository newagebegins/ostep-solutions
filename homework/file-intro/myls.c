#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// for getpwuid
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>

// for getgrgid
#include <grp.h>

#define USAGE "usage: myls [-l] [directory]\n"

int main(int argc, char* argv[argc+1]) {
  if (argc > 3) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  bool long_format = false;
  char const* directory = 0;
  for (int i = 1; i < argc; ++i) {
    char* p = argv[i];
    if (p[0] == '-' && p[1] == 'l' && p[2] == 0) {
      long_format = true;
    } else {
      directory = p;
    }
  }
  if (argc == 3 && !(long_format && directory)) {
    printf(USAGE);
    return EXIT_FAILURE;
  }
  if (!directory) {
    directory = ".";
  }
  DIR* dp = opendir(directory);
  if (!dp) {
    perror("opendir failed");
    return EXIT_FAILURE;
  }

  if (chdir(directory) != 0) {
    perror("chdir failed");
    return EXIT_FAILURE;
  }

  time_t cur_time = time(0);
  struct tm cur_tm;
  if (!localtime_r(&cur_time, &cur_tm)) {
    perror("localtime_r failed");
    return EXIT_FAILURE;
  }

  struct dirent* d;
  while ((d = readdir(dp))) {
    if (d->d_name[0] == '.') {
      continue;
    }
    if (long_format) {
      struct stat st;
      if (stat(d->d_name, &st) != 0) {
        perror("stat failed");
        return EXIT_FAILURE;
      }
      printf("%c%c%c%c%c%c%c%c%c%c\t",
             st.st_mode & S_IFDIR ? 'd' : '-',

             st.st_mode & S_IRUSR ? 'r' : '-',
             st.st_mode & S_IWUSR ? 'w' : '-',
             st.st_mode & S_IXUSR ? 'x' : '-',

             st.st_mode & S_IRGRP ? 'r' : '-',
             st.st_mode & S_IWGRP ? 'w' : '-',
             st.st_mode & S_IXGRP ? 'x' : '-',

             st.st_mode & S_IROTH ? 'r' : '-',
             st.st_mode & S_IWOTH ? 'w' : '-',
             st.st_mode & S_IXOTH ? 'x' : '-');

      printf("%hu\t", st.st_nlink);

      struct passwd* passwd = getpwuid(st.st_uid);
      if (!passwd) {
        perror("getpwuid failed");
        return EXIT_FAILURE;
      }
      printf("%s\t", passwd->pw_name);

      struct group* group = getgrgid(st.st_gid);
      if (!group) {
        perror("getgrgid failed");
        return EXIT_FAILURE;
      }
      printf("%s\t", group->gr_name);

      printf("%lld\t", st.st_size);

      struct tm* tm = localtime(&st.st_mtimespec.tv_sec);
      if (!tm) {
        perror("localtime failed");
        return EXIT_FAILURE;
      }

      char* time_format = tm->tm_year == cur_tm.tm_year ? "%b %d %H:%M" : "%b %d %Y";
      char mtime[128];
      if (strftime(mtime, sizeof mtime, time_format, tm) == 0) {
        printf("strftime failed\n");
        return EXIT_FAILURE;
      }
      printf("%s\t", mtime);

      printf("%s\n", d->d_name);
    } else {
      printf("%s ", d->d_name);
    }
  }
  printf("\n");
  closedir(dp);
  return EXIT_SUCCESS;
}
