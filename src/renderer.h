#ifndef RENDERER_H
#define RENDERER_H
#include "./utils/math.h"
#include "game_level_state.h"
#include "input.h"
#include "menu.h"
#include "utils/math/irect.h"

typedef struct {
  IVec2 terminal_size;
  int offset_x;
  int stride;
  IRect gameRect;
  int buffer_size;
  char* buffer;
} renderer;

renderer renderer_init();
void render_level(renderer* r, GameLevelState* ge);
void render_main_menu(renderer* r, Menu* menu);
void render_menu_with_score(renderer* r, Menu* menu, int score);
#endif  // !renderer_H
