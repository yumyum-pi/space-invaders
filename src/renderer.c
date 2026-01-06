#include "./renderer.h"
#include "./terminal.h"
#include "./utils/math.h"
#include "assert.h"
#include "game_state.h"
#include <stdio.h>
#include <stdlib.h>

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

  char *buffer = (char *)malloc(sizeof(char) * (1 + buffer_size));
  assert(buffer != NULL);

  renderer r = {
      .terminal_size = terminal_size,
      .stride = stride,
      .buffer_size = buffer_size,
      .buffer = buffer,
  };
  return r;
}
void renderer_clear(renderer *r, char bg) {
  for (int y = 0; y < r->terminal_size.y; y++) {
    for (int x = 0; x < r->terminal_size.x; x++) {
      r->buffer[y * r->stride + x] = bg;
    }
  }
  r->buffer[r->buffer_size] = '\0';
}
int position_to_index(renderer *r, Vec2i v) {
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
void render_player(renderer *r, Vec2i p) {
  int index = position_to_index(r, p);
  r->buffer[index] = 'A';
}

void render_enemy(renderer *r, Vec2i p) {
  int index = position_to_index(r, p);
  r->buffer[index] = 'V';
}
//

// TODO: move to a different location
int c_str_len(const char *c) {
  int len = 0;
  while (c[len] != '\0') {
    len++;
  }
  return len;
}

void render_char_at_offset(renderer *r, const char *c, int offset) {
  int len = c_str_len(c);
  assert(len != 0);
  assert(offset < r->buffer_size);
  assert(offset + len <= r->buffer_size);
  for (int i = 0; i < len; i++) {
    r->buffer[i + offset] = c[i];
  }
}

int calc_offset_top_right(renderer *r, size_t l) {
  return r->terminal_size.x - l;
}
int calc_offset_bottom_right(renderer *r, size_t l) {
  return (r->buffer_size - l);
}

void render_ui(renderer *r, GameState *gs) {
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
  {
    char size[64];
    int l = snprintf(size, 64, "| width:%lu * height:%lu |",
                     (unsigned long)r->terminal_size.x,
                     (unsigned long)r->terminal_size.y);
    assert(l != 0);

    int offset = calc_offset_top_right(r, l) - 1;
    render_char_at_offset(r, size, offset);
  }
  {

    Player *p = &gs->player;
    char position[64];
    int l = snprintf(position, 64, "| X:%lu * Y:%lu | dx:%d dy:%d |",
                     (unsigned long)p->position.x, (unsigned long)p->position.y,
                     p->velocity.x, p->velocity.y);
    assert(l != 0);

    int offset = calc_offset_bottom_right(r, l) - 1;
    render_char_at_offset(r, position, offset);
  }
}

void render(renderer *r, GameState *gs) {
  renderer_clear(r, ' ');
  render_player(r, gs->player.position);
  render_enemy(r, gs->enemy.position);
  render_ui(r, gs);

  t_print_frame(r->buffer, r->buffer_size);
}

// void update_player_x_position(int x) {}
//

// Render UI
