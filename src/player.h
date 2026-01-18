#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"

typedef struct {
  int fire_rate;
  int magazine_size, remaining_rounds;
  int reload_rate;
  int last_fired_frame;
  bool is_reloading;
} Gun;

typedef struct {
  Vec2i position;
  Vec2i velocity;
  Gun gun;
} Player;

Player new_player(void);

Gun new_gun(int fire_rate, int reload_rate, int magazine_size);
Gun new_gun_default(void);

bool gun_fire(Gun* g, int frame_count, bool should_fire);
bool gun_should_fire(Gun* g, int frame_count);

#endif  // !PLAYER_H
