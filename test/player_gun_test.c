#include <assert.h>
#include <stdio.h>
#include "../src/gun.c"
#include "./test.h"

TEST(can_fire_with_ammo_and_no_cooldown) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  bool result = gun_fire(&g, 10, true);
  assert(result == true);
  assert(g.remaining_rounds == 4);
}

TEST(cannot_fire_during_cooldown) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  bool result = gun_fire(&g, 5, true);  // Only 5 frames passed, need 10
  assert(result == false);
}

TEST(cannot_fire_without_ammo) {
  Gun g = new_gun(10, 60, 5);
  g.remaining_rounds = 0;
  g.last_fired_frame = 0;

  bool result = gun_fire(&g, 10, true);
  assert(result == false);
}

TEST(cannot_fire_while_reloading) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.last_fired_frame = 0;

  bool result = gun_fire(&g, 30, true);  // Halfway through reload
  assert(result == false);
  assert(g.is_reloading == true);  // Still reloading
}

TEST(reload_completes_and_refills_magazine) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.remaining_rounds = 0;
  g.last_fired_frame = 0;

  bool result = gun_fire(&g, 60, true);  // Reload time reached

  assert(result == true);
  assert(g.is_reloading == false);
  assert(g.remaining_rounds == 4);  //imidiate fire after reload
  assert(g.last_fired_frame == 60);
}

TEST(fire_rate_boundary_exact) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  // Exactly at fire rate boundary
  bool result = gun_fire(&g, 10, true);
  assert(result == true);

  // One frame before
  g.last_fired_frame = 0;
  result = gun_fire(&g, 9, true);
  assert(result == false);
}

TEST(reload_boundary_exact) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.last_fired_frame = 0;

  // Exactly at reload boundary
  gun_fire(&g, 60, true);
  assert(g.is_reloading == false);

  // One frame before
  g.is_reloading = true;
  g.last_fired_frame = 0;
  gun_fire(&g, 59, true);
  assert(g.is_reloading == true);
}

TEST(multiple_shots_sequence) {
  Gun g = new_gun(10, 60, 3);

  // Shot 1
  assert(gun_fire(&g, 0, true) == true);
  g.last_fired_frame = 0;
  g.remaining_rounds = 2;

  // Too soon
  assert(gun_fire(&g, 5, true) == false);

  // Shot 2
  assert(gun_fire(&g, 10, true) == true);
  g.last_fired_frame = 10;
  g.remaining_rounds = 1;

  // Shot 3
  assert(gun_fire(&g, 20, true) == true);
  g.last_fired_frame = 20;
  g.remaining_rounds = 0;

  // Out of ammo
  assert(gun_fire(&g, 30, true) == false);
}

TEST(reload_then_fire_full_sequence) {
  Gun g1 = new_gun(10, 60, 3);
  g1.remaining_rounds = 0;
  g1.is_reloading = true;
  g1.last_fired_frame = 0;

  // During reload
  assert(gun_fire(&g1, 30, true) == false);

  // Reload completes at frame 60
  assert(gun_fire(&g1, 60, false) == false);
  assert(g1.is_reloading == false);
  assert(g1.remaining_rounds == 3);
  assert(g1.last_fired_frame == 0);

  Gun g2 = new_gun(10, 60, 3);
  g2.remaining_rounds = 0;
  g2.is_reloading = true;
  g2.last_fired_frame = 0;
  // Can fire at frame 70
  assert(gun_fire(&g2, 60, true) == true);
  assert(g2.is_reloading == false);
  assert(g2.remaining_rounds == 2);
  assert(g2.last_fired_frame == 60);
}

TEST(auto_reload_when_magazine_empty) {
  Gun g = new_gun(10, 60, 3);
  g.remaining_rounds = 1;
  g.last_fired_frame = 0;

  // Fire last round
  bool result = gun_fire(&g, 10, true);
  assert(result == true);
  assert(g.remaining_rounds == 0);
  assert(g.is_reloading == true);  // Should auto-reload

  // Cannot fire while reloading
  result = gun_fire(&g, 20, true);
  assert(result == false);

  // Reload completes
  result = gun_fire(&g, 70, true);  // 10 + 60
  assert(g.is_reloading == false);
  assert(g.remaining_rounds == 2);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  printf("Running gun_fire tests...\n\n");

  RUN_TEST(can_fire_with_ammo_and_no_cooldown);
  RUN_TEST(cannot_fire_during_cooldown);
  RUN_TEST(cannot_fire_without_ammo);
  RUN_TEST(cannot_fire_while_reloading);
  RUN_TEST(reload_completes_and_refills_magazine);
  RUN_TEST(fire_rate_boundary_exact);
  RUN_TEST(reload_boundary_exact);
  RUN_TEST(multiple_shots_sequence);
  RUN_TEST(reload_then_fire_full_sequence);
  RUN_TEST(auto_reload_when_magazine_empty);

  printf("\n✓ All tests passed!\n");
  return 0;
}
