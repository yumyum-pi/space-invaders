#include "./input.h"
#include <unistd.h>

Input input_poll() {
  char c = 'i';
  Input i = {
      .is_input = false,
      .quit = false,
      .ax = 0,
      .ay = 0,
  };
  while (read(STDIN_FILENO, &c, 1) == 1) {
    i.is_input = true;
    switch (c) {
    case 'd':
      i.ax += 1;
      break;
    case 'a':
      i.ax -= 1;
      break;
    case 'w':
      i.ay += 1;
      break;
    case 's':
      i.ay -= 1;
      break;
    case 'q':
      i.quit = true;
      break;
    }
  }

  return i;
}
