// Multithreaded, but without locks (works incorrectly)

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define Pthread_create(a,b,c,d) assert(pthread_create(a,b,c,d) == 0)
#define Pthread_join(a,b) assert(pthread_join(a,b) == 0)
#define Gettimeofday(a,b) assert(gettimeofday(a,b) == 0)

typedef struct Node Node;
struct Node {
  int val;
  Node* next;
};

#define NUMCPUS 4
#define N NUMCPUS*10000
int numbers[N];

void generate_numbers() {
  srand(123);
  for (int i = 0; i < N; i++) {
    numbers[i] = rand() % 1024;
  }
}

Node HEAD = { .val = -1, .next = 0 };
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
  //printf("T%zu: start\n", tid);
  assert(N % NUMCPUS == 0);
  size_t istart = (N / NUMCPUS) * (tid + 0);
  size_t iend   = (N / NUMCPUS) * (tid + 1);
  for (size_t i = istart; i < iend; i++) {
    int x = numbers[i];
    //printf("T%zu: start insert numbers[%zu]=%d\n", tid, i, x);
    Node* p = head;
    for (;;) {
      assert(p);
      assert(x >= p->val);
      //printf("T%zu: at %d\n", tid, p->val);
      if (p->next) {
        if (x < p->next->val) {
          Node* n = malloc(sizeof(Node));
          n->val = x;
          n->next = p->next;
          p->next = n;
          break;
        } else {
          p = p->next;
        }
      } else {
        Node* n = malloc(sizeof(Node));
        n->val = x;
        n->next = 0;
        p->next = n;
        break;
      }
    }
    //printf("T%zu: end insert numbers[%zu]=%d\n", tid, i, x);
  }
  //printf("T%zu: end\n", tid);
  return 0;
}

int main(void) {
  generate_numbers();
  // print_numbers();
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
