#include "./irect.h"
#include <assert.h>
#include "common.h"
#include "ivec2.h"

// Constructors
IRect IRectNew(IVec2 min, IVec2 max) {
  assert(min.x < max.x);
  assert(min.y < max.y);
  return (IRect){min, max};
}
IRect IRectFromCenter(IVec2 center, IVec2 size) {
  IVec2 half = IVec2Scale_D(size, 2);
  IVec2 min = IVec2Sub(center, half);
  IVec2 max = IVec2Add(center, half);
  return (IRect){min, max};
}

IRect IRectZero(void) {
  return (IRect){IVec2Zero(), IVec2Zero()};
}
IRect IRectOne(void) {
  return (IRect){IVec2One(), IVec2One()};
}

// Collision checks
bool IRectColideIVec2(IRect a, IVec2 p) {
  return (p.x <= a.max.x && p.x >= a.min.x) &&
         (p.y <= a.max.y && p.y >= a.min.y);
}
bool IRectColide(IRect a, IRect b) {
  return (a.min.x < b.max.x && a.max.x > b.min.x) &&
         (a.min.y < b.max.y && a.max.y > b.min.y);
}

// Getters
int IRectGetWidth(IRect a) {
  return IAbs(a.max.x - a.min.x);
}

int IRectGetHeight(IRect a) {
  return IAbs(a.max.y - a.min.y);
}

IVec2 IRectGetSize(IRect a) {
  return IVec2Abs(IVec2Sub(a.max, a.min));
}

IVec2 IRectGetCenter(IRect a) {
  IVec2 size = IRectGetSize(a);
  IVec2 halfSize = IVec2Scale_D(size, 2);
  return IVec2Add(a.min, halfSize);
}

// Transformations
IRect IRectOffset(IRect a, IVec2 offset) {
  return (IRect){
      IVec2Add(a.min, offset),
      IVec2Add(a.max, offset),
  };
}

IRect IRectClamp(IRect b, IRect bounds) {
  return (IRect){
      IVec2Clamp(b.min, bounds.min, bounds.max),
      IVec2Clamp(b.max, bounds.min, bounds.max),
  };
}
IRect IRectGrow(IRect a, int factor) {
  return (IRect){
      IVec2Scale_M(a.min, factor),
      IVec2Scale_M(a.max, factor),
  };
}
IRect IRectShrink(IRect a, int factor) {
  return (IRect){
      IVec2Scale_D(a.min, factor),
      IVec2Scale_D(a.max, factor),
  };
}

// Overlap / Intersection
bool IRectContains(IRect outer, IRect inner) {
  // inner.min should be greater than outer.min
  return (inner.min.x >= outer.min.x && inner.min.y >= outer.min.y) &&
         // inner.max should be less than outer.max
         (inner.max.x <= outer.max.x && inner.max.y <= outer.max.y);
}
IRect IRectIntersection(IRect a, IRect b) {
  // check if colliding
  if (!IRectContains(a, b)) {
    return IRectZero();
  }
  return (IRect){
      {
          IMax(a.min.x, b.min.x),
          IMax(a.min.y, b.min.y),
      },
      {
          IMin(a.max.x, b.max.x),
          IMin(a.max.y, b.max.y),
      },
  };
}

IVec2 IRectGetOverlap(IRect a, IRect b) {
  return IRectGetSize(IRectIntersection(a, b));
}

int IRectDistanceSq(IRect a, IRect b) {
  return IVec2DistanceSq(IRectGetCenter(a), IRectGetCenter(b));
}
int IRectDistanceSqIVec2(IRect a, IVec2 p) {
  return IVec2DistanceSq(IRectGetCenter(a), p);
}
