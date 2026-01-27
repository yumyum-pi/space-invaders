#include <stdio.h>
#include <unistd.h>
#include "./game_level_state.h"
#include "./game_state.h"
#include "./game_update.h"
#include "./input.h"
#include "./renderer.h"

int main() {
  renderer r = renderer_init();
  GameState game_state = NewGameState(r.terminal_size);
  GameLevelState level = game_level_init(r.terminal_size, "Space Invaders");
  game_update_init(game_state.terminal_size);

  MenuInput menu_input;
  while (game_state.is_running) {
    frame_begin(&game_state);
    switch (game_state.mode) {
      case MAIN_MENU:
        // capture input
        menu_input = menu_input_poll();

        if (menu_input.quit) {
          game_state.is_running = 0;
          break;
        }
        // return inputs
        render_main_menu(&r, &menu_input);
        break;
      case GAME_PAUSE: break;
      case GAME_PLAY:
        // get the inputs
        level.input = game_input_poll();
        game_update(&game_state, &level);
        render_level(&r, &level);
        level.frame_count += 1;
        break;
      case GAME_END: break;
      default:
        // this should never happen break the game
        printf("unknown game state: %d\n", game_state.mode);
        return 2;
    }
    frame_sleep(&game_state);
  }
  return 0;
}
