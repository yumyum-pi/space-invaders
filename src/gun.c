#include "./gun.h"
#include <assert.h>
#include "utils/math.h"

const Vec2i PLAYER_BULLET_DIRECTION = {
    .x = 0,
    .y = 1,
};

const Vec2i ENEMY_BULLET_DIRECTION = {
    .x = 0,
    .y = -1,
};

Gun new_gun(int fire_rate, int reload_rate, int magazine_size,
            Vec2i direction) {
  Gun g = {
      .direction = direction,
      .bullet_type = 0,
      .fire_rate = fire_rate,
      .reload_rate = reload_rate,
      .last_fired_frame = -fire_rate,
      .magazine_size = magazine_size,
      .remaining_rounds = magazine_size,
  };
  return g;
};

// TODO: remove the following number with FPS
Gun new_gun_default_player(void) {
  return new_gun(1 * 24, 5 * 24, 5, PLAYER_BULLET_DIRECTION);
}

Gun new_gun_default_enemy(void) {
  return new_gun(2 * 24, 5 * 24, 15, ENEMY_BULLET_DIRECTION);
}

// set reload false
// fill the remaining_rounds
void gun_reload(Gun* g) {
  g->is_reloading = false;
  g->remaining_rounds = g->magazine_size;
}

// reset last_fired_frame
// constume ammo
// trigger reload if needed
void gun_execute_shot(Gun* g, int frame_count) {
  g->last_fired_frame = frame_count;
  g->remaining_rounds--;
  if (g->remaining_rounds == 0) {
    g->is_reloading = true;
  }
}

bool gun_fire(Gun* g, int frame_count, bool should_fire) {
  assert(frame_count >= g->last_fired_frame);

  const int last_fire_diff = (frame_count - g->last_fired_frame);
  // check if reloading every frame to make sure that is relaoding is false
  // as soon as the last fire diff is more than relaod rate
  if (g->is_reloading) {
    if (last_fire_diff >= g->reload_rate) {
      gun_reload(g);
    } else {
      return false;  // Still reloading
    }
  }

  // check if gun should fired
  if (should_fire) {
    bool can_fire_yet = last_fire_diff >= g->fire_rate;
    bool has_ammo = g->remaining_rounds > 0;

    if (can_fire_yet && has_ammo) {
      gun_execute_shot(g, frame_count);
      return true;
    }
  }
  return false;
}
