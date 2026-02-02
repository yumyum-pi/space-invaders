#include "./player.h"
#include <assert.h>
#include <stdlib.h>
#include "sprite.h"
#include "utils/math/ivec2.h"

Player* PlayerNew(IVec2 position, const char* spriteStr, GunType gunType) {
  Player* p = malloc(sizeof(Player));
  if (p == NULL) {
    assert(false);
    return NULL;
  }

  p->sprite = SpriteNewFromStr(spriteStr);
  p->position = position;
  p->health = 1;
  p->rectSize = (IVec2){p->sprite->width, p->sprite->height};
  p->velocity = IVec2Zero();
  p->gun = GetGun(gunType);
  return p;
}

void PlayerFree(Player* p) {
  if (p == NULL) {
    return;
  }
  if (p->sprite == NULL) {
    free(p);
    return;
  }

  SpriteFree(p->sprite);
  free(p);
}
