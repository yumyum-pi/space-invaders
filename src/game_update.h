#ifndef GAME_UPDATE
#define GAME_UPDATE

#include "./game_state.h"
#include "./input.h"

void game_update(GameState *g, const Input *in, int dt);

#endif // !GAME_UPDATE
