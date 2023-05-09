#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int* p = malloc(sizeof(int));
  *p = 23;
  printf("*p = %d\n", *p);
}
