#ifndef PLAYER_H
#define PLAYER_H

#include <stddef.h>
#include "./utils/math.h"
#include "gun.h"

typedef struct {
  char* buffer;
  size_t buffer_size;
} Render;

typedef struct {
  int health;
  IVec2 position;
  IVec2 velocity;
  IVec2 rectSize;
  Render render;
  Gun gun;
} Player;

Player* PlayerNew(IVec2 Position, const char* render, GunType gunType);
void PlayerFree(Player* p);

#endif  // !PLAYER_H
