#include "vec.h"
#include <assert.h>

struct vec {
  int* p;
  size_t len;
  size_t cap;
};

struct vec* vec_new() {
  struct vec* v = malloc(sizeof(*v));
  v->cap = 1;
  v->len = 0;
  v->p = malloc(v->cap * sizeof(*v->p));
  assert(v->p);
  return v;
}

void vec_push(struct vec* v, int x) {
  if (v->len == v->cap) {
    v->cap *= 2;
    v->p = realloc(v->p, v->cap * sizeof(*v->p));
    assert(v->p);
  }
  v->p[v->len] = x;
  ++v->len;
}

int vec_pop(struct vec* v) {
  assert(v->len);
  --v->len;
  return v->p[v->len];
}

int vec_get(struct vec* v, size_t i) {
  assert(i < v->len);
  return v->p[i];
}

size_t vec_length(struct vec* v) {
  return v->len;
}

void vec_free(struct vec* v) {
  free(v->p);
  free(v);
}
