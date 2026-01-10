#include "./enemy.h"
#include "utils/math.h"

Enemy new_enemy(Vec2i position, int health) {
  return (Enemy){
      .position = position,
      .target_position = position,
      .velocity = {0, 0},
      .health = health,
      .is_active = true,
      .speed = 0.1f,
  };
}
