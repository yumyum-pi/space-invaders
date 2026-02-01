#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "game_level_state.h"
#include "menu.h"
#include "utils/math.h"
typedef enum {
  MAIN_MENU,
  GAME_PAUSE,
  GAME_PLAY,
  GAME_END,
} GameMode;

typedef struct {
  GameMode mode;
  Menu* MainMenu;
  Menu* PauseMenu;
  Menu* EndMenu;
  GameLevelState* LevelState;
  IVec2 terminal_size;
  IVec2 bounds_min;
  IVec2 bounds_max;
  bool is_running;
  struct timespec frame_start;
  long target_frame_ms;
} GameState;

GameState* InitGameState(IVec2 terminal_siz);
GameLevelState getLevel(GameState* gs, int level_state);

#endif  // GAME_STATE_H
