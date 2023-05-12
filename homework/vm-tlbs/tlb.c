#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char* argv[argc+1]) {
  if (argc != 3) {
    printf("usage: tlb <num_pages> <num_trials>\n");
    return 1;
  }

  int num_pages = atoi(argv[1]);
  if (num_pages <= 0) {
    printf("num_pages must be positive\n");
    return 1;
  }

  int num_trials = atoi(argv[2]);
  if (num_trials <= 0) {
    printf("num_trials must be positive\n");
    return 1;
  }

  int page_size = getpagesize();
  int jump = page_size / sizeof(int);
  int max_i = num_pages * jump;
  int* a = malloc(num_pages * page_size);

  struct timeval start, end;

  if (gettimeofday(&start, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }

  for (int trial = 0; trial < num_trials; ++trial) {
    for (int i = 0; i < max_i; i += jump) {
      a[i] += 1;
    }
  }

  if (gettimeofday(&end, 0) != 0) {
    perror("gettimeofday error");
    exit(1);
  }

  // time in microseconds
  double time_total_usec = (end.tv_sec - start.tv_sec)*1E6 + (double)(end.tv_usec - start.tv_usec);
  double time_per_trial_usec = time_total_usec / (num_trials * num_pages);
  double time_per_trial_nsec = time_per_trial_usec * 1000.0;
  printf("%g\n", time_per_trial_nsec);
}
