#include "./math.h"
#include <assert.h>
Vec2i zero_vec(void) {
  return (Vec2i){
      .x = 0,
      .y = 0,
  };
};

int move_to(int source, int target, int dx) {
  // calcuate the distance
  int distance = (source < target) ? (target - source) : (source - target);

  if (distance <= dx) {
    return target;
  }

  if (source < target) {
    source += dx;
  } else {
    source -= dx;
  }
  return source;
}
/*
 * this function is same as normal move to but if the distance is smaller than
 * dx return source. this is to fix the enemy flickering when moving over to
 * sine
 */
int move_to_source(int source, int target, int dx) {
  // calcuate the distance
  int distance = (source < target) ? (target - source) : (source - target);

  if (distance <= dx) {
    return source;
  }

  if (source < target) {
    source += dx;
  } else {
    source -= dx;
  }
  return source;
}

Vec2i move_to_vec2i(Vec2i source, Vec2i target, int dx) {
  source.x = move_to(source.x, target.x, dx);
  source.y = move_to(source.y, target.y, dx);
  return source;
}

int clamp(int v, int min, int max) {
  assert(min <= max);
  if (v < min)
    return min;
  if (v > max)
    return max;
  return v;
}

Vec2i clamp_vec2i(Vec2i source, Vec2i min, Vec2i max) {
  source.x = clamp(source.x, min.x, max.x);
  source.y = clamp(source.y, min.y, max.y);
  return source;
}

bool is_point_in_rect(Vec2i p, Vec2i rect_min, Vec2i rect_max) {
  return (p.x >= rect_min.x && p.x <= rect_max.x) &&
         (p.y >= rect_min.y && p.y <= rect_max.y);
}

#define SINE_TABLE_SIZE 128
const int sine_table[SINE_TABLE_SIZE] = {
    0,   1,   2,   3,   4,   5,   6,   8,   9,   10,  11,  12,  13,  14,  15,
    16,  16,  17,  18,  19,  19,  20,  21,  21,  22,  22,  22,  23,  23,  23,
    23,  23,  24,  23,  23,  23,  23,  23,  22,  22,  22,  21,  21,  20,  19,
    19,  18,  17,  16,  16,  15,  14,  13,  12,  11,  10,  9,   8,   6,   5,
    4,   3,   2,   1,   0,   -1,  -2,  -3,  -4,  -5,  -6,  -8,  -9,  -10, -11,
    -12, -13, -14, -15, -16, -16, -17, -18, -19, -19, -20, -21, -21, -22, -22,
    -22, -23, -23, -23, -23, -23, -24, -23, -23, -23, -23, -23, -22, -22, -22,
    -21, -21, -20, -19, -19, -18, -17, -16, -16, -15, -14, -13, -12, -11, -10,
    -9,  -8,  -6,  -5,  -4,  -3,  -2,  -1,
};

int get_sine_table_value_with_offset(int source, int offset) {
  int index = (source + offset) % SINE_TABLE_SIZE;
  // assert(index <= 0);
  return sine_table[index];
}
int get_sine_value(int source) {
  return get_sine_table_value_with_offset(source, 0);
}

bool is_eq_vec2i(Vec2i a, Vec2i b) {
  if (a.x == b.x && a.y == b.y) {
    return true;
  }
  return false;
}
