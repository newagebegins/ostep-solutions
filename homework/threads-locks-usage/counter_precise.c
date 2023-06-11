#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>

#define Pthread_mutex_init(a,b) assert(pthread_mutex_init(a,b) == 0)
#define Pthread_mutex_lock(a) assert(pthread_mutex_lock(a) == 0)
#define Pthread_mutex_unlock(a) assert(pthread_mutex_unlock(a) == 0)

#define Pthread_create(a,b,c,d) assert(pthread_create(a,b,c,d) == 0)
#define Pthread_join(a,b) assert(pthread_join(a,b) == 0)

#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

typedef struct __counter_t {
  int value;
  pthread_mutex_t lock;
} counter_t;

typedef struct __worker_args_t {
  counter_t *counter;
  int num_iterations;
} worker_args_t;

void init(counter_t *c) {
  c->value = 0;
  Pthread_mutex_init(&c->lock, 0);
}

void increment(counter_t *c) {
  Pthread_mutex_lock(&c->lock);
  c->value++;
  Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
  Pthread_mutex_lock(&c->lock);
  size_t rc = c->value;
  Pthread_mutex_unlock(&c->lock);
  return rc;
}

void* worker(void* arg) {
  worker_args_t *args = arg;
  for (size_t i = 0; i < args->num_iterations; i++) {
    increment(args->counter);
  }
  return 0;
}

int main(int argc, char* argv[argc+1]) {
  if (argc != 3) {
    fputs("usage: precise_counter <num_iterations> <num_threads>\n", stderr);
    exit(1);
  }

  int num_iterations = atoi(argv[1]);
  if (num_iterations <= 0) {
    fputs("num_iterations must be positive\n", stderr);
    exit(1);
  }

  int num_threads = atoi(argv[2]);
  if (num_threads <= 0) {
    fputs("num_threads must be positive\n", stderr);
    exit(1);
  }

  counter_t counter;
  init(&counter);
  printf("start counter = %d\n", get(&counter));

  pthread_t threads[num_threads];
  worker_args_t args = {
    .counter = &counter,
    .num_iterations = num_iterations,
  };

  struct timeval start;
  Gettimeofday(&start, 0);

  for (int i = 0; i < num_threads; i++) {
    Pthread_create(&threads[i], 0, worker, (void*) &args);
  }

  for (int i = 0; i < num_threads; i++) {
    Pthread_join(threads[i], 0);
  }

  struct timeval end;
  Gettimeofday(&end, 0);

  printf("end counter = %d\n", get(&counter));

  double secs = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
  printf("time = %g secs\n", secs);

  return 0;
}
