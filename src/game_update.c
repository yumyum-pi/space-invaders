#include "./game_update.h"
#include <assert.h>
#include "bullet.h"
#include "enemy.h"
#include "game_state.h"
#include "input.h"
#include "player.h"
#include "utils/math.h"

void update_player_velocity(const Input* in, Vec2i* v) {
  {
    const Vec2i accleration = {
        .x = 1,
        .y = 1,
    };
    if (in->ay == 0) {
      v->y = move_to(v->y, 0, 1);
    } else if (in->ay > 0) {
      v->y -= accleration.y;
    } else if (in->ay < 0) {
      v->y += accleration.y;
    }
    if (in->ax == 0) {
      v->x = move_to(v->x, 0, 1);
    } else if (in->ax < 0) {
      v->x -= accleration.x;
    } else if (in->ax > 0) {
      v->x += accleration.x;
    }
  }

  {
    const Vec2i max_velocity = {
        .x = 2,
        .y = 2,
    };
    // clamp the velocity
    v->x = clamp(v->x, -max_velocity.x, max_velocity.x);
    v->y = clamp(v->y, -max_velocity.y, max_velocity.y);
  }
  return;
}

void update_player_position(Vec2i* position, Vec2i* velocity, GameState* g) {
  position->x += velocity->x;
  position->y += velocity->y;
  // clamp
  if (!is_point_in_rect(*position, g->bounds_min, g->bounds_max)) {
    *position = clamp_vec2i(*position, g->bounds_min, g->bounds_max);
    *velocity = zero_vec();
  }
}
static long timespec_diff_ms(const struct timespec* a,
                             const struct timespec* b) {
  return (a->tv_sec - b->tv_sec) * 1000L + (a->tv_nsec - b->tv_nsec) / 1000000L;
}
void frame_begin(GameState* gs) {
  clock_gettime(CLOCK_MONOTONIC, &gs->frame_start);
}
void frame_sleep(GameState* g) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  long elapsed = timespec_diff_ms(&now, &g->frame_start);
  long remaining = g->target_frame_ms - elapsed;

  if (remaining <= 0) {
    return;
  }

  struct timespec sleep_time = {
      .tv_sec = remaining / 1000,
      .tv_nsec = (remaining % 1000) * 1000000L,
  };

  nanosleep(&sleep_time, NULL);
}

int ping_pong(int min, int max, float speed, int frame_count) {
  int diff = max - min;
  int cycle_length = diff * 2;

  float progress = (float)((int)(frame_count * speed) % cycle_length);

  float t = progress / diff;
  if (t > 1.0f)
    t = 2.0f - t;

  return (int)(min + (t * diff));
}
// Ping-pong the progress so it goes 0 -> 1 -> 0
int ping_pong_ease_in_out(int min, int max, float speed, int frame_count) {
  int diff = max - min;
  int cycle_length = diff * 2;

  float progress = (float)((int)(frame_count * speed) % cycle_length);

  float t = progress / diff;
  if (t > 1.0f)
    t = 2.0f - t;

  // curve the linear "t" into a smooth S-shape
  float eased_t = t * t * (3.0f - 2.0f * t);

  return (int)(min + (eased_t * diff));
}

void update_enemy(Enemy* e, int frame_count, Vec2i terminal_size) {
  {
    float speed = e->speed;
    // TODO: the bound_offset_x should not be hard coded
    int offset = 64;  // left and right extrems of ping pong positions
    int x = ping_pong_ease_in_out(-offset, offset, speed, frame_count);
    e->position.x = e->target_position.x + x;
  }
  // TODO: the bonding box should be defined by the gamestate
  if (!is_point_in_rect(e->position, zero_vec(), terminal_size)) {
    e->position = clamp_vec2i(e->position, zero_vec(), terminal_size);
  }
}
void update_bullet(GameState* g) {
  Vec2i* position = &(g->bullet.position);
  position->y -= g->bullet.speed;

  // bounds
  if (!is_point_in_rect(*position, zero_vec(), g->terminal_size)) {
    *position = zero_vec();
    g->bullet.is_active = false;
  }
};

void fire_bullet(GameState* g, Vec2i pos) {
  g->bullet.is_active = true;
  g->bullet.position = pos;
};

void player_bullet(bullet* pb, Enemy* e) {
  // check if they are in the same coordinates
  if (is_eq_vec2i(pb->position, e->position)) {
    e->is_active = false;
  }
};

void update_collision(GameState* g) {
  // check if the bullet has the same position as enemey
  bullet* pb = &(g->bullet);
  Enemy* e = &(g->enemy);
  player_bullet(pb, e);
}

void update_player(GameState* g, const Input* in) {
  bool fire = in->fire;
  Player* p = &g->player;
  Vec2i* v = &p->velocity;
  Vec2i* pos = &p->position;
  update_player_velocity(in, v);
  update_player_position(pos, v, g);
  Gun* gun = &(p->gun);
  if (fire && gun_should_fire(gun, g->frame_count)) {
    gun->last_fired_frame = g->frame_count;
    fire_bullet(g, *pos);
  }
}

void game_update(GameState* g) {
  const Input* in = &(g->input);
  if (in->quit) {
    g->is_running = 0;
    return;
  }
  update_player(g, in);
  // only update the enemey when the enemy is is_active
  {
    Enemy* e = &g->enemy;
    if (e->is_active) {
      update_enemy(e, g->frame_count, g->terminal_size);
    }
  }

  if (g->bullet.is_active) {
    update_bullet(g);
  }

  // check for collision
  update_collision(g);
}
