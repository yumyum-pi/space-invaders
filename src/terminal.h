#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
#include "utils/math.h"

/**
 * Terminal System Components
 * Responsibility: Handle raw mode, screen buffering, and window dimensions.
 */

// Global access to original terminal settings for restoration if needed
extern struct termios og_termios;

// Window Management
Vec2i t_get_terminal_size(void);

// Screen Buffer Management
void t_enter_alt_screen(void);
void t_leave_alt_screen(void);

// Mode Management
void t_enableRawMode(void);
void t_disableRawMode(void);

// Output
void t_print_frame(char* buffer, int buffer_size);

#endif  // TEMRNAL_H
