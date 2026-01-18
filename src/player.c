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
