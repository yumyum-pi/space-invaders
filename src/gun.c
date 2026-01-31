#include "./gun.h"
#include <assert.h>
#include "utils/math.h"

static const Gun GUN_DEFS[GUN_COUNT] = {
    [GUN_LASER] =
        {
            .fire_rate = 10,
            .magazine_size = 20,
            .reload_rate = 60,
        },
    [GUN_PLASMA] =
        {
            .fire_rate = 25,
            .magazine_size = 5,
            .reload_rate = 90,
        },
    [GUN_GATLING] =
        {
            .fire_rate = 3,
            .magazine_size = 100,
            .reload_rate = 120,
        },
    [GUN_BURST] =
        {
            .fire_rate = 2,
            .magazine_size = 6,
            .reload_rate = 45,
        },
};

Gun new_gun(int fire_rate, int reload_rate, int magazine_size) {
  Gun g = {
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
  return new_gun(1 * 24, 5 * 24, 5);
}

Gun new_gun_default_enemy(void) {
  return new_gun(2 * 24, 5 * 24, 15);
}

void SetGun(Gun* g, GunType type) {
  type = clamp(type, 0, GUN_COUNT);
  Gun def = GUN_DEFS[type];

  g->fire_rate = def.fire_rate;
  g->magazine_size = def.magazine_size;
  g->reload_rate = def.reload_rate;
  g->remaining_rounds = def.magazine_size;
  g->last_fired_frame = 0;
  g->is_reloading = false;
}

Gun GetGun(GunType type) {
  type = clamp(type, 0, GUN_COUNT);
  Gun def = GUN_DEFS[type];
  return (Gun){
      .fire_rate = def.fire_rate,
      .magazine_size = def.magazine_size,
      .reload_rate = def.reload_rate,
      .remaining_rounds = def.magazine_size,
      .last_fired_frame = 0,
      .is_reloading = false,
  };
}

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
