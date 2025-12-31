#include "./slice_int.h"
#include "./utils/string.h"
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

  my_string st = cstr_to_my_string("skd\0jhfksjahfdj");

  printf("string: length: %lu\n", st.length);
  printf("array: length: %lu, capacity: %lu\n", s.length, s.capacity);
  return s.capacity;
}
