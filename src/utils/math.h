#ifndef MATH_H
#define MATH_H

#include <stdbool.h>
typedef struct {
  int x;
  int y;
} Vec2i;

Vec2i zero_vec(void);

int clamp(int v, int min, int max);
Vec2i clamp_vec2i(Vec2i source, Vec2i min, Vec2i max);
int move_to(int source, int target, int dx);
Vec2i move_to_vec2i(Vec2i source, Vec2i target, int dx);

bool is_point_in_rect(Vec2i p, Vec2i rect_min, Vec2i rect_max);

#endif // !MATH_H
