#ifndef PLAYER_H
#define PLAYER_H

#include "./utils/math.h"

typedef struct {
  int fire_rate;
  int relaod_rate;
  int last_fired_frame;
} Gun;

typedef struct {
  Vec2i position;
  Vec2i velocity;
  Gun gun;
} Player;

Player new_player(void);

Gun new_gun(int fire_rate, int reload_rate);
Gun new_gun_default(void);

#endif // !PLAYER_H
