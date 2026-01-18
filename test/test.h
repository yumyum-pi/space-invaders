#ifndef TEST_H
#define TEST_H

#define TEST(name) void test_##name()
#define RUN_TEST(name)                   \
  do {                                   \
    printf("Running test_%s...", #name); \
    test_##name();                       \
    printf(" PASSED\n");                 \
  } while (0)

#endif  // !TEST_H
