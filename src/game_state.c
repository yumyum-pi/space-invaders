#include "game_state.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "utils/math.h"

Vec2i create_player_start_pos(Vec2i terminal_size) {
  return (Vec2i){
      .x = terminal_size.x / 2, // middle of the screen
      .y = terminal_size.y - 8,
  };
}
GameState game_state_init(Vec2i terminal_size, const char *title) {
  Player p = {
      .position = create_player_start_pos(terminal_size),
      .velocity = zero_vec(),
      .gun = new_gun_default(),
  };

  bullet b = {
      .position = zero_vec(),
      .fired_by = 0,
      .speed = 1,
      .is_active = true,
  };

  Enemy e = new_enemy((Vec2i){terminal_size.x / 2, 4}, 10);
  Vec2i bounds_min = {
      .x = 16,
      .y = terminal_size.y - 32,
  };

  Vec2i bounds_max = {
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };
  return (GameState){
      .is_running = true,
      .title = title,
      .terminal_size = terminal_size,
      .target_frame_ms = FRAME_MS,
      .bounds_min = bounds_min,
      .bounds_max = bounds_max,
      .player = p,
      .enemy = e,
      .frame_count = 1,
      .bullet = b,
  };
}
