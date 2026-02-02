#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"
#include "gun.h"
#include "sprite.h"

typedef struct {
  int health;
  IVec2 position;
  IVec2 velocity;
  IVec2 rectSize;
  Sprite* sprite;
  Gun gun;
} Player;

Player* PlayerNew(IVec2 Position, const char* render, GunType gunType);
void PlayerFree(Player* p);

#endif  // !PLAYER_H
