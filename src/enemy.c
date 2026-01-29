#include "./enemy.h"
#include "gun.h"
#include "utils/math.h"

typedef struct {
  float speed;
  GunType gunType;
} EnemyDef;

static const EnemyDef ENEMY_DEFS[ENEMY_COUNT] = {
    [ENEMY_LASER] =
        {
            .speed = 0.2f,
            .gunType = GUN_LASER,
        },
    [ENEMY_PLASMA] =
        {
            .speed = 0.25f,
            .gunType = GUN_LASER,
        },
    [ENEMY_GATLING] =
        {
            .speed = 0.3f,
            .gunType = GUN_LASER,
        },
    [ENEMY_BURST] =
        {
            .speed = 0.4f,
            .gunType = GUN_LASER,
        },
};

void SetEnemy(Enemy* e, EnemyType type, Vec2i ancher_position) {
  type = clamp(type, 0, ENEMY_COUNT);
  EnemyDef def = ENEMY_DEFS[type];
  e->type = type;
  e->position = ancher_position;
  e->ancher_position = ancher_position;
  e->is_active = true;
  e->speed = def.speed;
  SetGun(&(e->gun), def.gunType);
}
