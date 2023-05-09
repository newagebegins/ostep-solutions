#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

struct vec;

struct vec* vec_new();
void vec_push(struct vec* v, int x);
int vec_pop(struct vec* v);
int vec_get(struct vec* v, size_t i);
size_t vec_length(struct vec* v);
void vec_free(struct vec* v);

#endif
