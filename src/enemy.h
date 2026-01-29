#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>
#include "./utils/math.h"
#include "gun.h"

typedef enum {
  ENEMY_LASER,
  ENEMY_PLASMA,
  ENEMY_GATLING,
  ENEMY_BURST,
  ENEMY_COUNT
} EnemyType;

typedef struct {
  EnemyType type;
  Vec2i position;
  Vec2i ancher_position;
  bool is_active;
  float speed;
  Gun gun;
} Enemy;

void SetEnemy(Enemy* e, EnemyType type, Vec2i ancher_position);
#endif  // !ENEMY_H
