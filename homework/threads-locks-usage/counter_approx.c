#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#define NUMCPUS 4

#define Pthread_mutex_init(a,b) assert(pthread_mutex_init(a,b) == 0)
#define Pthread_mutex_lock(a) assert(pthread_mutex_lock(a) == 0)
#define Pthread_mutex_unlock(a) assert(pthread_mutex_unlock(a) == 0)

#define Pthread_create(a,b,c,d) assert(pthread_create(a,b,c,d) == 0)
#define Pthread_join(a,b) assert(pthread_join(a,b) == 0)

#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

typedef struct __counter_t {
  int global;
  pthread_mutex_t glock;
  int local[NUMCPUS];
  pthread_mutex_t llock[NUMCPUS];
  int threshold;
} counter_t;

typedef struct __worker_args_t {
  counter_t *counter;
  int num_iterations;
  int threadID;
} worker_args_t;

void init(counter_t *c, int threshold) {
  c->threshold = threshold;
  c->global = 0;
  Pthread_mutex_init(&c->glock, NULL);
  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->local[i] = 0;
    Pthread_mutex_init(&c->llock[i], NULL);
  }
}

void update(counter_t *c, int threadID, int amt) {
  int cpu = threadID % NUMCPUS;
  Pthread_mutex_lock(&c->llock[cpu]);
  c->local[cpu] += amt;
  if (c->local[cpu] >= c->threshold) {
    Pthread_mutex_lock(&c->glock);
    c->global += c->local[cpu];
    Pthread_mutex_unlock(&c->glock);
    c->local[cpu] = 0;
  }
  Pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
  Pthread_mutex_lock(&c->glock);
  int val = c->global;
  Pthread_mutex_unlock(&c->glock);
  return val;
}

void* worker(void* arg) {
  worker_args_t* args = arg;
  for (size_t i = 0; i < args->num_iterations; i++) {
    update(args->counter, args->threadID, 1);
  }
  return 0;
}

int main(int argc, char* argv[argc+1]) {
  if (argc != 4) {
    fputs("usage: approx_counter <num_iterations> <num_threads> <threshold>\n", stderr);
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

  int threshold = atoi(argv[3]);
  if (threshold < 1) {
    fputs("threshold must be >= 1\n", stderr);
    exit(1);
  }

  counter_t counter;
  init(&counter, threshold);
  printf("start counter = %d\n", get(&counter));

  pthread_t threads[num_threads];
  worker_args_t args[num_threads];

  struct timeval start;
  Gettimeofday(&start, 0);

  for (int i = 0; i < num_threads; i++) {
    args[i].counter = &counter;
    args[i].num_iterations = num_iterations;
    args[i].threadID = i;
    Pthread_create(&threads[i], 0, worker, (void*) &args[i]);
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
