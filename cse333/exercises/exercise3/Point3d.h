// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#ifndef PRINT3D_H_  // Header Guard, continue if PRINT3D_H_ is not defined
#define PRINT3D_H_  // Header Guard, define PRINT3D_H_

#include <stdint.h>  // for int32_t

// A "Point3d" is the sructure that defines a point in a three-dimensional
// coordinate system, which contains int32_t fields for x, y, and z coordinates.
typedef struct point3d_st {
  int32_t x;  // x coordinate
  int32_t y;  // y coordinate
  int32_t z;  // z coordinate
} Point3d;

// Allocates a new Point3d with the given x, y, z coordinates.
// Deliberately crashes with a failed assertion if there is an out-of-memory
// error.
//
// Arguments:
//
// - x: x coordinate
// - y: y coordinate
// - z: z coordinate
//
// Returns:
//
// - a pointer to the new Point3d.
Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z);

// Scales the coordinates of the given Point3d by the given value.
//
// Arguments:
//
// - point: a pointer to the Point3d whose coordinates are scaled
// - scale: the value that the x, y, and z coordinates are scaled by
//
// Returns nothing.
void Point3d_Scale(Point3d* point, int32_t scale);

// Constructs a Point3d with x, y, and z coordinates equal to zero and returns a
// copy of the Point3d.
//
// Accepts no arguments.
//
// Returns a copy of the Point3d with x, y, and z coordinates equal to zero.
Point3d Point3d_GetOrigin();

#endif  // PRINT3D_H_
