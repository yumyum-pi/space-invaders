#include "./renderer.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/object_pool/object_pool.h"
#include "./terminal.h"
#include "assert.h"
#include "bullet.h"
#include "enemy.h"
#include "game_level_state.h"
#include "input.h"
#include "menu.h"
#include "player.h"
#include "utils/math/irect.h"
#include "utils/math/ivec2.h"

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];

// TODO: not sure if i need to call endable raw mode from there
renderer renderer_init() {
  t_enableRawMode();
  IVec2 terminal_size = t_get_terminal_size();

  assert(terminal_size.x != 0);
  assert(terminal_size.y != 0);
  assert(terminal_size.y * 2 <= terminal_size.x);

  IVec2 gameSize = {terminal_size.y * 2, terminal_size.y};

  int offset_x = (terminal_size.x - gameSize.x) / 2;

  // add one char extra per line for new line
  // int stride = width + 1;
  int stride = terminal_size.x;
  int buffer_size = stride * terminal_size.y;
  assert(buffer_size != 0);

  char* buffer = (char*)malloc(sizeof(char) * (1 + buffer_size));
  assert(buffer != NULL);

  renderer r = {
      .terminal_size = terminal_size,
      .gameRect = IRectNew((IVec2){offset_x, 0},
                           (IVec2){offset_x + gameSize.x, gameSize.y}),
      .offset_x = offset_x,
      .stride = stride,
      .buffer_size = buffer_size,
      .buffer = buffer,
  };
  return r;
}

void renderer_clear(renderer* r, char bg) {
  char wall = '.';
  memset(r->buffer, bg, r->buffer_size);
  int offset = 0;
  for (int y = 0; y < r->terminal_size.y; y++) {
    offset = y * r->stride;
    memset(&(r->buffer[offset]), wall, r->offset_x);
    memset(&(r->buffer[offset + r->offset_x + (r->terminal_size.y * 2)]), wall,
           r->offset_x);
  }
  r->buffer[r->buffer_size] = '\0';
}
int position_to_index(renderer* r, IVec2 v) {
  assert(v.x >= 0);
  assert(v.x < r->terminal_size.x);
  assert(v.y >= 0);
  assert(v.y < r->terminal_size.y);

  int index = v.y * (r->terminal_size.x);
  index += v.x;
  assert(index < r->buffer_size);

  return index;
}

int worldPosToIndex(renderer* r, IVec2 pos) {

  pos.x += r->offset_x;
  assert(IRectColideIVec2(r->gameRect, pos));

  int index = pos.y * (r->terminal_size.x);
  index += pos.x;
  assert(index < r->buffer_size);

  return index;
}

void renderSprite(renderer* r, Sprite* sprite, IVec2 position) {
  // get min max from a point and size
  IRect rec = IRectFromCenter(position, (IVec2){sprite->width, sprite->height});

  // bounds check for min and max points
  int min_offset = worldPosToIndex(r, rec.min);
  worldPosToIndex(r, rec.max);

  size_t offset = min_offset;
  char c = '0';
  //loop through
  for (size_t i = 0; i < sprite->length; i++) {
    c = sprite->buffer[i];
    if (c == '\n') {
      offset += (size_t)(r->terminal_size.x) - sprite->width;
      continue;
    }
    if (c == '\0') {
      break;
    }
    r->buffer[offset] = c;
    offset++;
  }
}

//
void render_player(renderer* r, Player* p) {
  renderSprite(r, p->sprite, p->position);
}

// TODO: move to a different location
int c_str_len(const char* c) {
  int len = 0;
  while (c[len] != '\0') {
    len++;
  }
  return len;
}

void render_char_at_offset(renderer* r, const char* c, int offset) {
  int len = c_str_len(c);
  assert(len != 0);
  assert(offset < r->buffer_size);
  assert(offset + len <= r->buffer_size);
  for (int i = 0; i < len; i++) {
    r->buffer[i + offset] = c[i];
  }
}

int calc_offset_top_right(renderer* r, size_t l) {
  return r->terminal_size.x - l;
}
int calc_offset_bottom_right(renderer* r, size_t l) {
  return (r->buffer_size - l);
}

