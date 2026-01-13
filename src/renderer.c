#include "./renderer.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/object_pool/object_pool.h"
#include "./terminal.h"
#include "./utils/math.h"
#include "assert.h"
#include "bullet.h"
#include "game_state.h"
#include "input.h"
#include "player.h"

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];

// TODO: not sure if i need to call endable raw mode from there
renderer renderer_init() {
  t_enableRawMode();
  Vec2i terminal_size = t_get_terminal_size();

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
int position_to_index(renderer* r, Vec2i v) {
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
void render_player(renderer* r, Vec2i p) {
  int index = position_to_index(r, p);
  r->buffer[index] = 'A';
}

void render_enemy(renderer* r, Vec2i p) {
  int index = position_to_index(r, p);
  r->buffer[index] = 'V';
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

void render_ui_elm(renderer* r, Vec2i ancher, Vec2i offset, const char* buffer,
                   int buffer_size) {

  Vec2i o = zero_vec();
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

void render_input(renderer* r, Input i) {
  int l = snprintf(buffer, BUFFER_SIZE,
                   "| input  x:%d  y:%d  fire:%d is_input:%d  |", i.ax, i.ay,
                   i.fire, i.is_input);
  assert(l != 0);
  render_ui_elm(r, (Vec2i){.x = 1, .y = -1}, (Vec2i){.x = 1, .y = 1}, buffer,
                l);
};

void render_player_pos(renderer* r, Player* p) {
  int l = snprintf(buffer, 64, "| X:%lu * Y:%lu | dx:%d dy:%d |",
                   (unsigned long)p->position.x, (unsigned long)p->position.y,
                   p->velocity.x, p->velocity.y);
  assert(l != 0);

  render_ui_elm(r, (Vec2i){.x = -1, .y = -1}, (Vec2i){.x = 1, .y = 1}, buffer,
                l);
}
void render_window_size(renderer* r) {
  int l = snprintf(buffer, BUFFER_SIZE, "| width:%lu * height:%lu |",
                   (unsigned long)r->terminal_size.x,
                   (unsigned long)r->terminal_size.y);
  assert(l != 0);
  assert(l != 0);

  render_ui_elm(r, (Vec2i){.x = 1, .y = 1}, (Vec2i){.x = 0, .y = 0}, buffer, l);
}

void render_ui(renderer* r, GameState* gs) {
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

  // enter title on top left
  render_char_at_offset(r, gs->title, 2);
  render_window_size(r);

  // show player position
  render_player_pos(r, &(gs->player));

  // show inputs on screen
  render_input(r, gs->input);
}

void render_bullet(void* payload, void* args) {
  renderer* r = (renderer*)args;
  bullet* b = (bullet*)payload;
  // get position
  int index = position_to_index(r, b->position);
  r->buffer[index] = '|';
}

void render(renderer* r, GameState* gs) {
  renderer_clear(r, ' ');
  render_player(r, gs->player.position);
  if (gs->enemy.is_active) {
    render_enemy(r, gs->enemy.position);
  }
  render_ui(r, gs);
  object_pool_itr(gs->bullet_pool, render_bullet, r);

  t_print_frame(r->buffer, r->buffer_size);
}
