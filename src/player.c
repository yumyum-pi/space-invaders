#include "./player.h"
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

Gun new_gun_default(void) { return new_gun(1, 5); }
