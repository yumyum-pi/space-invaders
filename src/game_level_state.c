#include "game_level_state.h"
#include <assert.h>
#include <stdlib.h>
#include "bullet.h"
#include "enemy.h"
#include "gun.h"
#include "level.h"
#include "player.h"
#include "utils/math.h"

Vec2i create_player_start_pos(Vec2i terminal_size) {
  return (Vec2i){
      .x = terminal_size.x / 2,  // middle of the screen
      .y = terminal_size.y - 8,
  };
}

// TODO: take a struct that defines the no. of emenies
GameLevelState* NewGameLevel(Vec2i terminal_size, const char* title) {
  GameLevelState* level_state = malloc(sizeof(GameLevelState));
  if (level_state == NULL) {
    return NULL;
  }

  Player p = {
      .health = 1,
      .position = create_player_start_pos(terminal_size),
      .velocity = zero_vec(),
      .gun = GetGun(GUN_LASER),
  };

  Pool* bullet_pool = new_object_pool(sizeof(bullet), 30);
  assert(bullet_pool != NULL);
  Pool* enemy_pool = new_object_pool(sizeof(Enemy), 10);
  assert(enemy_pool != NULL);

  // Enemy e = new_enemy((Vec2i){terminal_size.x / 2, 4}, 10);
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
