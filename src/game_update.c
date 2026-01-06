#include "./game_update.h"
#include "input.h"
#include "utils/math.h"

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

void game_update(GameState *g, const Input *in, int dt) {

  if (in->quit) {
    g->is_running = 0;
    return;
  }

  Player *p = &g->player;
  Vec2i *v = &p->velocity;
  Vec2i *pos = &p->position;
  update_player_velocity(in, v, dt);
  update_player_position(pos, v, dt, g);
}
