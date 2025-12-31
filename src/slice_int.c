#include "./slice_int.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

IntSlice slice_init(size_t length, size_t capacity) {
  assert(length <= capacity);
  assert(capacity > 0);
  int *buffer = (int *)malloc(sizeof(int) * capacity);
  // check if th returned pointer value is not null
  // incase of the system run outs of mememory it return NULL
  assert(buffer != NULL);

  IntSlice s = {.data = buffer, .length = length, .capacity = capacity};
  return s;
}

bool slice_push(IntSlice *s, int value) {
  assert(s != NULL);
  assert(s->length <= s->capacity);

  if (s->length == s->capacity) {
    return false; // no reallocation allowed
  }

  s->data[s->length] = value;
  s->length++;

  return true;
}

void slice_free(IntSlice *s) {
  assert(s->data != NULL);
  free(s->data);
  s->data = NULL;
  s->length = 0;
  s->capacity = 0;
}
