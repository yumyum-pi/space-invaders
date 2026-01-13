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
typedef struct {
  float health;
} Enemy;

// This matches your Pool_func_itr signature: void func(void* payload, void* args)
void apply_damage(void* payload, void* args) {
  Enemy* enemy = (Enemy*)payload;
  float damage = *(float*)args;
  enemy->health -= damage;
}

void test_iterator() {
  printf("Testing object_pool_itr... ");

  // 1. Setup pool and data
  Pool* pool = new_object_pool(sizeof(Enemy), 10);

  // Borrow 3 objects
  Enemy* e1 = (Enemy*)object_pool_borrow(pool);
  Enemy* e2 = (Enemy*)object_pool_borrow(pool);
  Enemy* e3 = (Enemy*)object_pool_borrow(pool);

  e1->health = 100.0f;
  e2->health = 100.0f;
  e3->health = 100.0f;

  // 2. Run Iterator
  float damage_amount = 25.0f;
  object_pool_itr(pool, apply_damage, &damage_amount);

  // 3. Verify
  assert(e1->health == 75.0f);
  assert(e2->health == 75.0f);
  assert(e3->health == 75.0f);

  // 4. Test that it skips unused objects
  // If we return e2, the iterator shouldn't touch it anymore
  object_pool_return(pool, e2);
  float more_damage = 10.0f;
  object_pool_itr(pool, apply_damage, &more_damage);

  assert(e1->health == 65.0f);  // Was 75, now 65
  assert(e2->health == 75.0f);  // Should still be 75 because it's not "used"
  assert(e3->health == 65.0f);  // Was 75, now 65

  remove_object_pool(pool);
  printf("PASSED\n");
}

int main() {
  printf("--- Starting Pool Tests ---\n");
  test_basic_allocation();
  test_pool_exhaustion();
  test_recycling();
  test_alignment();
  test_iterator();
  printf("--- All Tests Passed! ---\n");
  return 0;
}
