#ifndef GAME_UPDATE
#define GAME_UPDATE

#include "./game_state.h"
#include "./input.h"

void game_update(GameState* g);

void frame_begin(GameState* gs);
void frame_sleep(GameState* gs);

#endif  // !GAME_UPDATE
// Reminder: Terminal does NOT track simultaneous key state
