#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "./game_level_state.h"
#include "./game_state.h"
#include "./game_update.h"
#include "./input.h"
#include "./renderer.h"
#include "menu.h"

int main() {
  renderer r = renderer_init();
  GameState* game_state = InitGameState(r.terminal_size);
  game_update_init(game_state->terminal_size);

  MenuInput menu_input;
  GameLevelState* level = NULL;
  while (game_state->is_running) {
    frame_begin(game_state);
    switch (game_state->mode) {
      case MAIN_MENU:
        // capture input
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->MainMenu, &menu_input);
        // render the menu
        render_main_menu(&r, game_state->MainMenu);
        break;
      case GAME_PAUSE:
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->PauseMenu, &menu_input);
        // render the menu
        render_main_menu(&r, game_state->PauseMenu);
        break;
      case GAME_PLAY:
        level = game_state->LevelState;
        assert(level != NULL);
        // get the inputs
        level->input = game_input_poll();
        level->frame_count += 1;
        game_update(game_state, level);
        render_level(&r, level);
        break;
      case GAME_END: break;
      default:
        // this should never happen break the game
        printf("unknown game state: %d\n", game_state->mode);
        return 2;
    }
    frame_sleep(game_state);
  }
  return 0;
}
