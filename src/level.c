#include "./level.h"
#include <assert.h>
#include <stdlib.h>

Level* NewLevel(size_t capcity) {
  assert(capcity != 0);

  Level* l = malloc(sizeof(Level));
  if (l == NULL) {
    return NULL;
  }

  l->capacity = capcity;
  l->events = malloc(sizeof(SpawnEvent) * l->capacity);
  if (l->events == NULL) {
    free(l);
    return NULL;
  }
  return l;
}

bool LevelSpawnEventAppend(Level* l, SpawnEvent event) {
  assert(l->length <= l->capacity);
  if (l->length == l->capacity) {
    return false;
  }
  l->events[l->length] = event;
  l->length++;
  return true;
}

Level* Level1() {
  Level* l = NewLevel(10);

  LevelSpawnEventAppend(
      l, (SpawnEvent){
             .frame = 0,
             .entity = ENEMY,
             .args = {.enemyArgs = {.start_position = {.x = 40, .y = 5}}}});
  LevelSpawnEventAppend(
      l, (SpawnEvent){
             .frame = 0,
             .entity = ENEMY,
             .args = {.enemyArgs = {.start_position = {.x = 90, .y = 5}}}});
  LevelSpawnEventAppend(
      l, (SpawnEvent){
             .frame = 24 * 5,
             .entity = ENEMY,
             .args = {.enemyArgs = {.start_position = {.x = 90, .y = 5}}}});

  return NULL;
};
