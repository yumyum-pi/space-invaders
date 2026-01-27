#include "game_level_state.h"
#include <stdlib.h>
#include "bullet.h"
#include "enemy.h"
#include "gun.h"
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
  GameLevelState* level = malloc(sizeof(GameLevelState));
  if (level == NULL) {
    return NULL;
  }
  Player p = {
      .health = 1,
      .position = create_player_start_pos(terminal_size),
      .velocity = zero_vec(),
      .gun = new_gun_default_player(),
  };

  Pool* bullet_pool = new_object_pool(sizeof(bullet), 20);
  Enemy e = new_enemy((Vec2i){terminal_size.x / 2, 4}, 10);

  level->title = title;
  level->player = p;
  level->enemy = e;
  level->frame_count = 1;
  level->bullet_pool = bullet_pool;
  return level;
}

void RemoveGameLevel(GameLevelState* level) {
  if (level == NULL) {
    return;
  }
  if (level->bullet_pool != NULL) {
    remove_object_pool(level->bullet_pool);
  }

  free(level);
}
