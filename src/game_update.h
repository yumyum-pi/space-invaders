#ifndef GAME_UPDATE
#define GAME_UPDATE

#include "./game_level_state.h"
#include "./input.h"
#include "utils/math.h"

void game_update(GameLevelState* g);
void game_update_init(Vec2i terminal_size);

void frame_begin(GameLevelState* gs);
void frame_sleep(GameLevelState* gs);

#endif  // !GAME_UPDATE
// Reminder: Terminal does NOT track simultaneous key state
