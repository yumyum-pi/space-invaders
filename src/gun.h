#ifndef GUN_H
#define GUN_H
#include <stdbool.h>
#include "utils/math.h"

typedef enum {
  GUN_LASER,
  GUN_PLASMA,
  GUN_GATLING,
  GUN_BURST,
  GUN_LASER_PLAYER,
  GUN_COUNT
} GunType;

typedef struct {
  int fire_rate;
  int magazine_size, remaining_rounds;
  int reload_rate;
  int last_fired_frame;
  bool is_reloading;
} Gun;

Gun new_gun(int fire_rate, int reload_rate, int magazine_size);

void SetGun(Gun* g, GunType type);
Gun GetGun(GunType type);

bool gun_fire(Gun* g, int frame_count, bool should_fire);
#endif  // !GUN_H
