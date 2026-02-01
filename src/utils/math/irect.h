#ifndef I_RECT_H
#define I_RECT_H

#include <stdbool.h>
#include "ivec2.h"

typedef struct {
  IVec2 min;
  IVec2 max;
} IRect;

IRect IRectNew(IVec2 min, IVec2 max);
IRect IRectFromCenter(IVec2 center, IVec2 size);

IRect IRectZero(void);
IRect IRectOne(void);

bool IRectColideIVec2(IRect a, IVec2 p);
bool IRectCollide(IRect a, IRect b);

int IRectGetWidth(IRect a);
int IRectGetHeight(IRect a);
IVec2 IRectGetSize(IRect a);
IVec2 IRectGetCenter(IRect a);

IRect IRectOffset(IRect a, IVec2 offset);
IRect IRectClamp(IRect b, IRect bounds);
IRect IRectGrow(IRect a, int factor);
IRect IRectShrink(IRect a, int factor);

bool IRectContains(IRect outer, IRect inner);
IRect IRectIntersection(IRect a, IRect b);
IVec2 IRectGetOverlap(IRect a, IRect b);
int IRectDistanceSq(IRect a, IRect b);
int IRectDistanceSqIVec2(IRect a, IVec2 p);

#endif  // !I_RECT_H
