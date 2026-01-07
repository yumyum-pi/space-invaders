#include "./game_update.h"
#include "enemy.h"
#include "game_state.h"
#include "input.h"
#include "utils/math.h"
#include <assert.h>

void update_player_velocity(const Input *in, Vec2i *v, int dt) {
  if (in->is_input) {

    if (in->ay > 0) {
      v->y -= 1 * dt;
    } else if (in->ay < 0) {
      v->y += 1 * dt;
    }

    if (in->ax < 0) {
      v->x -= 1 * dt;
    } else if (in->ax > 0) {
      v->x += 1 * dt;
    }

    // clamp the velocity
    v->x = clamp(v->x, -5 * dt, 5 * dt);
    v->y = clamp(v->y, -4, 4);
    return;
  }
  *v = move_to_vec2i(*v, zero_vec(), 1);
}
void update_player_position(Vec2i *position, Vec2i *velocity, int dt,
                            GameState *g) {

  position->x += velocity->x / (dt / 2);
  position->y += velocity->y / (dt / 2);

  // clamp
  //
  if (!is_point_in_rect(*position, g->bounds_min, g->bounds_max)) {
    *position = clamp_vec2i(*position, g->bounds_min, g->bounds_max);
    *velocity = zero_vec();
  }
}
static long timespec_diff_ms(const struct timespec *a,
                             const struct timespec *b) {
  return (a->tv_sec - b->tv_sec) * 1000L + (a->tv_nsec - b->tv_nsec) / 1000000L;
}
void frame_begin(GameState *gs) {
  clock_gettime(CLOCK_MONOTONIC, &gs->frame_start);
}
void frame_sleep(GameState *g) {
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

void move_enemy(Enemy *e, int frame_count, Vec2i terminal_size) {
  float speed_x = 2.0f;
  int bound_offset_x = 64;
  int x = ping_pong_ease_in_out(-bound_offset_x, bound_offset_x, speed_x,
                                frame_count);

  e->position.x = e->target_position.x + x;
  if (!is_point_in_rect(e->position, zero_vec(), terminal_size)) {
    e->position = clamp_vec2i(e->position, zero_vec(), terminal_size);
    e->velocity = zero_vec();
  }
}

void update_player(GameState *g, const Input *in, int dt) {
  Player *p = &g->player;
  Vec2i *v = &p->velocity;
  Vec2i *pos = &p->position;
  update_player_velocity(in, v, dt);
  update_player_position(pos, v, dt, g);
}

void game_update(GameState *g, const Input *in, int dt) {
  if (in->quit) {
    g->is_running = 0;
    return;
  }
  update_player(g, in, dt);
  move_enemy(&g->enemy, g->frame_count, g->terminal_size);
}
