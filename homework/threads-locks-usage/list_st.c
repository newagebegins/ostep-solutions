// Single-threaded

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

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
    numbers[i] = rand();
  }
}

Node HEAD = {0};
Node* const head = &HEAD;

void insert(int x) {
  Node* p = head;
  for (;;) {
    assert(p);
    assert(x >= p->val);
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
}

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
  printf("numbers: ");
  printf("%d", numbers[0]);
  for (int i = 1; i < N; i++) {
    printf(",%d", numbers[i]);
  }
  printf("\n");
}

void print_list() {
  printf("list: ");
  Node* p = head->next;
  printf("%d", p->val);
  p = p->next;
  while (p) {
    printf(",%d", p->val);
    p = p->next;
  }
  printf("\n");
}

int main(void) {
  generate_numbers();
  //print_numbers();

  struct timeval start;
  Gettimeofday(&start, 0);

  for (int i = 0; i < N; i++) {
    insert(numbers[i]);
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
