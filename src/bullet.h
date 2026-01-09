#ifndef BULLET_H
#define BULLET_H

#include "utils/math.h"
#include <stdbool.h>

typedef struct {
  Vec2i position;
  int fired_by;
  int speed;
  bool is_active;
} bullet;

#endif // !BULLET_H
