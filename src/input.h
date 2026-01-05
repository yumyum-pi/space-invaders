#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

typedef struct {
  int ax;
  int ay;
  bool is_input;
  bool quit;
} Input;

Input input_poll(void);

#endif // !INPUT_H
