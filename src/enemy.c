#include "./enemy.h"
#include "gun.h"
#include "sprite.h"
#include "utils/math/ivec2.h"

typedef struct {
  float speed;
  GunType gunType;
  Sprite sprite;
  IVec2 rectSize;
} EnemyDef;

EnemyDef ENEMY_DEFS[ENEMY_COUNT] = {
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

void setSprit(EnemyType type, const char* str) {
  Sprite* s = SpriteNewFromStr(str);
  IVec2 size = {s->width, s->height};
  ENEMY_DEFS[type].sprite = *s;
  ENEMY_DEFS[type].rectSize = size;
}

void EnemyInit(void) {
  setSprit(ENEMY_LASER, "<=|=>");
  setSprit(ENEMY_PLASMA, "\\\\-v-//");
  setSprit(ENEMY_GATLING, "\\=|=/");
  setSprit(ENEMY_BURST, "\\==*==/");
}

Sprite EnemyGetSprite(EnemyType type) {
  type = IClamp(type, 0, ENEMY_COUNT);
  EnemyDef def = ENEMY_DEFS[type];
  return def.sprite;
}

void SetEnemy(Enemy* e, EnemyType type, IVec2 ancher_position) {
  type = IClamp(type, 0, ENEMY_COUNT);
  EnemyDef def = ENEMY_DEFS[type];
  e->type = type;
  e->rectSize = def.rectSize;
  e->position = ancher_position;
  e->ancher_position = ancher_position;
  e->is_active = true;
  e->speed = def.speed;
  SetGun(&(e->gun), def.gunType);
}
