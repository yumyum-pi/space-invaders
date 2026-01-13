#include "object_pool.h"
#include <assert.h>
#include <stdalign.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int x, y;
  char name[16];
} Entity;

void test_basic_allocation() {
  printf("Testing basic allocation... ");
  Pool* pool = new_object_pool(sizeof(Entity), 5);

  Entity* e1 = (Entity*)object_pool_borrow(pool);
  assert(e1 != NULL);

  // Ensure we can write to it without crashing
  e1->x = 10;
  e1->y = 20;
  strcpy(e1->name, "Player");

  assert(e1->x == 10);
  remove_object_pool(pool);
  printf("PASSED\n");
}

void test_pool_exhaustion() {
  printf("Testing pool exhaustion... ");
  size_t size = 3;
  Pool* pool = new_object_pool(sizeof(Entity), size);

  // Drain the pool
  for (size_t i = 0; i < size; i++) {
    assert(object_pool_borrow(pool) != NULL);
  }

  // Next borrow should be NULL
  assert(object_pool_borrow(pool) == NULL);

  remove_object_pool(pool);
  printf("PASSED\n");
}

void test_recycling() {
  printf("Testing memory recycling... ");
  Pool* pool = new_object_pool(sizeof(Entity), 1);

  Entity* first = (Entity*)object_pool_borrow(pool);
  assert(first != NULL);

  // Return it
  object_pool_return(pool, first);

  // Borrow again - it should give us the exact same pointer back
  Entity* second = (Entity*)object_pool_borrow(pool);
  assert(first == second);

  remove_object_pool(pool);
  printf("PASSED\n");
}

void test_alignment() {
  printf("Testing alignment logic... ");
  // Request a weird size (7 bytes)
  Pool* pool = new_object_pool(7, 10);

  void* p1 = object_pool_borrow(pool);
  void* p2 = object_pool_borrow(pool);

  // The distance between pointers should be a multiple of max_align_t
  uintptr_t diff = (uintptr_t)p2 - (uintptr_t)p1;
  assert(diff % alignof(max_align_t) == 0);
  assert(diff >= 7);

  remove_object_pool(pool);
  printf("PASSED\n");
}

int main() {
  printf("--- Starting Pool Tests ---\n");
  test_basic_allocation();
  test_pool_exhaustion();
  test_recycling();
  test_alignment();
  printf("--- All Tests Passed! ---\n");
  return 0;
}
