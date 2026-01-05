#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static void enter_alt_screen(void) {
  /*
   * \x1b[?1049h : Enter alternate screen buffer
   * \x1b[?25l   : Hide the cursor
   */
  write(STDOUT_FILENO, "\x1b[?1049h\x1b[?25l", 14);
}
static void leave_alt_screen(void) {
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

//  24 fps
const int FRAME_RATE = 41666;

// TODO: need to learn how to use perror and exit
// TODO: need to know why i am gettig undefined error: 0;
void die(const char *s) {
  leave_alt_screen();
  // print the error and exit
  perror(s);
  exit(1);
}

// restore the default terminal
void disableRawMode() {
  leave_alt_screen();
  // 1. STDIN_FILENO → read the standard input stream(in this case 0 -
  // keyboard)
  // 2. TCSAFLUSH → apply changes after flushing input. (discarding) all
  //    data that has been written to the terminal but not yet transmitted, and
  //    all data that has been received from the terminal but not yet read.
  // 3. &orig_termios → restore saved settings
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_termios) == -1) {
    die("tcsetattr");
  }
}

void enableRawMode() {
  // get the current terminal attributes and save it
  if (tcgetattr(STDIN_FILENO, &og_termios) == -1) {
    // print error if unable to get get attr
    die("tcgetattr");
  }

  // register disableRawMode to be called when the program exits;
  atexit(disableRawMode);

  // copy the current setting
  struct termios raw = og_termios;

  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);

  // Set minimum number of characters for read() to return (VMIN) and a timeout
  // (VTIME)
  raw.c_cc[VMIN] = 0; // Read returns as soon as there is any input
  raw.c_cc[VTIME] = 0;

  // Apply the new terminal attributes
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
  enter_alt_screen();
}

typedef struct {
  size_t width;
  size_t height;
  size_t stride;
  size_t buffer_size;
  char *buffer;
} renderer;

typedef struct {
  size_t x;
  size_t y;
} Vec2;

typedef struct {
  Vec2 position;
  char display_char;
} Player;

renderer render_init(int width, int height) {
  assert(width != 0);
  assert(height != 0);

  // add one char extra per line for new line
  // size_t stride = width + 1;
  size_t stride = width;
  size_t buffer_size = stride * height;
  assert(buffer_size != 0);

  char *buffer = (char *)malloc(sizeof(char) * (1 + buffer_size));
  assert(buffer != NULL);

  renderer r = {
      .width = width,
      .height = height,
      .stride = stride,
      .buffer_size = buffer_size,
      .buffer = buffer,
  };
  return r;
}
void render_clear(renderer *r, char bg) {
  for (size_t y = 0; y < r->height; y++) {
    for (size_t x = 0; x < r->width; x++) {
      r->buffer[y * r->stride + x] = bg;
    }
    // r->buffer[y * r->stride + r->width] = '\n';
  }
  r->buffer[r->buffer_size] = '\0';
}
size_t position_to_index(renderer *r, Vec2 v) {
  assert(v.x >= 0);
  assert(v.x < r->width);
  assert(v.y >= 0);
  assert(v.y < r->height);

  size_t index = v.y * (r->width);
  index += v.x;
  assert(index < r->buffer_size);

  return index;
}
//
void render_player(renderer *r, Player *p) {
  size_t index = position_to_index(r, p->position);
  r->buffer[index] = p->display_char;
}

Vec2 get_window_size() {
  struct winsize size;

  Vec2 win_size_vec2 = {
      .x = 0,
      .y = 0,
  };

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) >= 0) {
    win_size_vec2 = (Vec2){
        .x = size.ws_col,
        .y = size.ws_row,
    };
  }

  assert(win_size_vec2.x != 0);
  assert(win_size_vec2.y != 0);
  return win_size_vec2;
}

void render(renderer *r) {
  // write(STDOUT_FILENO, "\x1b[H\x1b[2J", 7); // clear screen
  // Move cursor to home (1,1) without clearing the whole screen
  write(STDOUT_FILENO, "\x1b[H", 3);
  write(STDOUT_FILENO, r->buffer, r->buffer_size);
}

void handle_input(char c, Player *p) {
  switch (c) {
  case 'd':
    p->position.x += 1;
    break;
  case 'a':
    p->position.x -= 1;
    break;
  case 'w':
    p->position.y -= 1;
    break;
  case 's':
    p->position.y += 1;
    break;
  }
};

// need to simulate a ball falling on the ground
int main() {
  enableRawMode();
  char c = 'a';
  // clearScreen();

  Vec2 win_size_vec2 = get_window_size();
  renderer r = render_init(win_size_vec2.x, win_size_vec2.y);
  //
  Player p = {
      .position =
          {
              .x = 0,
              .y = 1,
          },
      .display_char = 'V',
  };
  //
  while (c != 'q') {

    while (read(STDIN_FILENO, &c, 1) == 1) {
      handle_input(c, &p);
    }

    render_clear(&r, ' ');
    render_player(&r, &p);
    render(&r);

    usleep(FRAME_RATE);
  }
  return 0;
}
