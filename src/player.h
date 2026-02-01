#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"
#include "gun.h"

typedef struct {
  int health;
  IVec2 position;
  IVec2 velocity;
  Gun gun;
} Player;

#endif  // !PLAYER_H
