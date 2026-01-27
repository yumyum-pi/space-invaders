#ifndef MENU_H
#define MENU_H
#include <stdlib.h>
#include "input.h"

#define MenuNameBufferSize 64

typedef void(MenuFunction)(void);
typedef char Name[MenuNameBufferSize];

typedef struct {
  size_t length;
  size_t capacity;
  int hover;
  Name* names;               // Array of fixed-size name buffers
  MenuFunction** functions;  // Array of function pointers
} Menu;

Menu* NewMenu(size_t capacity);
MenuFunction* MenuGetSelectedFunction(Menu* menu);
void MenuSetFunction(Menu* menu, char* name, MenuFunction* function);
void MenuFree(Menu* menu);

#endif  // !MENU_H
