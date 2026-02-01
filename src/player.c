#include "./player.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Player* PlayerNew(IVec2 position, const char* renderStr, GunType gunType) {
  Player* p = malloc(sizeof(Player));
  if (p == NULL) {
    assert(false);
    return NULL;
  }

  // loop through the string
  size_t len = strlen(renderStr);

  p->render.buffer = malloc(len + 1);
  if (!p->render.buffer) {
    assert(false);
    free(p);
    return NULL;
  }
  memcpy(p->render.buffer, renderStr, len);
  p->render.buffer[len] = '\0';  // Ensure it's null-terminated
  p->render.buffer_size = len;

  p->position = position;
  p->health = 1;
  p->rectSize = (IVec2){len, 1};
  p->velocity = IVec2Zero();
  p->gun = GetGun(gunType);
  return p;
}

void PlayerFree(Player* p) {
  if (p == NULL) {
    return;
  }
  if (p->render.buffer == NULL) {
    free(p);
    return;
  }

  free(p->render.buffer);
  free(p);
}
