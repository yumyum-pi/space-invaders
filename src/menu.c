#include "./menu.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void MenuFree(Menu* menu) {
  if (menu == NULL) {
    return;
  }
  if (menu->names != NULL) {
    free(menu->names);
  }
  if (menu->functions != NULL) {
    free(menu->functions);
  }
  free(menu);
}

Menu* NewMenu(size_t capacity) {
  Menu* menu = (Menu*)malloc(sizeof(Menu));
  if (menu == NULL) {
    return NULL;
  }
  assert(capacity > 0);
  menu->capacity = capacity;
  menu->length = 0;
  menu->hover = -1;

  // create new names array
  menu->names = (Name*)malloc(sizeof(Name) * capacity);
  if (menu->names == NULL) {
    MenuFree(menu);
    return NULL;
  }
  menu->functions = (MenuFunction**)malloc(sizeof(MenuFunction*) * capacity);
  if (menu->functions == NULL) {
    MenuFree(menu);
    return NULL;
  }
  return menu;
};

MenuFunction* MenuGetSelectedFunction(Menu* menu) {
  if (menu == NULL) {
    return NULL;
  }
  int index = menu->hover;
  if (index < 0 || index >= menu->capacity)
    return NULL;
  if (menu->functions == NULL)
    return NULL;

  return menu->functions[index];
};

void MenuSetFunction(Menu* menu, char* name, MenuFunction* function) {
  if (menu == NULL) {
    return;
  }
  // check if capacity is max
  if (menu->length >= menu->capacity) {
    return;
  }
  assert(name != NULL);
  assert(function != NULL);

  // Use strncpy for safer string copying
  strncpy(menu->names[menu->length], name, MenuNameBufferSize - 1);
  menu->names[menu->length][MenuNameBufferSize - 1] = '\0';

  menu->functions[menu->length] = function;
  menu->length++;
};

void MenuUpdateInput(Menu* menu, MenuInput* mi) {
  if (menu == NULL && mi == NULL) {
    return;
  }
  if (menu->length == 0)
    return;

  menu->hover += mi->ay;

  if (menu->hover >= menu->length) {
    menu->hover = 0;
  } else if (menu->hover < 0) {
    menu->hover = menu->length - 1;
  }

  if (mi->enter) {
    MenuFunction* func = MenuGetSelectedFunction(menu);
    assert(func != NULL);
    func();
  }
};
