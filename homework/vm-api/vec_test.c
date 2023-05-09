#include "vec.h"
#include <stdio.h>

int main(void) {
  struct vec* v = vec_new();
  printf("initial len = %zu\n", vec_length(v));
  for (int i = 0; i < 10; ++i) {
    vec_push(v, i);
  }
  printf("len after pushes = %zu\n", vec_length(v));
  for (int i = 0; i < 5; ++i) {
    printf("vec_pop() = %d\n", vec_pop(v));
  }
  printf("len after pops = %zu\n", vec_length(v));
  printf("v[3] = %d\n", vec_get(v, 3));
  vec_free(v);
  v = 0;
}
