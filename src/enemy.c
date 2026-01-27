#include "./enemy.h"
#include "gun.h"
#include "utils/math.h"

Enemy new_enemy(Vec2i position, int health) {
  return (Enemy){
      .position = position,
      .gun = new_gun_default_enemy(),
      .target_position = position,
      .velocity = {0, 2},
      .health = health,
      .is_active = true,
      .speed = 0.2f,
  };
}
