#ifndef SPRITE_H
#define SPRITE_H
#include <stddef.h>

typedef struct {
  char* buffer;
  size_t length;
  int width;  // Useful if the sprite is multi-line
  int height;
} Sprite;

Sprite* SpriteNewFromStr(const char* str);
void SpriteFree(Sprite* sprite);
#endif  // !SPRITE_H
