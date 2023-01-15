#include <stdio.h>

#include "Point3d.h"

Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z) {
  Point3d* new_point;

  new_point = (Point3d*)malloc(sizeof(Point3d));
  assert(new_point != NULL);

  new_point->x = x;
  new_point->y = y;
  new_point->z = z;

  return new_point;
}

void Point3d_Scale(Point3d* point, int32_t scale) {
  point->x *= scale;
  point->y *= scale;
  point->z *= scale;
}

Point3d Point3d_GetOrigin() {
  Point3d* origin = Point3d_Allocate(0, 0, 0);
  return *origin;
}