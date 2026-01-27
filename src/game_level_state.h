#ifndef GAME_LEVEL_STATE_H
#define GAME_LEVEL_STATE_H

#include <stdbool.h>
#include <time.h>
#include "../lib/object_pool/object_pool.h"
#include "bullet.h"
#include "enemy.h"
#include "input.h"
#include "player.h"
#include "utils/math.h"

#define FPS 24
#define FRAME_MS (1000 / FPS)

typedef struct {
  GameInput input;
  Player player;
  int frame_count;
  Enemy enemy;
  Pool* bullet_pool;
  const char* title;
} GameLevelState;

GameLevelState game_level_init(Vec2i terminal_size, const char* title);

#endif  // !GAME_LEVEL_STATE_H