void render_ui_elm(renderer* r, IVec2 ancher, IVec2 offset, const char* buffer,
                   int buffer_size) {

  IVec2 o = IVec2Zero();
  switch (ancher.y) {
    case 0:
      // make the char middle algin
      o.y = r->terminal_size.y / 2 + offset.y;
      break;
    case 1:
      // make the char top align
      o.y = 0 + offset.y;
      break;
    case -1:
      // make the char bottom align
      o.y = r->terminal_size.y - offset.y - 1;
      break;
    default: assert(false && "unknown input for ancher y"); break;
  }
  assert(o.y >= 0);
  assert(o.y <= r->terminal_size.y);

  switch (ancher.x) {
    case 0:
      // make the char center algin
      o.x = (r->terminal_size.x - buffer_size) / 2 + offset.x;
      break;
    case 1:
      // make the char right align
      o.x = r->terminal_size.x - buffer_size - offset.x;
      break;
    case -1:
      // make the char top align
      o.x = 0 + offset.x;
      break;
    default: assert(false && "unknown input for ancher x"); break;
  }
  assert(o.x >= 0);
  assert(o.x <= r->terminal_size.x);

  int index = o.y * r->terminal_size.x + o.x;
  assert(index >= 0);
  assert(index < r->buffer_size);
  render_char_at_offset(r, buffer, index);
}

void render_input(renderer* r, GameInput i) {
  int l = snprintf(buffer, BUFFER_SIZE,
                   "| input  x:%d  y:%d  fire:%d is_input:%d  |", i.ax, i.ay,
                   i.fire, i.is_input);
  assert(l != 0);
  render_ui_elm(r, (IVec2){.x = 1, .y = -1}, (IVec2){.x = 1, .y = 1}, buffer,
                l);
};

void render_player_pos(renderer* r, Player* p) {
  int l = snprintf(buffer, 64, "| X:%lu * Y:%lu | dx:%d dy:%d |",
                   (unsigned long)p->position.x, (unsigned long)p->position.y,
                   p->velocity.x, p->velocity.y);
  assert(l != 0);

  render_ui_elm(r, (IVec2){.x = -1, .y = -1}, (IVec2){.x = 1, .y = 1}, buffer,
                l);
}
void render_window_size(renderer* r) {
  int l = snprintf(buffer, BUFFER_SIZE, "| width:%lu * height:%lu |",
                   (unsigned long)r->terminal_size.x,
                   (unsigned long)r->terminal_size.y);
  assert(l != 0);

  render_ui_elm(r, (IVec2){.x = 1, .y = 1}, (IVec2){.x = 0, .y = 0}, buffer, l);
}

void render_gun(renderer* r, Gun* g) {
  int l = 0;
  if (g->is_reloading) {

    l = snprintf(buffer, BUFFER_SIZE, "| Reloading |");
  } else {

    l = snprintf(buffer, BUFFER_SIZE, "| Magzine:%d |", g->remaining_rounds);
  }
  assert(l != 0);
  render_ui_elm(r, (IVec2){.x = -1, .y = -1}, (IVec2){.x = 1, .y = 2}, buffer,
                l);
};
void render_border(renderer* r) {
  char border_char = '+';
  // render borders
  int w = 0;
  int l_line = r->buffer_size - r->terminal_size.x;
  for (int i = 0; i < r->buffer_size; i++) {
    w = i % r->terminal_size.x;
    if (w == 0 || w == (r->terminal_size.x - 1) || i < r->terminal_size.x ||
        i > l_line) {
      r->buffer[i] = border_char;
    }
  }
}

void render_ui(renderer* r, GameLevelState* gs) {
  // render_border(r);

  // enter title on top left
  render_char_at_offset(r, gs->title, 2);
  render_window_size(r);

  // show player position
  render_player_pos(r, gs->player);

  // show inputs on screen
  render_input(r, gs->input);

  // show gun
  render_gun(r, &(gs->player->gun));
}

bool render_bullet(void* payload, void* args) {
  renderer* r = (renderer*)args;
  bullet* b = (bullet*)payload;
  // get position
  int index = worldPosToIndex(r, b->position);
  r->buffer[index] = '|';
  return true;
}

