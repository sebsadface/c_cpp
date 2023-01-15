#ifndef PRINT3D_H_
#define PRINT3D_H_

#include <stdint.h>

typedef struct point3d_st {
  int32_t x;
  int32_t y;
  int32_t z;
} Point3d;

Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z);

void Point3d_Scale(Point3d* point, int32_t scale);

Point3d Point3d_GetOrigin();

#endif  // PRINT3D_H_