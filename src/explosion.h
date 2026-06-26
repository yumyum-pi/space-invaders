#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdbool.h>
#include "utils/math/ivec2.h"

#define MAX_EXPLOSIONS  10
#define EXPLOSION_FRAMES 3

typedef struct {
  IVec2 position;
  int   frames_remaining;
  bool  is_active;
} Explosion;

#endif  // EXPLOSION_H
