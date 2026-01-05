#include "./game_state.h"
#include "./renderer.h"
#include "game_update.h"
#include "input.h"
#include "utils/math.h"
#include <unistd.h>

int main() {
  renderer r = renderer_init();
  Vec2i terminal_size = (Vec2i){
      .x = r.width,
      .y = r.height,
  };
  GameState gs = game_state_init(terminal_size, "This is title");

  while (gs.is_running) {
    // get the inputs
    Input in = input_poll();
    game_update(&gs, &in, 2);

    render(&r, &gs);
    //
    // TODO: Better "Frame Timing"
    usleep(41666);
  }
  return 0;
}
