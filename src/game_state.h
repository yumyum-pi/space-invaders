#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "bullet.h"
#include "enemy.h"
#include "input.h"
#include "player.h"
#include "utils/math.h"
#include <stdbool.h>
#include <time.h>

#define FPS 24
#define FRAME_MS (1000 / FPS)

typedef struct {
  Input input;
  Player player;
  int frame_count;
  Enemy enemy;
  bullet bullet;
  Vec2i terminal_size;
  Vec2i bounds_min;
  Vec2i bounds_max;
  bool is_running;
  const char *title;
  struct timespec frame_start;
  long target_frame_ms;

} GameState;

GameState game_state_init(Vec2i terminal_size, const char *title);

#endif // !GAME_state_H
