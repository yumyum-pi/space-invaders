#include "./game_state.h"
#include "./renderer.h"
#include "game_update.h"
#include "input.h"
#include "utils/math.h"
#include <unistd.h>

int main() {
  renderer r = renderer_init();
  GameState gs = game_state_init(r.terminal_size, "This is title");

  // TODO: create enemies
  // - Add them in the world
  // - make them move over time
  // TODO: need to properly handle non blocking inputs
  input_init();
  gs.enemy.position = (Vec2i){r.terminal_size.x / 2, 4};

  while (gs.is_running) {
    frame_begin(&gs);
    // get the inputs
    Input in = input_poll();
    game_update(&gs, &in, 2);

    render(&r, &gs);
    frame_sleep(&gs);
  }
  return 0;
}