bool render_enemy_pool(void* payload, void* args) {
  renderer* r = (renderer*)args;
  Enemy* e = (Enemy*)payload;
  Sprite s = EnemyGetSprite(e->type);
  renderSprite(r, &s, e->position);
  return true;
}

static void render_explosions(renderer* r, GameLevelState* gs) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    Explosion* ex = &gs->explosions[i];
    if (!ex->is_active) continue;
    IVec2 pos = {ex->position.x - 2, ex->position.y};
    render_char_at_offset(r, "*-X-*", worldPosToIndex(r, pos));
  }
}

static void render_tutorial_overlay(renderer* r, GameLevelState* gs) {
  static const char* steps[] = {
      "Move left/right -- press A or D",
      "Shoot -- press SPACE",
      "You're ready!  Returning to menu...",
  };
  int step = gs->tutorial.step;
  if (step > 2) return;
  const char* msg = steps[step];
  render_ui_elm(r, (IVec2){0, 0}, (IVec2){0, 0}, msg, c_str_len(msg));
}

static void render_shields(renderer* r, GameLevelState* gs) {
  static const char* sprites[] = {"###", "-#-", "..."};
  for (int i = 0; i < gs->shield_count; i++) {
    Shield* s = &gs->shields[i];
    if (!s->is_active || s->hp <= 0) continue;
    IVec2 pos = {s->position.x - 1, s->position.y};
    render_char_at_offset(r, sprites[s->hp - 1], worldPosToIndex(r, pos));
  }
}

static void render_hud(renderer* r, GameLevelState* gs) {
  char buf[32];
  int l;
  l = snprintf(buf, sizeof(buf), "SCORE: %d", gs->score);
  render_ui_elm(r, (IVec2){0, 1}, (IVec2){0, 0}, buf, l);
  l = snprintf(buf, sizeof(buf), "LIVES: %d", gs->player->lives);
  render_ui_elm(r, (IVec2){-1, 1}, (IVec2){2, 0}, buf, l);
}

void render_level(renderer* r, GameLevelState* gs) {
  renderer_clear(r, ' ');
  render_player(r, gs->player);
  render_shields(r, gs);
  render_explosions(r, gs);
  object_pool_itr(gs->enemy_pool, render_enemy_pool, r);
  object_pool_itr(gs->bullet_pool, render_bullet, r);
  render_hud(r, gs);
  render_ui(r, gs);
  if (gs->tutorial.active) render_tutorial_overlay(r, gs);

  t_print_frame(r->buffer, r->buffer_size);
}

void render_menu_input(renderer* r, MenuInput* mi) {
  MenuInput i = *mi;
  int l = snprintf(buffer, BUFFER_SIZE,
                   "| input  x:%d  y:%d  enter:%d is_input:%d  |", i.ax, i.ay,
                   i.enter, i.is_input);
  assert(l != 0);
  render_ui_elm(r, (IVec2){.x = 1, .y = -1}, (IVec2){.x = 1, .y = 1}, buffer,
                l);
}
void render_menu(renderer* r, Menu* menu) {
  MenuNameBufferSize;
  for (size_t i = 0; i < menu->length; i++) {
    int l = 0;
    if (i == menu->hover) {

      l = snprintf(buffer, BUFFER_SIZE, "> %s <", (char*)menu->names[i]);
    } else {

      l = snprintf(buffer, BUFFER_SIZE, "  %s  ", (char*)menu->names[i]);
    }
    render_ui_elm(r, (IVec2){.x = 0, .y = 0}, (IVec2){.x = 0, .y = i}, buffer,
                  l);
  }
}

void render_main_menu(renderer* r, Menu* menu) {
  renderer_clear(r, ' ');
  // render_menu_input(r, mi);
  // render_border(r);
  render_menu(r, menu);
  t_print_frame(r->buffer, r->buffer_size);
}

void render_menu_with_score(renderer* r, Menu* menu, int score) {
  renderer_clear(r, ' ');
  render_menu(r, menu);
  char buf[32];
  int l = snprintf(buf, sizeof(buf), "SCORE: %d", score);
  render_ui_elm(r, (IVec2){0, 1}, (IVec2){0, 2}, buf, l);
  t_print_frame(r->buffer, r->buffer_size);
}
