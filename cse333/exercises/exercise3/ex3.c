// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <stdlib.h>  // for free(), EXIT_SUCCESS
#include <assert.h>  // for assert()
#include <stdio.h>   // for printf(), NULL

#include "Point3d.h"

#define X 111      // testing value for x
#define Y 222      // testing value for y
#define Z 333      // testing value for z
#define SCALE 444  // testing value for scale

int main(int argc, char* argv[]) {
  Point3d* test_point;
  Point3d test_origin;
  int32_t test_x, test_y, test_z, test_scale;
  int i;

  // Initialize testing parameters.
  test_x = X;
  test_y = Y;
  test_z = Z;
  test_scale = SCALE;

  // Prints out the testing parameters for the current trial.
  printf("Testing with parameters: x = %d, y = %d, z = %d, scale = %d\n",
         test_x, test_y, test_z, test_scale);

  // Initialize a new Point3d. Deliberately crashes with a failed assertion if
  // there is a mismatch between the expected values and the Point3d
  // fields.
  //
  // Test the function Point3d_Allocate with the given parameters.
  test_point = Point3d_Allocate(test_x, test_y, test_z);
  assert(test_point->x == test_x);
  assert(test_point->y == test_y);
  assert(test_point->z == test_z);
  printf("Point3d_Allocate test passed\n");

  // Test the function Point3d_Scale with the given parameters.
  Point3d_Scale(test_point, test_scale);
  assert(test_point->x == (test_scale * test_x));
  assert(test_point->y == (test_scale * test_y));
  assert(test_point->z == (test_scale * test_z));
  printf("Point3d_Scale test passed\n");

  // Test the function Point3d_GetOrigin.
  test_origin = Point3d_GetOrigin();
  assert(test_origin.x == 0);
  assert(test_origin.y == 0);
  assert(test_origin.z == 0);
  printf("Point3d_GetOrigin test passed\n");

  free(test_point);

  return EXIT_SUCCESS;
}
