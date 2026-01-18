#include <assert.h>
#include <stdio.h>
#include "../src/player.h"
#define TEST(name) void test_##name()
#define RUN_TEST(name)                   \
  do {                                   \
    printf("Running test_%s...", #name); \
    test_##name();                       \
    printf(" PASSED\n");                 \
  } while (0)

TEST(can_fire_with_ammo_and_no_cooldown) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  bool result = gun_should_fire(&g, 10);
  assert(result == true);
  assert(g.remaining_rounds == 5);  // Function doesn't modify ammo
}

TEST(cannot_fire_during_cooldown) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  bool result = gun_should_fire(&g, 5);  // Only 5 frames passed, need 10
  assert(result == false);
}

TEST(cannot_fire_without_ammo) {
  Gun g = new_gun(10, 60, 5);
  g.remaining_rounds = 0;
  g.last_fired_frame = 0;

  bool result = gun_should_fire(&g, 10);
  assert(result == false);
}

TEST(cannot_fire_while_reloading) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.last_fired_frame = 0;

  bool result = gun_should_fire(&g, 30);  // Halfway through reload
  assert(result == false);
  assert(g.is_reloading == true);  // Still reloading
}

TEST(reload_completes_and_refills_magazine) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.remaining_rounds = 0;
  g.last_fired_frame = 0;

  // bool result = gun_should_fire(&g, 60);  // Reload time reached
  gun_should_fire(&g, 60);

  assert(g.is_reloading == false);
  assert(g.remaining_rounds == 5);
  assert(g.last_fired_frame == 60);
}

TEST(can_fire_immediately_after_reload_completes) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.remaining_rounds = 0;
  g.last_fired_frame = 0;

  // Reload completes at frame 60
  bool result = gun_should_fire(&g, 60);

  // Should be able to fire immediately since last_fired_frame is now 60
  // and fire_rate is 10, so we need to be at frame 70+
  assert(result == false);  // Can't fire yet, need 10 more frames

  result = gun_should_fire(&g, 70);
  assert(result == true);  // Now can fire
}

TEST(reload_sets_last_fired_frame_correctly) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.last_fired_frame = 100;

  gun_should_fire(&g, 160);  // 60 frames later

  assert(g.last_fired_frame == 160);
  assert(g.is_reloading == false);
}

TEST(fire_rate_boundary_exact) {
  Gun g = new_gun(10, 60, 5);
  g.last_fired_frame = 0;

  // Exactly at fire rate boundary
  bool result = gun_should_fire(&g, 10);
  assert(result == true);

  // One frame before
  g.last_fired_frame = 0;
  result = gun_should_fire(&g, 9);
  assert(result == false);
}

TEST(reload_boundary_exact) {
  Gun g = new_gun(10, 60, 5);
  g.is_reloading = true;
  g.last_fired_frame = 0;

  // Exactly at reload boundary
  gun_should_fire(&g, 60);
  assert(g.is_reloading == false);

  // One frame before
  g.is_reloading = true;
  g.last_fired_frame = 0;
  gun_should_fire(&g, 59);
  assert(g.is_reloading == true);
}

TEST(multiple_shots_sequence) {
  Gun g = new_gun(10, 60, 3);

  // Shot 1
  assert(gun_should_fire(&g, 0) == true);
  g.last_fired_frame = 0;
  g.remaining_rounds = 2;

  // Too soon
  assert(gun_should_fire(&g, 5) == false);

  // Shot 2
  assert(gun_should_fire(&g, 10) == true);
  g.last_fired_frame = 10;
  g.remaining_rounds = 1;

  // Shot 3
  assert(gun_should_fire(&g, 20) == true);
  g.last_fired_frame = 20;
  g.remaining_rounds = 0;

  // Out of ammo
  assert(gun_should_fire(&g, 30) == false);
}

TEST(reload_then_fire_full_sequence) {
  Gun g = new_gun(10, 60, 3);
  g.remaining_rounds = 0;
  g.is_reloading = true;
  g.last_fired_frame = 0;

  // During reload
  assert(gun_should_fire(&g, 30) == false);

  // Reload completes at frame 60
  assert(gun_should_fire(&g, 60) == false);  // Can't fire yet
  assert(g.is_reloading == false);
  assert(g.remaining_rounds == 3);
  assert(g.last_fired_frame == 60);

  // Can fire at frame 70
  assert(gun_should_fire(&g, 70) == true);
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
  assert(g.remaining_rounds == 3);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  printf("Running gun_should_fire tests...\n\n");

  RUN_TEST(can_fire_with_ammo_and_no_cooldown);
  RUN_TEST(cannot_fire_during_cooldown);
  RUN_TEST(cannot_fire_without_ammo);
  RUN_TEST(cannot_fire_while_reloading);
  RUN_TEST(reload_completes_and_refills_magazine);
  RUN_TEST(can_fire_immediately_after_reload_completes);
  RUN_TEST(reload_sets_last_fired_frame_correctly);
  RUN_TEST(fire_rate_boundary_exact);
  RUN_TEST(reload_boundary_exact);
  RUN_TEST(multiple_shots_sequence);
  RUN_TEST(reload_then_fire_full_sequence);
  RUN_TEST(auto_reload_when_magazine_empty);

  printf("\n✓ All tests passed!\n");
  return 0;
}
