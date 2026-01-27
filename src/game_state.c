#include "./game_state.h"
#include <stdio.h>
#include "menu.h"

void NewGame() {
  printf("Need to create new level\n");
}
void QuitGame() {
  printf("Need to quit the game\n");
}
GameState NewGameState(Vec2i terminal_size) {
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
  MenuSetFunction(main_menu, "Quit", &NewGame);

  // create game menu
  return (GameState){
      .mode = MAIN_MENU,
      .MainMenu = main_menu,
      .is_running = true,
      .terminal_size = terminal_size,
      .target_frame_ms = FRAME_MS,
      .bounds_min = bounds_min,
      .bounds_max = bounds_max,
  };
};

// GameLevelState getLevel(int level);
