#include "./sprite.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t length;
  size_t width;  // Useful if the sprite is multi-line
  size_t height;
} Info;

Info getInfoStr(const char* str) {
  Info i = {0, 0, 0};
  if (str == NULL) {
    return i;
  }
  char current_c = '0';
  size_t currentWidth = 0;
  while (1) {
    current_c = str[i.length];
    i.length++;
    if (current_c == '\0' || current_c == '\r' || current_c == '\n') {
      if (currentWidth > i.width) {
        i.width = currentWidth;
      }
      currentWidth = 0;
      i.height++;

      if (current_c == '\0') {
        break;
      }
    } else {
      currentWidth++;
    }
  }
  return i;
}

Sprite* SpriteNewFromStr(const char* str) {
  if (!str) {
    return NULL;
  }

  Sprite* s = malloc(sizeof(Sprite));
  if (!s) {
    return NULL;
  }

  // count the no. of new lines
  // count the max width
  // count the length
  Info i = getInfoStr(str);

  s->buffer = malloc(sizeof(char) * (i.length));
  if (!s->buffer) {
    free(s);
    return NULL;
  }

  memcpy(s->buffer, str, i.length);
  s->width = i.width;
  s->length = i.length - 1;  // excluding \0
  s->height = i.height;
  return s;
}

void SpriteFree(Sprite* s) {
  if (!s) {
    return;
  }
  if (!s->buffer) {
    free(s);
    return;
  }

  free(s->buffer);
  free(s);
}
