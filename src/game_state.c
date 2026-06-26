#include "./game_state.h"
#include "game_level_state.h"
#include "menu.h"

static GameState gs = {
    .mode = MENU,
    .is_running = true,
    .target_frame_ms = FRAME_MS,
};

void RemoveGameLevel(GameLevelState* level_state) {
  if (level_state != NULL) {
    return;
  }

  if (level_state->enemy_pool != NULL) {
    remove_object_pool(level_state->enemy_pool);
    level_state->enemy_pool = NULL;
  }
  if (level_state->bullet_pool != NULL) {
    remove_object_pool(level_state->bullet_pool);
    level_state->bullet_pool = NULL;
  }
  if (level_state->level != NULL) {
    LevelRemove(level_state->level);
    level_state->level = NULL;
  }

  free(level_state);
}

void NewGame() {
  gs.LevelState = GameLevelNew(gs.terminal_size, "Space Invaders");
  gs.mode = GAME_PLAY;
}
void QuitGame() {
  gs.is_running = false;
}

void GameResume() {
  gs.mode = GAME_PLAY;
}
void GameMainMenu() {
  RemoveGameLevel(gs.LevelState);
  gs.MainMenu->hover = 0;
  gs.mode = MENU;
}

void Restart() {
  RemoveGameLevel(gs.LevelState);
  NewGame();
}

void TutorialMenu() {
  gs.mode = MENU;
}

typedef void (*TutorialFunc)(void);
void TutorialMove(void) { /* ... */ }
void Tutorial_1() {
  gs.mode = GAME_PLAY;
}

// void* GetTutorial(int i) {
//
//   return &Tutorial_1;
// }

GameState* InitGameState(IVec2 terminal_size) {
  IVec2 bounds_min = {
      .x = 16,
      .y = terminal_size.y - 32,
  };

  IVec2 bounds_max = {
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };

  Menu* main_menu = NewMenu(3);
  MenuSetFunction(main_menu, "New Game", &NewGame);
  MenuSetFunction(main_menu, "Tutorial", &NewGame);
  MenuSetFunction(main_menu, "Quit", &QuitGame);

  Menu* game_pause_menu = NewMenu(3);
  MenuSetFunction(game_pause_menu, "Resume", &GameResume);
  MenuSetFunction(game_pause_menu, "Main Menu", &GameMainMenu);
  MenuSetFunction(game_pause_menu, "Quit", &QuitGame);

  Menu* game_end_menu = NewMenu(3);
  MenuSetFunction(game_end_menu, "Restart", &Restart);
  MenuSetFunction(game_end_menu, "Main Menu", &GameMainMenu);
  MenuSetFunction(game_end_menu, "Quit", &QuitGame);

  Menu* turorial_menu = NewMenu(3);
  MenuSetFunction(game_end_menu, "Tutorial 1", &Restart);
  // Menu* turorial_menu = NewMenu(3);
  // Menu* end_menu = NewMenu(3);

  // create game menu
  gs = (GameState){
      .mode = MENU,
      .MainMenu = main_menu,
      .GamePauseMenu = game_pause_menu,
      .GameEndMenu = game_end_menu,
      .TutorialMenu = turorial_menu,
      .is_running = true,
      .terminal_size = terminal_size,
      .target_frame_ms = FRAME_MS,
      .bounds_min = bounds_min,
      .bounds_max = bounds_max,
  };

  return &gs;
};
