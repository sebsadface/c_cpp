// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <stdio.h>   // for NULL
#include <stdlib.h>  // for malloc(), free()
#include <assert.h>  // for assert()

#include "Point3d.h"

Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z) {
  Point3d* new_point;

  // Allocate the new Point3d.
  new_point = (Point3d*)malloc(sizeof(Point3d));
  assert(new_point != NULL);

  // Initialize the the new Point3d with given x, y, and z coordinates.
  new_point->x = x;
  new_point->y = y;
  new_point->z = z;

  return new_point;
}

void Point3d_Scale(Point3d* point, int32_t scale) {
  // Scales the coordinates of the given Point3d with the given value.
  point->x *= scale;
  point->y *= scale;
  point->z *= scale;
}

Point3d Point3d_GetOrigin() {
  // Construct a new Point3d with x, y, and z equal to zero.
  Point3d* origin_ptr = Point3d_Allocate(0, 0, 0);
  // Get a copy of the origin Point3d.
  Point3d origin = *origin_ptr;
  // Free space allocated for the origin Point3d
  free(origin_ptr);

  return origin;
}
