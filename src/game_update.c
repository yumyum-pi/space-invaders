#include "./game_update.h"
#include <assert.h>
#include <stdio.h>
#include "../lib/object_pool/object_pool.h"
#include "bullet.h"
#include "enemy.h"
#include "game_level_state.h"
#include "game_state.h"
#include "gun.h"
#include "input.h"
#include "level.h"
#include "player.h"

const IVec2 ENEMY_BULLET_DIRECTION = {
    .x = 0,
    .y = -1,
};

const IVec2 PLAYER_BULLET_DIRECTION = {
    .x = 0,
    .y = 1,
};

static IRect WorldRect;
static IRect PlayerBoundingIRect;
static IRect EnemyBoundingIRect;

void game_update_init(IVec2 terminal_size) {
  IVec2 t = terminal_size;
  WorldRect = IRectZero();
  t.x--;
  t.y--;
  assert(t.x > 0);
  assert(t.y > 0);
  WorldRect.max = t;

  PlayerBoundingIRect.min = (IVec2){
      .x = 16,
      .y = terminal_size.y - 32,
  };

  PlayerBoundingIRect.max = (IVec2){
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };

  EnemyBoundingIRect.min = (IVec2){
      .x = 16,
      .y = 0,
  };

  EnemyBoundingIRect.max = (IVec2){
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 1,
  };
}

bool IsInWord(IVec2 position) {
  return IRectColideIVec2(WorldRect, position);
}

void fire_bullet(GameLevelState* level, IVec2 direction, IVec2 pos) {
  // borrow bullet
  bullet* b = object_pool_borrow(level->bullet_pool);
  // check if bullet is not NULL
  assert(b != NULL);
  if (b == NULL) {
    return;
  }
  b->position = pos;
  b->is_active = true;
  b->speed = direction.y;
};

void update_player_velocity(const GameInput* in, IVec2* v) {
  {
    const IVec2 accleration = {
        .x = 1,
        .y = 1,
    };
    if (in->ay == 0) {
      v->y = IMoveTowards(v->y, 0, 1);
    } else if (in->ay > 0) {
      v->y -= accleration.y;
    } else if (in->ay < 0) {
      v->y += accleration.y;
    }
    if (in->ax == 0) {
      v->x = IMoveTowards(v->x, 0, 1);
    } else if (in->ax < 0) {
      v->x -= accleration.x;
    } else if (in->ax > 0) {
      v->x += accleration.x;
    }
  }

  {
    const IVec2 max_velocity = {
        .x = 2,
        .y = 2,
    };
    // clamp the velocity
    v->x = IClamp(v->x, -max_velocity.x, max_velocity.x);
    v->y = IClamp(v->y, -max_velocity.y, max_velocity.y);
  }
  return;
}

