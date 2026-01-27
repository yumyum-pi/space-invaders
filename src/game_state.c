#include "./game_state.h"

GameState NewGameState(Vec2i terminal_size) {
  Vec2i bounds_min = {
      .x = 16,
      .y = terminal_size.y - 32,
  };

  Vec2i bounds_max = {
      .x = terminal_size.x - 16,
      .y = terminal_size.y - 8,
  };
  return (GameState){
      .mode = GAME_PLAY,
      .is_running = true,
      .terminal_size = terminal_size,
      .target_frame_ms = FRAME_MS,
      .bounds_min = bounds_min,
      .bounds_max = bounds_max,
  };
};

// GameLevelState getLevel(int level);
