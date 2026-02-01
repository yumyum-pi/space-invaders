#include "./common.h"

int IMax(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int IMin(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

int IAbs(int a) {
  if (a < 0) {
    return -a;
  }
  return a;
}

int IClamp(int source, int min, int max) {
  if (source < min) {
    return min;
  }
  if (source > max) {
    return max;
  }
  return source;
}
int IMoveTowards(int current, int target, int dx) {
  // calcuate the distance
  int distance = (current < target) ? (target - current) : (current - target);

  if (distance <= dx) {
    return target;
  }

  if (current < target) {
    current += dx;
  } else {
    current -= dx;
  }
  return current;
}

#define SINE_TABLE_SIZE 128
const int sine_table[SINE_TABLE_SIZE] = {
    0,   1,   2,   3,   4,   5,   6,   8,   9,   10,  11,  12,  13,  14,  15,
    16,  16,  17,  18,  19,  19,  20,  21,  21,  22,  22,  22,  23,  23,  23,
    23,  23,  24,  23,  23,  23,  23,  23,  22,  22,  22,  21,  21,  20,  19,
    19,  18,  17,  16,  16,  15,  14,  13,  12,  11,  10,  9,   8,   6,   5,
    4,   3,   2,   1,   0,   -1,  -2,  -3,  -4,  -5,  -6,  -8,  -9,  -10, -11,
    -12, -13, -14, -15, -16, -16, -17, -18, -19, -19, -20, -21, -21, -22, -22,
    -22, -23, -23, -23, -23, -23, -24, -23, -23, -23, -23, -23, -22, -22, -22,
    -21, -21, -20, -19, -19, -18, -17, -16, -16, -15, -14, -13, -12, -11, -10,
    -9,  -8,  -6,  -5,  -4,  -3,  -2,  -1,
};

int ISinLUT(int angle) {
  int index = (angle) % SINE_TABLE_SIZE;
  return sine_table[index];
}
