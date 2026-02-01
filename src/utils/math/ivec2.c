#include "./ivec2.h"
#include "common.h"

IVec2 IVec2New(int x, int y) {
  return (IVec2){x, y};
}

IVec2 IVec2Zero(void) {
  return (IVec2){0, 0};
}
IVec2 IVec2One(void) {
  return (IVec2){1, 1};
}

IVec2 IVec2Add(IVec2 a, IVec2 b) {
  return (IVec2){a.x + b.x, a.y + b.y};
}
IVec2 IVec2Sub(IVec2 a, IVec2 b) {
  return (IVec2){a.x - b.x, a.y - b.y};
}

IVec2 IVec2Scale_M(IVec2 a, int factor) {
  return (IVec2){a.x * factor, a.y * factor};
}
IVec2 IVec2Scale_D(IVec2 a, int factor) {
  if (factor == 0) {
    return a;
  }
  return (IVec2){a.x / factor, a.y / factor};
}

bool IVec2Equal(IVec2 a, IVec2 b) {
  return (a.x == b.x && a.y == b.y);
}
IVec2 IVec2Clamp(IVec2 a, IVec2 min, IVec2 max) {
  return (IVec2){IClamp(a.x, min.x, max.x), IClamp(a.y, min.y, max.y)};
}

IVec2 IVec2MoveTowards(IVec2 current, IVec2 target, int dx) {
  return (IVec2){IMoveTowards(current.x, target.x, dx),
                 IMoveTowards(current.y, target.y, dx)};
}

int IVec2DistanceSq(IVec2 a, IVec2 b) {
  int x = a.x - b.x;
  int y = a.y - b.y;
  return x * x + y * y;
}

IVec2 IVec2Abs(IVec2 a) {
  return (IVec2){IAbs(a.x), IAbs(a.y)};
}