void update_player_position(IVec2* position, IVec2* velocity) {
  position->x += velocity->x;
  position->y += velocity->y;
  // clamp
  if (!IRectColideIVec2(PlayerBoundingIRect, *position)) {
    *position =
        IVec2Clamp(*position, PlayerBoundingIRect.min, PlayerBoundingIRect.max);
    *velocity = IVec2Zero();
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

void update_enemy(GameLevelState* level, Enemy* e) {
  {
    float speed = e->speed;
    // TODO: the bound_offset_x should not be hard coded
    int offset = 32;  // left and right extrems of ping pong positions
    int x = ping_pong_ease_in_out(-offset, offset, speed, level->frame_count);
    e->position.x = e->ancher_position.x + x;
    e->ancher_position.y += 1 / (level->frame_count % 12);
    e->position.y = e->ancher_position.y;
  }

  if (!IRectColideIVec2(EnemyBoundingIRect, e->position)) {
    e->position =
        IVec2Clamp(e->position, EnemyBoundingIRect.min, EnemyBoundingIRect.max);
  }

  if (!IsInWord(e->position)) {
    e->is_active = false;
  }

  Gun* gun = &(e->gun);

  if (gun_fire(gun, level->frame_count, true)) {
    fire_bullet(level, ENEMY_BULLET_DIRECTION, e->position);
  }
}
//
typedef struct {
  IVec2 terminal_size;
  Pool* pool;  // to return object to the pool
} BulletUpdateContext;

bool update_bullet_itr_wrapper(void* payload, void* args) {
  bullet* b = (bullet*)payload;
  BulletUpdateContext* context = (BulletUpdateContext*)args;
  IVec2* position = &(b->position);
  position->y -= b->speed;

  if (!IsInWord(*position)) {
    *position = IVec2Zero();
    // return the bullet
    object_pool_return(context->pool, b);
  }
  return true;
};

typedef struct {
  Player* player;
  Pool* enemy_pool;   // to return object to the pool
  Pool* bullet_pool;  // to return object to the pool
} BulletCollisionContext;

typedef struct {
  bullet* b;
  Pool* enemy_pool;
  bool* hit;
} bullet_enemy_context;

bool bullet_collision_to_enemy_itr(void* payload, void* args) {
  Enemy* e = (Enemy*)payload;
  bullet_enemy_context* context = (bullet_enemy_context*)args;

  if (IVec2Equal(context->b->position, e->position)) {
    *context->hit = true;
    e->is_active = false;
    object_pool_return(context->enemy_pool, e);
    return false;
  }
  return true;
}

// How to handle enemy from object pool
bool bullet_obj_pool_enemy_itr_wrapper(void* payload, void* args) {
  bullet* b = (bullet*)payload;
  BulletCollisionContext* context = (BulletCollisionContext*)args;

  // enemy bullet speed = -1;
  // enemy bullet check of player collision
  if (b->speed == -1 && IVec2Equal(b->position, context->player->position)) {
    context->player->health--;
    object_pool_return(context->bullet_pool, b);  // return obj
    //player bullet check for enemy
  } else if (b->speed == 1) {
    bool hit = false;
    bullet_enemy_context ctx = (bullet_enemy_context){
        .b = b,
        .enemy_pool = context->enemy_pool,
        .hit = &hit,
    };
    object_pool_itr(context->enemy_pool, bullet_collision_to_enemy_itr, &ctx);
    if (hit) {
      object_pool_return(context->bullet_pool, b);  // return obj
    }
  }
  return true;
}

void update_collision(GameLevelState* level) {
  BulletCollisionContext collisionContext = {
      .bullet_pool = level->bullet_pool,
      .player = level->player,
      .enemy_pool = level->enemy_pool,
  };
  object_pool_itr(level->bullet_pool, bullet_obj_pool_enemy_itr_wrapper,
                  &collisionContext);
}

void update_player(GameLevelState* level, const GameInput* in) {
  bool fire = in->fire;
  Player* p = level->player;
  IVec2* v = &p->velocity;
  IVec2* pos = &p->position;
  update_player_velocity(in, v);
  update_player_position(pos, v);
  Gun* gun = &(p->gun);
  if (gun_fire(gun, level->frame_count, fire)) {
    fire_bullet(level, PLAYER_BULLET_DIRECTION, *pos);
  }
}

typedef struct {
  GameLevelState* level;
  Pool* pool;  // to return object to the pool
} EnemyUpdateContext;

bool update_enemy_itr_wrapper(void* payload, void* args) {
  Enemy* e = (Enemy*)payload;
  EnemyUpdateContext* context = (EnemyUpdateContext*)args;
  update_enemy(context->level, e);
  return true;
};

IVec2 center_offset(IVec2 terminal_size, IVec2 offset) {
  return (IVec2){
      .x = IClamp(terminal_size.x / 2 + offset.x, 0, terminal_size.x),
      .y = offset.y,
  };
}

void game_update(GameState* gs, GameLevelState* level_state) {
  const GameInput* in = &(level_state->input);
  if (in->quit) {
    gs->mode = GAME_PAUSE;
    return;
  }

  SpawnEvent* s;
  while ((s = LevelGetSpawnEvent(level_state->level,
                                 level_state->frame_count)) != NULL) {
    // spawn
    switch (s->entity) {
      case ENEMY: {
        EntityArgsEnemy args = s->args.enemyArgs;
        Enemy* e = object_pool_borrow(level_state->enemy_pool);
        assert(e != NULL);
        SetEnemy(e, args.type,
                 center_offset(gs->terminal_size, args.start_position));
        break;
      }
      default:
        printf("This should not be true");
        assert(false);
        return;
    }
  }
  update_player(level_state, in);

  EnemyUpdateContext enemyUpdateContext = {
      .level = level_state,
      .pool = level_state->bullet_pool,
  };
  object_pool_itr(level_state->enemy_pool, update_enemy_itr_wrapper,
                  &enemyUpdateContext);

  BulletUpdateContext bulletUpdateContext = {
      .terminal_size = gs->terminal_size,
      .pool = level_state->bullet_pool,
  };
  // update the position of the bullets
  object_pool_itr(level_state->bullet_pool, update_bullet_itr_wrapper,
                  &bulletUpdateContext);

  // check for collision
  update_collision(level_state);

  if (level_state->player->health <= 0) {
    // TODO: pass the score here
    gs->mode = GAME_END;
  }
}
