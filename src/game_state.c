#include "./game_state.h"
#include "game_level_state.h"
#include "menu.h"

static GameState gs = {
    .mode = MAIN_MENU,
    .is_running = true,
    .target_frame_ms = FRAME_MS,
};

void resetMenus() {
  gs.MainMenu->hover = 0;
  gs.PauseMenu->hover = 0;
  // gs.QuitGame->hover = 0;
}
void NewGame() {
  gs.LevelState = NewGameLevel(gs.terminal_size, "Space Invaders");
  gs.mode = GAME_PLAY;
  resetMenus();
}
void QuitGame() {
  gs.is_running = false;
  resetMenus();
}

void GameResume() {
  gs.mode = GAME_PLAY;
  resetMenus();
}
void GameMainMenu() {
  RemoveGameLevel(gs.LevelState);
  gs.mode = MAIN_MENU;
  resetMenus();
}

GameState* InitGameState(Vec2i terminal_size) {
  Vec2i bounds_min = {
      .x = 16,
      .y = terminal_size.y - 32,
  };

  Vec2i bounds_max = {
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };

  Menu* main_menu = NewMenu(3);

  MenuSetFunction(main_menu, "New Game", &NewGame);
  MenuSetFunction(main_menu, "Quit", &QuitGame);

  Menu* pause_menu = NewMenu(3);
  MenuSetFunction(pause_menu, "Resume", &GameResume);
  MenuSetFunction(pause_menu, "Main Menu", &GameMainMenu);

  // create game menu
  gs = (GameState){
      .mode = MAIN_MENU,
      .MainMenu = main_menu,
      .PauseMenu = pause_menu,
      .is_running = true,
      .terminal_size = terminal_size,
      .target_frame_ms = FRAME_MS,
      .bounds_min = bounds_min,
      .bounds_max = bounds_max,
  };

  return &gs;
};

// GameLevelState getLevel(int level);
