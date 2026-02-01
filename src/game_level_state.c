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
GameLevelState* NewGameLevel(IVec2 terminal_size, const char* title) {
  GameLevelState* level_state = malloc(sizeof(GameLevelState));
  if (level_state == NULL) {
    return NULL;
  }

  Player p = {
      .health = 1,
      .position = create_player_start_pos(terminal_size),
      .velocity = IVec2Zero(),
      .gun = GetGun(GUN_LASER),
  };

  Pool* bullet_pool = new_object_pool(sizeof(bullet), BULLET_POOL_SIZE);
  assert(bullet_pool != NULL);
  Pool* enemy_pool = new_object_pool(sizeof(Enemy), ENEMY_POOL_SIZE);
  assert(enemy_pool != NULL);

  // Enemy e = new_enemy((IVec2){terminal_size.x / 2, 4}, 10);
  Level* level = Level1();
  assert(level != NULL);

  level_state->title = title;
  level_state->player = p;
  level_state->enemy_pool = enemy_pool;
  level_state->bullet_pool = bullet_pool;
  level_state->level = level;
  level_state->frame_count = 0;
  return level_state;
}

void RemoveGamelevel_state(GameLevelState* level_state) {
  if (level_state == NULL) {
    return;
  }
  if (level_state->bullet_pool != NULL) {
    remove_object_pool(level_state->bullet_pool);
  }

  free(level_state);
}
