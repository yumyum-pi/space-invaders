#include "game_level_state.h"
#include <assert.h>
#include <stdlib.h>
#include "bullet.h"
#include "enemy.h"
#include "gun.h"
#include "level.h"
#include "player.h"

IVec2 create_player_start_pos(IVec2 terminal_size) {
  return (IVec2){
      .x = terminal_size.x / 2,  // middle of the screen
      .y = terminal_size.y - 8,
  };
}

#define BULLET_POOL_SIZE 80
#define ENEMY_POOL_SIZE 10

// TODO: take a struct that defines the no. of emenies
GameLevelState* GameLevelNew(IVec2 terminal_size, const char* title, Level* level) {
  GameLevelState* level_state = malloc(sizeof(GameLevelState));
  if (level_state == NULL) {
    return NULL;
  }
  Player* p = PlayerNew(create_player_start_pos(terminal_size), "/=|^|=\\",
                        GUN_LASER_PLAYER);

  Pool* bullet_pool = new_object_pool(sizeof(bullet), BULLET_POOL_SIZE);
  assert(bullet_pool != NULL);
  Pool* enemy_pool = new_object_pool(sizeof(Enemy), ENEMY_POOL_SIZE);
  assert(enemy_pool != NULL);

  assert(level != NULL);

  level_state->title = title;
  level_state->player = p;
  level_state->enemy_pool = enemy_pool;
  level_state->bullet_pool = bullet_pool;
  level_state->level = level;
  level_state->frame_count = 0;
  level_state->score = 0;
  level_state->formation = (Formation){.dir = 1, .drop = false};
  level_state->tutorial = (Tutorial){.active = false, .step = 0, .step_frames = 0};
  for (int i = 0; i < MAX_EXPLOSIONS; i++)
    level_state->explosions[i].is_active = false;

  int sy = terminal_size.y - 12;
  level_state->shield_count = 3;
  level_state->shields[0] = (Shield){.position = {terminal_size.x / 4,     sy}, .hp = SHIELD_MAX_HP, .is_active = true};
  level_state->shields[1] = (Shield){.position = {terminal_size.x / 2,     sy}, .hp = SHIELD_MAX_HP, .is_active = true};
  level_state->shields[2] = (Shield){.position = {terminal_size.x * 3 / 4, sy}, .hp = SHIELD_MAX_HP, .is_active = true};

  return level_state;
}

void GamelevelFree(GameLevelState* level_state) {
  if (level_state == NULL) {
    return;
  }
  if (level_state->bullet_pool != NULL) {
    remove_object_pool(level_state->bullet_pool);
  }

  if (level_state->enemy_pool != NULL) {
    remove_object_pool(level_state->enemy_pool);
  }

  if (level_state->player != NULL) {
    PlayerFree(level_state->player);
  }

  free(level_state);
}
