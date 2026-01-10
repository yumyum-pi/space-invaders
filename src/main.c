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

  // TODO: make mulitple bullets
  // - player ship wil have a fire rate: 24 frames per seconds - 1 bullet per
  // second
  // - player should have 10 rounds of bullets
  // - after 10 rounds the player will relaod which will take 4 seconds
  while (gs.is_running) {
    frame_begin(&gs);
    // get the inputs
    Input in = input_poll();
    game_update(&gs, &in, 2);
    render(&r, &gs);
    gs.frame_count += 1;
    frame_sleep(&gs);
  }
  return 0;
}
