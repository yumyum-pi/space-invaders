#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"
#include "gun.h"

typedef struct {
  int health;
  Vec2i position;
  Vec2i velocity;
  Gun gun;
} Player;

#endif  // !PLAYER_H
