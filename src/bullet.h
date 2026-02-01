#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include "utils/math.h"

typedef struct {
  IVec2 position;
  int fired_by;
  int speed;
  bool is_active;
} bullet;

#endif  // !BULLET_H
