#include <unistd.h>
#include "./game_level_state.h"
#include "./renderer.h"
#include "game_update.h"
#include "input.h"

int main() {
  renderer r = renderer_init();
  GameLevelState gs = game_state_init(r.terminal_size, "This is title");
  game_update_init(gs.terminal_size);

  while (gs.is_running) {
    frame_begin(&gs);
    // get the inputs
    gs.input = input_poll();
    game_update(&gs);
    render(&r, &gs);
    gs.frame_count += 1;
    frame_sleep(&gs);
  }
  return 0;
}
