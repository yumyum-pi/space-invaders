#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int *data;
  size_t length;
  size_t capacity;
} IntSlice;
#define MAX_ITEMS 128

static int buffer[MAX_ITEMS];

static IntSlice slice_init(void) {
  IntSlice s = {.data = buffer, .length = 0, .capacity = MAX_ITEMS};

  assert(s.data != NULL);
  assert(s.capacity > 0);

  return s;
}

static bool slice_push(IntSlice *s, int value) {
  assert(s != NULL);
  assert(s->length <= s->capacity);

  if (s->length == s->capacity) {
    return false; // no reallocation allowed
  }

  s->data[s->length] = value;
  s->length++;

  return true;
}
int main() {

  IntSlice s = slice_init();
  slice_push(&s, 10);
  printf("hello from c: %lu:\n", s.length);
  return s.capacity;
}
