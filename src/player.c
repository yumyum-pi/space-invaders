#include "./player.h"
#include <assert.h>
#include "utils/math.h"

Player new_player() {
  return (Player){
      .position = zero_vec(),
      .velocity = zero_vec(),
      .gun = new_gun_default(),
  };
}

Gun new_gun(int fire_rate, int reload_rate) {
  Gun g = {
      .fire_rate = fire_rate,
      .relaod_rate = reload_rate,
      .last_fired_frame = 0,
  };
  return g;
};

// TODO: remove the following number with FPS
Gun new_gun_default(void) {
  return new_gun(1 * 24, 5);
}

bool gun_should_fire(Gun* g, int frame_count) {
  // CASE: interger overflow in frame_count
  assert(frame_count > g->last_fired_frame);
  return g->fire_rate < (frame_count - g->last_fired_frame);
};
