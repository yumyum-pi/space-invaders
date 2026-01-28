#ifndef LEVEL_H
#define LEVEL_H

#include <unistd.h>
#include "utils/math.h"
typedef enum { ENEMY } EntityType;

typedef struct {
  Vec2i start_position;
} EntityArgsEnemy;

typedef union {
  EntityArgsEnemy enemyArgs;
} EntityArgs;

typedef struct {
  int frame;
  EntityType entity;
  EntityArgs args;
} SpawnEvent;

typedef struct {
  SpawnEvent* events;
  size_t length;
  size_t capacity;
} Level;

Level* Level1();

#endif  // !LEVEL_H
