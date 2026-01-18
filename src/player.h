#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"
#include "gun.h"

typedef struct {
  Vec2i position;
  Vec2i velocity;
  Gun gun;
} Player;

Player new_player(void);

#endif  // !PLAYER_H
