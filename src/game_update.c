#include "./game_update.h"
#include "utils/math.h"

void game_update(GameState *g, const Input *in, int dt) {

  if (in->quit) {
    g->is_running = 0;
    return;
  }

  Player *p = &g->player;
  Vec2i *v = &p->velocity;

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
  } else {
    p->velocity = move_to_vec2i(p->velocity, zero_vec(), 1);
  }

  p->position.x += p->velocity.x / (dt / 2);
  p->position.y += p->velocity.y / (dt / 2);

  // clamp
  if (!is_point_in_rect(p->position, g->bounds_min, g->bounds_max)) {
    p->position = clamp_vec2i(p->position, g->bounds_min, g->bounds_max);
    p->velocity = zero_vec();
  }
}
