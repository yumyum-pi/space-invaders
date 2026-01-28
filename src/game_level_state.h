#ifndef GAME_LEVEL_STATE_H
#define GAME_LEVEL_STATE_H

#include <stdbool.h>
#include <time.h>
#include "../lib/object_pool/object_pool.h"
#include "bullet.h"
#include "enemy.h"
#include "input.h"
#include "level.h"
#include "player.h"
#include "utils/math.h"

#define FPS 24
#define FRAME_MS (1000 / FPS)

typedef struct {
  GameInput input;
  Player player;
  int frame_count;
  Level* level;
  Pool* enemy_pool;
  Pool* bullet_pool;
  const char* title;
} GameLevelState;

GameLevelState* NewGameLevel(Vec2i terminal_size, const char* title);
void RemoveGameLevel(GameLevelState* level);

#endif  // !GAME_LEVEL_STATE_H
