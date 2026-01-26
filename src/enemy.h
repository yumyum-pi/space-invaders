#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>
#include "./utils/math.h"
#include "gun.h"

typedef struct {
  Vec2i position;
  Vec2i target_position;
  Vec2i velocity;
  int health;
  bool is_active;
  float speed;
  Gun gun;
} Enemy;

Enemy new_enemy(Vec2i position, int health);
#endif  // !ENEMY_H
