#include "./game_update.h"
#include <assert.h>
#include <limits.h>
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
#include "utils/math/irect.h"

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
#define WORLD_PADDING_X 8

void game_update_init(IVec2 terminal_size) {
  IVec2 t = terminal_size;
  WorldRect = IRectZero();
  t.x--;
  t.y--;
  assert(t.x > 0);
  assert(t.y > 0);
  WorldRect.max = t;

  PlayerBoundingIRect.min = (IVec2){
      .x = WORLD_PADDING_X,
      .y = terminal_size.y - 32,
  };

  PlayerBoundingIRect.max = (IVec2){
      .x = terminal_size.x - WORLD_PADDING_X,
      .y = terminal_size.y - 8,
  };

  EnemyBoundingIRect.min = (IVec2){
      .x = WORLD_PADDING_X,
      .y = 0,
  };

  EnemyBoundingIRect.max = (IVec2){
      .x = terminal_size.x - WORLD_PADDING_X,
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
        .x = 3,
        .y = 3,
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
        .x = 3,
        .y = 3,
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

#define FORMATION_SPEED_X 1
#define FORMATION_DROP_Y  2

typedef struct {
  int  min_x;
  int  max_x;
  bool found;
} BoundsCtx;

bool bounds_itr(void* payload, void* args) {
  Enemy* e    = (Enemy*)payload;
  BoundsCtx* ctx = (BoundsCtx*)args;
  if (e->position.x < ctx->min_x) ctx->min_x = e->position.x;
  if (e->position.x > ctx->max_x) ctx->max_x = e->position.x;
  ctx->found = true;
  return true;
}

typedef struct {
  Formation*      f;
  GameLevelState* level;
} MoveCtx;

bool move_enemy_itr(void* payload, void* args) {
  Enemy* e    = (Enemy*)payload;
  MoveCtx* ctx = (MoveCtx*)args;
  e->position.x        += ctx->f->dir * FORMATION_SPEED_X;
  e->ancher_position.x += ctx->f->dir * FORMATION_SPEED_X;
  if (ctx->f->drop) {
    e->position.y        += FORMATION_DROP_Y;
    e->ancher_position.y += FORMATION_DROP_Y;
  }
  if (gun_fire(&e->gun, ctx->level->frame_count, true))
    fire_bullet(ctx->level, ENEMY_BULLET_DIRECTION, e->position);
  if (!IsInWord(e->position)) e->is_active = false;
  return true;
}

void update_enemies_formation(GameLevelState* level) {
  BoundsCtx bounds = {.min_x = INT_MAX, .max_x = INT_MIN, .found = false};
  object_pool_itr(level->enemy_pool, bounds_itr, &bounds);
  if (!bounds.found) return;

  Formation* f = &level->formation;
  int nx_min = bounds.min_x + f->dir * FORMATION_SPEED_X;
  int nx_max = bounds.max_x + f->dir * FORMATION_SPEED_X;
  if (nx_min <= EnemyBoundingIRect.min.x || nx_max >= EnemyBoundingIRect.max.x) {
    f->dir  = -f->dir;
    f->drop = true;
  }
  MoveCtx ctx = {.f = f, .level = level};
  object_pool_itr(level->enemy_pool, move_enemy_itr, &ctx);
  f->drop = false;
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
  Pool* enemy_pool;
  Pool* bullet_pool;
  int* score;
  Shield* shields;
  int shield_count;
  Explosion* explosions;
} BulletCollisionContext;

typedef struct {
  bullet* b;
  Pool* enemy_pool;
  bool* hit;
  int* score;
  Explosion* explosions;
} bullet_enemy_context;

static void spawn_explosion(Explosion* exps, IVec2 pos) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (!exps[i].is_active) {
      exps[i] = (Explosion){pos, EXPLOSION_FRAMES, true};
      return;
    }
  }
}

bool bullet_collision_to_enemy_itr(void* payload, void* args) {
  Enemy* e = (Enemy*)payload;
  bullet_enemy_context* context = (bullet_enemy_context*)args;

  if (IRectColideIVec2(IRectFromCenter(e->position, e->rectSize), context->b->position)) {
    *context->hit = true;
    (*context->score)++;
    spawn_explosion(context->explosions, e->position);
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

  // shields block all bullets
  for (int i = 0; i < context->shield_count; i++) {
    Shield* s = &context->shields[i];
    if (!s->is_active) continue;
    if (IRectColideIVec2(IRectFromCenter(s->position, (IVec2){3, 1}), b->position)) {
      s->hp--;
      if (s->hp <= 0) s->is_active = false;
      object_pool_return(context->bullet_pool, b);
      return true;
    }
  }

  // enemy bullet speed = -1;
  // enemy bullet check of player collision
  if (b->speed == -1 &&
      context->player->respawn_frames == 0 &&
      IRectColideIVec2(
          IRectFromCenter(context->player->position, context->player->rectSize),
          b->position)) {
    context->player->health--;
    object_pool_return(context->bullet_pool, b);  // return obj
    //player bullet check for enemy
  } else if (b->speed == 1) {
    bool hit = false;
    bullet_enemy_context ctx = (bullet_enemy_context){
        .b = b,
        .enemy_pool = context->enemy_pool,
        .hit = &hit,
        .score = context->score,
        .explosions = context->explosions,
    };
    object_pool_itr(context->enemy_pool, bullet_collision_to_enemy_itr, &ctx);
    if (hit) {
      object_pool_return(context->bullet_pool, b);  // return obj
    }
  }
  return true;
}

static void update_explosions(GameLevelState* level) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (level->explosions[i].is_active)
      if (--level->explosions[i].frames_remaining <= 0)
        level->explosions[i].is_active = false;
  }
}

void update_collision(GameLevelState* level) {
  BulletCollisionContext collisionContext = {
      .bullet_pool = level->bullet_pool,
      .player = level->player,
      .enemy_pool = level->enemy_pool,
      .score = &level->score,
      .shields = level->shields,
      .shield_count = level->shield_count,
      .explosions = level->explosions,
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

void tutorial_update(GameState* gs, GameLevelState* level) {
  const GameInput* in = &level->input;
  if (in->quit) {
    gs->mode = MENU;
    return;
  }

  update_player(level, in);

  BulletUpdateContext bctx = {.terminal_size = gs->terminal_size,
                               .pool = level->bullet_pool};
  object_pool_itr(level->bullet_pool, update_bullet_itr_wrapper, &bctx);

  Tutorial* t = &level->tutorial;
  t->step_frames++;
  switch (t->step) {
    case 0: if (in->ax != 0) { t->step = 1; t->step_frames = 0; } break;
    case 1: if (in->fire)    { t->step = 2; t->step_frames = 0; } break;
    case 2: if (t->step_frames > FPS * 2) gs->mode = MENU; break;
    default: gs->mode = MENU; break;
  }
}

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

  update_enemies_formation(level_state);

  BulletUpdateContext bulletUpdateContext = {
      .terminal_size = gs->terminal_size,
      .pool = level_state->bullet_pool,
  };
  object_pool_itr(level_state->bullet_pool, update_bullet_itr_wrapper,
                  &bulletUpdateContext);

  update_collision(level_state);
  update_explosions(level_state);

  Player* p = level_state->player;
  if (p->health <= 0) {
    p->lives--;
    if (p->lives <= 0) {
      gs->mode = GAME_END;
    } else {
      p->health = 1;
      p->respawn_frames = FPS * 2;
    }
  }
  if (p->respawn_frames > 0) p->respawn_frames--;
}
