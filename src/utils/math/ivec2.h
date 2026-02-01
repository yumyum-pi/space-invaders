#ifndef I_VEC_H
#define I_VEC_H
#include <stdbool.h>

typedef struct {
  int x;
  int y;
} IVec2;

IVec2 IVec2New(int x, int y);

IVec2 IVec2Zero(void);
IVec2 IVec2One(void);

IVec2 IVec2Add(IVec2 a, IVec2 b);
IVec2 IVec2Sub(IVec2 a, IVec2 b);
IVec2 IVec2Scale_M(IVec2 a, int factor);
IVec2 IVec2Scale_D(IVec2 a, int factor);
bool IVec2Equal(IVec2 a, IVec2 b);

IVec2 IVec2Clamp(IVec2 a, IVec2 min, IVec2 max);
int IVec2DistanceSq(IVec2 a, IVec2 b);
IVec2 IVec2Abs(IVec2 a);

IVec2 IVec2MoveTowards(IVec2 current, IVec2 target, int dx);

#endif
