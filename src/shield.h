#ifndef SHIELD_H
#define SHIELD_H

#include <stdbool.h>
#include "utils/math/ivec2.h"

#define MAX_SHIELDS 4
#define SHIELD_MAX_HP 3

typedef struct {
  IVec2 position;
  int hp;
  bool is_active;
} Shield;

#endif  // SHIELD_H
