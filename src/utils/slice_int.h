#ifndef INIT_SLICE_H
#define INIT_SLICE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int *data;
  size_t length;
  size_t capacity;
} IntSlice;

typedef struct {
  size_t length;
  size_t capacity;
} IntSliceConfig;

IntSlice slice_init(size_t length, size_t capcity);
bool slice_push(IntSlice *s, int value);
void slice_free(IntSlice *s);

#endif
