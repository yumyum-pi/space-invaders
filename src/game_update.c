#include "./game_update.h"
#include <assert.h>
#include "../lib/object_pool/object_pool.h"
#include "bullet.h"
#include "enemy.h"
#include "game_level_state.h"
#include "game_state.h"
#include "input.h"
#include "player.h"
#include "utils/math.h"

static Vec2i bounding_box_vec[2] = {
    (Vec2i){0, 0},
    (Vec2i){0, 0},
};

static Vec2i player_bounding_box_vec[2] = {
    (Vec2i){0, 0},
    (Vec2i){0, 0},
};

void game_update_init(Vec2i terminal_size) {
  Vec2i t = terminal_size;
  t.x--;
  t.y--;
  assert(t.x > 0);
  assert(t.y > 0);
  bounding_box_vec[1] = t;

  player_bounding_box_vec[0] = (Vec2i){
      .x = 16,
      .y = terminal_size.y - 32,
  };

  player_bounding_box_vec[1] = (Vec2i){
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };
}

bool is_Inbounds(Vec2i position) {
  return is_point_in_rect(position, bounding_box_vec[0], bounding_box_vec[1]);
}

void fire_bullet(GameLevelState* level, Vec2i direction, Vec2i pos) {
  // borrow bullet
  bullet* b = object_pool_borrow(level->bullet_pool);
  // check if bullet is not NULL
  if (b == NULL) {
    return;
  }
  b->position = pos;
  b->is_active = true;
  b->speed = direction.y;
};

void update_player_velocity(const GameInput* in, Vec2i* v) {
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

void update_player_position(Vec2i* position, Vec2i* velocity) {
  position->x += velocity->x;
  position->y += velocity->y;
  // clamp
  if (!is_point_in_rect(*position, player_bounding_box_vec[0],
                        player_bounding_box_vec[1])) {
    *position = clamp_vec2i(*position, player_bounding_box_vec[0],
                            player_bounding_box_vec[1]);
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

void frame_sleep(GameState* gs) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  long elapsed = timespec_diff_ms(&now, &gs->frame_start);
  long remaining = gs->target_frame_ms - elapsed;

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

void update_enemy(GameLevelState* level, Enemy* e, int frame_count) {
  {
    float speed = e->speed;
    // TODO: the bound_offset_x should not be hard coded
    int offset = 32;  // left and right extrems of ping pong positions
    int x = ping_pong_ease_in_out(-offset, offset, speed, frame_count);
    e->position.x = e->target_position.x + x;
    e->position.y += 1 / (frame_count % 48);
  }

  if (!is_Inbounds(e->position)) {
    e->is_active = false;
  }

  Gun* gun = &(e->gun);

  if (gun_fire(gun, level->frame_count, true)) {
    fire_bullet(level, gun->direction, e->position);
  }
}
//
typedef struct {
  Vec2i terminal_size;
  Pool* pool;  // to return object to the pool
} BulletUpdateContext;

void update_bullet_itr_wrapper(void* payload, void* args) {
  bullet* b = (bullet*)payload;
  BulletUpdateContext* context = (BulletUpdateContext*)args;
  Vec2i* position = &(b->position);
  position->y -= b->speed;

  if (!is_Inbounds(*position)) {
    *position = zero_vec();
    // return the bullet
    object_pool_return(context->pool, b);
  }
};

typedef struct {
  Player* player;
  Enemy* enemy;       // to return object to the pool
  Pool* bullet_pool;  // to return object to the pool
} BulletCollisionContext;

// How to handle enemy from object pool
void bullet_obj_pool_enemy_itr_wrapper(void* payload, void* args) {
  bullet* b = (bullet*)payload;
  BulletCollisionContext* context = (BulletCollisionContext*)args;

  // enemy bullet speed = -1;
  // enmey bullet check of player collision
  if (b->speed == -1 && is_eq_vec2i(b->position, context->player->position)) {
    context->player->health--;
    object_pool_return(context->bullet_pool, b);  // return obj
    //player bullet check for enmey
  } else if (b->speed == 1 &&
             is_eq_vec2i(b->position, context->enemy->position)) {
    context->enemy->is_active = false;
    object_pool_return(context->bullet_pool, b);  // return obj
  }
}

void update_collision(GameLevelState* level) {
  BulletCollisionContext collisionContext = {
      .bullet_pool = level->bullet_pool,
      .player = &(level->player),
      .enemy = &(level->enemy),
  };

  object_pool_itr(level->bullet_pool, bullet_obj_pool_enemy_itr_wrapper,
                  &collisionContext);
}

void update_player(GameLevelState* level, const GameInput* in) {
  bool fire = in->fire;
  Player* p = &level->player;
  Vec2i* v = &p->velocity;
  Vec2i* pos = &p->position;
  update_player_velocity(in, v);
  update_player_position(pos, v);
  Gun* gun = &(p->gun);
  if (gun_fire(gun, level->frame_count, fire)) {
    fire_bullet(level, gun->direction, *pos);
  }
}

void game_update(GameState* gs, GameLevelState* level) {
  const GameInput* in = &(level->input);
  if (in->quit) {
    gs->mode = GAME_PAUSE;
    return;
  }
  update_player(level, in);
  // only update the enemey when the enemy is is_active
  {
    Enemy* e = &level->enemy;
    if (e->is_active) {
      update_enemy(level, e, level->frame_count);
    }
  }
  BulletUpdateContext bulletUpdateContext = {
      .terminal_size = gs->terminal_size,
      .pool = level->bullet_pool,
  };

  // update the position of the bullets
  object_pool_itr(level->bullet_pool, update_bullet_itr_wrapper,
                  &bulletUpdateContext);

  // check for collision
  update_collision(level);

  if (level->player.health <= 0) {
    gs->is_running = false;
  }
}
