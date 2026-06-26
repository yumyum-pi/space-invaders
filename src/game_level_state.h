#ifndef GAME_LEVEL_STATE_H
#define GAME_LEVEL_STATE_H

#include <stdbool.h>
#include <time.h>
#include "../lib/object_pool/object_pool.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "input.h"
#include "level.h"
#include "player.h"
#include "shield.h"
#include "utils/math.h"

#define FPS 24
#define FRAME_MS (1000 / FPS)

typedef struct {
  int  dir;   // +1 = moving right, -1 = moving left
  bool drop;  // true this frame = drop the whole formation 1 step
} Formation;

typedef struct {
  bool active;
  int  step;
  int  step_frames;
} Tutorial;

typedef struct {
  GameInput input;
  Player* player;
  int frame_count;
  int score;
  Formation formation;
  Tutorial tutorial;
  Level* level;
  Pool* enemy_pool;
  Pool* bullet_pool;
  Shield shields[MAX_SHIELDS];
  int shield_count;
  Explosion explosions[MAX_EXPLOSIONS];
  const char* title;
} GameLevelState;

GameLevelState* GameLevelNew(IVec2 terminal_size, const char* title, Level* level);
void GameLevelFree(GameLevelState* level);

#endif  // !GAME_LEVEL_STATE_H
