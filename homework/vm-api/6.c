#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int* data = malloc(10 * sizeof(int));
  for (size_t i = 0; i < 10; ++i) {
    data[i] = i;
  }
  free(data);
  printf("data[7] = %d\n", data[7]);
}
