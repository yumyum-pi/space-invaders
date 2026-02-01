#include "./renderer.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/object_pool/object_pool.h"
#include "./terminal.h"
#include "assert.h"
#include "bullet.h"
#include "enemy.h"
#include "game_level_state.h"
#include "input.h"
#include "menu.h"
#include "player.h"

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
#define PlayerBufferSize 7
const int PlayerBufferSizeHalf = PlayerBufferSize / 2;
#define EnemyBufferSize 7
const int EnemyBufferSizeHalf = EnemyBufferSize / 2;
const char player[PlayerBufferSize] = "/=|^|=\\";
const char enemy[EnemyBufferSize] = "\\\\-v-//";

// TODO: not sure if i need to call endable raw mode from there
renderer renderer_init() {
  t_enableRawMode();
  IVec2 terminal_size = t_get_terminal_size();

  assert(terminal_size.x != 0);
  assert(terminal_size.y != 0);

  // add one char extra per line for new line
  // int stride = width + 1;
  int stride = terminal_size.x;
  int buffer_size = stride * terminal_size.y;
  assert(buffer_size != 0);

  char* buffer = (char*)malloc(sizeof(char) * (1 + buffer_size));
  assert(buffer != NULL);

  renderer r = {
      .terminal_size = terminal_size,
      .stride = stride,
      .buffer_size = buffer_size,
      .buffer = buffer,
  };
  return r;
}

void renderer_clear(renderer* r, char bg) {
  for (int y = 0; y < r->terminal_size.y; y++) {
    for (int x = 0; x < r->terminal_size.x; x++) {
      r->buffer[y * r->stride + x] = bg;
    }
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
//
void render_player(renderer* r, IVec2 p) {
  int offset = position_to_index(r, p);
  assert(offset + PlayerBufferSizeHalf < r->buffer_size);
  assert(offset - PlayerBufferSizeHalf > 0);
  offset -= PlayerBufferSizeHalf;
  for (int i = 0; i < PlayerBufferSize; i++) {
    // Simple bounds check to prevent crashing if player is off-screen
    r->buffer[offset + i] = player[i];
  }
}

void render_enemy(renderer* r, IVec2 p) {
  int offset = position_to_index(r, p);
  assert(offset + EnemyBufferSizeHalf < r->buffer_size);
  assert(offset - EnemyBufferSizeHalf > 0);
  offset -= EnemyBufferSizeHalf;
  for (int i = 0; i < EnemyBufferSize; i++) {
    // Simple bounds check to prevent crashing if player is off-screen
    r->buffer[offset + i] = enemy[i];
  }
}
//

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
  render_border(r);

  // enter title on top left
  render_char_at_offset(r, gs->title, 2);
  render_window_size(r);

  // show player position
  render_player_pos(r, &(gs->player));

  // show inputs on screen
  render_input(r, gs->input);

  // show gun
  render_gun(r, &(gs->player.gun));
}

bool render_bullet(void* payload, void* args) {
  renderer* r = (renderer*)args;
  bullet* b = (bullet*)payload;
  // get position
  int index = position_to_index(r, b->position);
  r->buffer[index] = '|';
  return true;
}

bool render_enemy_pool(void* payload, void* args) {
  renderer* r = (renderer*)args;
  Enemy* e = (Enemy*)payload;
  render_enemy(r, e->position);
  return true;
}

void render_level(renderer* r, GameLevelState* gs) {
  renderer_clear(r, ' ');
  render_player(r, gs->player.position);

  object_pool_itr(gs->enemy_pool, render_enemy_pool, r);
  object_pool_itr(gs->bullet_pool, render_bullet, r);
  render_ui(r, gs);

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
  render_border(r);
  render_menu(r, menu);
  t_print_frame(r->buffer, r->buffer_size);
}
