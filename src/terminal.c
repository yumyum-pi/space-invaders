#include "./terminal.h"
#include <assert.h>
#include <libc.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

IVec2 t_get_terminal_size() {
  struct winsize size;

  IVec2 win_size_vec2 = {
      .x = 0,
      .y = 0,
  };

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) >= 0) {
    win_size_vec2 = (IVec2){
        .x = size.ws_col,
        .y = size.ws_row,
    };
  }

  assert(win_size_vec2.x != 0);
  assert(win_size_vec2.y != 0);
  return win_size_vec2;
}
// TODO: what is the use of static void
void t_enter_alt_screen(void) {
  /*
   * \x1b[?1049h : Enter alternate screen buffer
   * \x1b[?25l   : Hide the cursor
   */
  write(STDOUT_FILENO, "\x1b[?1049h\x1b[?25l", 14);
}

void t_leave_alt_screen(void) {
  /*
   * \x1b[?1049l : Leave alternate screen
   * \x1b[2J : Clear screen
   * \x1b[1;1H : Move cursor to top-left (home)
   * \x1b[?25h : Show cursor
   */
  write(STDOUT_FILENO, "\x1b[?1049l\x1b[2J\x1b[1;1H\x1b[?25h", 25);
}

// Global variable to store original settings
struct termios og_termios;

void t_disableRawMode() {
  t_leave_alt_screen();
  // 1. STDIN_FILENO → read the standard input stream(in this case 0 -
  // keyboard)
  // 2. TCSAFLUSH → apply changes after flushing input. (discarding) all
  //    data that has been written to the terminal but not yet transmitted, and
  //    all data that has been received from the terminal but not yet read.
  // 3. &orig_termios → restore saved settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_termios);
}

void t_enableRawMode() {
  // get the current terminal attributes and save it
  tcgetattr(STDIN_FILENO, &og_termios);

  // register disableRawMode to be called when the program exits;
  atexit(t_disableRawMode);

  // copy the current setting
  struct termios raw = og_termios;

  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);

  // Set minimum number of characters for read() to return (VMIN) and a timeout
  // (VTIME)
  raw.c_cc[VMIN] = 0;  // Read returns as soon as there is any input
  raw.c_cc[VTIME] = 0;

  // Apply the new terminal attributes
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  t_enter_alt_screen();
}

void t_print_frame(char* buffer, int buffer_size) {
  // write(STDOUT_FILENO, "\x1b[H\x1b[2J", 7); // clear screen
  // Move cursor to home (1,1) without clearing the whole screen
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, buffer, buffer_size);
}
