#include "./string.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

my_string cstr_to_my_string(char *src) {

  size_t length = 0;
  // loop through the each string to find the len
  // finding the null terminaltor
  while (src[length] != '\0') {
    length += 1;
  }

  int size = sizeof(char) * length;
  char *dest = malloc(size);
  assert(dest != NULL);

  // copy mememory
  memcpy(dest, src, size);

  my_string s = {
      .data = dest,
      .length = length,
  };
  assert(s.length != 0);
  assert(s.data != NULL);
  return s;
}

void my_string_free(my_string *s) {
  assert(s->data != NULL);
  free(s->data);
  s->data = NULL;
  s->length = 0;
}
