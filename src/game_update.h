#ifndef GAME_UPDATE
#define GAME_UPDATE

#include "./game_state.h"
#include "./input.h"

void game_update(GameState *g, const Input *in, int dt);

void frame_begin(GameState *gs);
void frame_sleep(GameState *gs);

#endif // !GAME_UPDATE
