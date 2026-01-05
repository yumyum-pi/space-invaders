#include "./math.h"
#include <assert.h>
Vec2i zero_vec(void) {
  return (Vec2i){
      .x = 0,
      .y = 0,
  };
};

int move_to(int source, int target, int dx) {
  if (source == target) {
    return target;
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
