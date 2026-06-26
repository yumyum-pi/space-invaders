#ifndef LEVEL_H
#define LEVEL_H

#include <sys/wait.h>
#include <unistd.h>
#include "enemy.h"
#include "utils/math.h"
typedef enum { ENEMY } EntityType;

typedef struct {
  IVec2 start_position;
  EnemyType type;
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
  size_t current_index;
} Level;

Level* Level1();
Level* LevelTutorial();
SpawnEvent* LevelGetSpawnEvent(Level* level, int frame_count);
void LevelRemove(Level* l);

#endif  // !LEVEL_H
