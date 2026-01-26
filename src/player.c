#include "./player.h"
#include <assert.h>

Player new_player() {
  return (Player){
      .health = 1,
      .position = zero_vec(),
      .velocity = zero_vec(),
      .gun = new_gun_default_player(),
  };
}
