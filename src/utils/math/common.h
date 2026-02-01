#ifndef MATH_COMMON_H
#define MATH_COMMON_H

int IMax(int a, int b);
int IMin(int a, int b);
int IAbs(int a);
int IClamp(int source, int min, int max);

int IMoveTowards(int current, int target, int dx);
int ISinLUT(int angle);

#endif  // MATH_COMMON_H
