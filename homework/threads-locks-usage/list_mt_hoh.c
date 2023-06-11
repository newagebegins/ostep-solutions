// Hand-over-hand locking

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define Pthread_create(a,b,c,d) assert(pthread_create(a,b,c,d) == 0)
#define Pthread_join(a,b) assert(pthread_join(a,b) == 0)

#define Pthread_mutex_init(a,b) assert(pthread_mutex_init(a,b) == 0)
#define Pthread_mutex_lock(a) assert(pthread_mutex_lock(a) == 0)
#define Pthread_mutex_unlock(a) assert(pthread_mutex_unlock(a) == 0)

#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

typedef struct Node Node;
struct Node {
  pthread_mutex_t lock;
  int val;
  Node* next;
};

#define NUMCPUS 4
#define N NUMCPUS*10000
int numbers[N];

void generate_numbers() {
  srand(123);
  for (int i = 0; i < N; i++) {
    numbers[i] = rand();
  }
}

Node HEAD = { .val = -1, .next = 0, .lock = PTHREAD_MUTEX_INITIALIZER };
Node* const head = &HEAD;

static int cmpint(const void *p1, const void *p2) {
  int x1 = *((int*)p1);
  int x2 = *((int*)p2);
  if (x1 < x2) return -1;
  if (x1 > x2) return 1;
  return 0;
}

void check_list() {
  size_t i;
  Node *p;
  for (i = 0, p = head->next; i < N && p; ++i, p = p->next) {
    assert(numbers[i] == p->val);
  }
}

void print_numbers() {
  printf("num: ");
  printf("%d", numbers[0]);
  for (int i = 1; i < N; i++) {
    printf(",%d", numbers[i]);
  }
  printf("\n");
}

void print_list() {
  printf("lst: ");
  Node* p = head->next;
  printf("%d", p->val);
  p = p->next;
  while (p) {
    printf(",%d", p->val);
    p = p->next;
  }
  printf("\n");
}

void* worker(void* arg) {
  size_t tid = (size_t) arg;
  assert(N % NUMCPUS == 0);
  size_t istart = (N / NUMCPUS) * (tid + 0);
  size_t iend   = (N / NUMCPUS) * (tid + 1);
  for (size_t i = istart; i < iend; i++) {
    int x = numbers[i];

    Node* n = malloc(sizeof(Node));
    Pthread_mutex_init(&n->lock, 0);
    n->val = x;
    n->next = 0;

    Node* p = head;
    for (;;) {
      Pthread_mutex_lock(&p->lock);
      assert(p);
      assert(x >= p->val);
      if (p->next) {
        Pthread_mutex_lock(&p->next->lock);
        if (x < p->next->val) {
          Pthread_mutex_unlock(&p->next->lock);
          n->next = p->next;
          p->next = n;
          Pthread_mutex_unlock(&p->lock);
          break;
        } else {
          Pthread_mutex_unlock(&p->next->lock);
          pthread_mutex_t *lock = &p->lock;
          p = p->next;
          Pthread_mutex_unlock(lock);
        }
      } else {
        p->next = n;
        Pthread_mutex_unlock(&p->lock);
        break;
      }
    }
  }
  return 0;
}

int main(void) {
  generate_numbers();
  //print_numbers();
  pthread_t threads[NUMCPUS];

  struct timeval start;
  Gettimeofday(&start, 0);

  for (size_t i = 0; i < NUMCPUS; i++) {
    Pthread_create(&threads[i], 0, worker, (void*) i);
  }
  for (size_t i = 0; i < NUMCPUS; i++) {
    Pthread_join(threads[i], 0);
  }

  struct timeval end;
  Gettimeofday(&end, 0);

  double secs = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
  printf("time = %g secs\n", secs);

  qsort(numbers, N, sizeof(int), cmpint);
  //print_numbers();
  //print_list();
  check_list();
}
