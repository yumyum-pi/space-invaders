#include "./slice_int.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
int main() {

  IntSlice s = slice_init(0, 2);
  if (!slice_push(&s, 10)) {
    printf("error while pushing to slice: %d\n", 1);
  }
  if (!slice_push(&s, 10)) {
    printf("error while pushing to slice: %d\n", 2);
  }
  if (!slice_push(&s, 10)) {
    printf("error while pushing to slice: %d\n", 3);
  }
  printf("array: length: %lu, capacity: %lu\n", s.length, s.capacity);
  return s.capacity;
}
