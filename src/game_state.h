#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h"
#include "utils/math.h"
#include <stdbool.h>

//  24 fps
// const int GAME_FRAME_RATE = 41666;

typedef struct {
  Player player;
  Vec2i terminal_size;
  Vec2i bounds_min;
  Vec2i bounds_max;
  bool is_running;
  const char *title;
} GameState;

GameState game_state_init(Vec2i terminal_size, const char *title);

#endif // !GAME_state_H
