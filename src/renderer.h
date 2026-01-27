#ifndef RENDERER_H
#define RENDERER_H
#include "./utils/math.h"
#include "game_level_state.h"

typedef struct {
  Vec2i terminal_size;
  int stride;
  int buffer_size;
  char* buffer;
} renderer;

renderer renderer_init();
void render(renderer* r, GameLevelState* ge);
// void renderer_shutdown(renderer *r);
#endif  // !renderer_H
