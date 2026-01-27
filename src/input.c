#include "./input.h"
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

GameInput game_input_poll() {
  char c = 'i';
  GameInput i = {
      .is_input = false,
      .quit = false,
      .ax = 0,
      .ay = 0,
      .fire = false,
  };
  while (read(STDIN_FILENO, &c, 1) == 1) {
    i.is_input = true;
    switch (c) {
      case 'd': i.ax += 1; break;
      case 'a': i.ax -= 1; break;
      case 'w': i.ay += 1; break;
      case 's': i.ay -= 1; break;
      case 'q': i.quit = true; break;
      case ' ': i.fire = true;
    }
  }

  return i;
}

MenuInput menu_input_poll() {
  char c = 'i';
  MenuInput i = {
      .is_input = false,
      .ax = 0,
      .ay = 0,
  };
  while (read(STDIN_FILENO, &c, 1) == 1) {
    i.is_input = true;
    switch (c) {
      case 'd': i.ax += 1; break;
      case 'a': i.ax -= 1; break;
      case 'w': i.ay += 1; break;
      case 's': i.ay -= 1; break;
      case 'q': i.quit = true; break;
      case '\r': i.enter = true; break;
      case '\n': i.enter = true; break;
    }
  }

  return i;
}
