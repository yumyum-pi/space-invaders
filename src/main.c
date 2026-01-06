#include "./game_state.h"
#include "./renderer.h"
#include "game_update.h"
#include "input.h"
#include <unistd.h>

int main() {
  renderer r = renderer_init();
  GameState gs = game_state_init(r.terminal_size, "This is title");

  // TODO: need to properly handle non blocking inputs
  input_init();
  while (gs.is_running) {
    // get the inputs
    Input in = input_poll();
    game_update(&gs, &in, 2);

    render(&r, &gs);
    // TODO: Better "Frame Timing"
    usleep(41666);
  }
  return 0;
}
