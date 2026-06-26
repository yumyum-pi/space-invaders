#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "./game_level_state.h"
#include "./game_state.h"
#include "./game_update.h"
#include "./input.h"
#include "./renderer.h"
#include "enemy.h"
#include "menu.h"
#include "utils/math/irect.h"

int main() {
  renderer r = renderer_init();
  GameState* game_state = InitGameState(IRectGetSize(r.gameRect));
  game_update_init(game_state->terminal_size);

  MenuInput menu_input;
  GameLevelState* level = NULL;
  EnemyInit();
  // game_state->MainMenu->functions[0]();
  while (game_state->is_running) {
    frame_begin(game_state);
    switch (game_state->mode) {
      case MENU:
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->MainMenu, &menu_input);
        render_main_menu(&r, game_state->MainMenu);
        break;
      case GAME_PAUSE:
        // TODO: should show score
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->GamePauseMenu, &menu_input);
        // render the menu
        render_main_menu(&r, game_state->GamePauseMenu);
        break;
      case GAME_PLAY:
        level = game_state->LevelState;
        assert(level != NULL);
        level->input = game_input_poll();
        level->frame_count += 1;
        game_update(game_state, level);
        render_level(&r, level);
        break;
      case GAME_END:
        // TODO: should show score
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->GameEndMenu, &menu_input);
        render_main_menu(&r, game_state->GameEndMenu);
        break;
      case TUTORIAL_MENU:
        menu_input = menu_input_poll();
        MenuUpdateInput(game_state->TutorialMenu, &menu_input);
        render_main_menu(&r, game_state->TutorialMenu);
        break;
      default:
        // this should never happen break the game
        printf("unknown game state: %d\n", game_state->mode);
        return 2;
    }
    frame_sleep(game_state);
  }
  return 0;
}
