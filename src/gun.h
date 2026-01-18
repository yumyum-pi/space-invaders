#ifndef GUN_H
#define GUN_H
#include <stdbool.h>
#include "utils/math.h"

typedef struct {
  Vec2i direction;
  int bullet_type;
  int fire_rate;
  int magazine_size, remaining_rounds;
  int reload_rate;
  int last_fired_frame;
  bool is_reloading;
} Gun;

Gun new_gun(int fire_rate, int reload_rate, int magazine_size, Vec2i direction);
Gun new_gun_default(void);
bool gun_fire(Gun* g, int frame_count, bool should_fire);
#endif  // !GUN_H
