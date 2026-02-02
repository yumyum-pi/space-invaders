#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>
#include "./utils/math.h"
#include "gun.h"
#include "sprite.h"

typedef enum {
  ENEMY_LASER,
  ENEMY_PLASMA,
  ENEMY_GATLING,
  ENEMY_BURST,
  ENEMY_COUNT
} EnemyType;

typedef struct {
  EnemyType type;
  IVec2 position;
  IVec2 rectSize;
  IVec2 ancher_position;
  bool is_active;
  float speed;
  Gun gun;
} Enemy;

void SetEnemy(Enemy* e, EnemyType type, IVec2 ancher_position);
void EnemyInit();
Sprite EnemyGetSprite(EnemyType type);
#endif  // !ENEMY_H
