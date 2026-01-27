#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

typedef struct {
  int ax;
  int ay;
  bool is_input;
  bool quit;
  bool fire;
} GameInput;

typedef struct {
  int ax;
  int ay;
  bool enter;
  bool quit;
  bool is_input;
} MenuInput;

GameInput game_input_poll(void);
MenuInput menu_input_poll(void);

#endif  // !INPUT_H
