// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <time.h>    // for time()
#include <stdlib.h>  // for srand(), rand(), free(), EXIT_SUCCESS
#include <assert.h>  // for assert()
#include <stdio.h>   // for printf(), NULL

#include "Point3d.h"

#define TEST_TRIALS 1000       // total number of testing trials
#define TEST_RANGE_LOWER -500  // the lower bound of testing values
#define TEST_RANGE_UPPER 500   // the upper bound of testing values

// Returns a random number between TEST_RANGER_LOWER and
// TEST_RANGE_UPPER with the given seed.
int32_t GetRandomNum(unsigned int* rand_seed);

int main(int argc, char* argv[]) {
  Point3d* test_point;
  Point3d test_origin;
  int32_t test_x, test_y, test_z, test_scale;
  int i;

  // Initialize random seed.
  unsigned int rand_seed = time(NULL);

  // Repeat the test for TEST_TRIALS number of times with random parameters.
  for (i = 0; i < TEST_TRIALS; i++) {
    // Initialize x,y,z, and scale with random numbers between TEST_RANGER_LOWER
    // and TEST_RANGE_UPPER.
    test_x = GetRandomNum(&rand_seed);
    test_y = GetRandomNum(&rand_seed);
    test_z = GetRandomNum(&rand_seed);
    test_scale = GetRandomNum(&rand_seed);

    // Prints out the testing parameters for the current trial.
    printf("Testing with parameters: x = %d, y = %d, z = %d, scale = %d\n",
           test_x, test_y, test_z, test_scale);

    // Initialize a new Point3d. Deliberately crashes with a failed assertion if
    // there is a mismatch between the expected values and the Point3d
    // fields.
    //
    // Test the function Point3d_Allocate with current parameters.
    test_point = Point3d_Allocate(test_x, test_y, test_z);
    assert(test_point->x == test_x);
    assert(test_point->y == test_y);
    assert(test_point->z == test_z);
    printf("Point3d_Allocate test# %d passed\n", i);

    // Test the function Point3d_Scale with current parameters.
    Point3d_Scale(test_point, test_scale);
    assert(test_point->x == (test_scale * test_x));
    assert(test_point->y == (test_scale * test_y));
    assert(test_point->z == (test_scale * test_z));
    printf("Point3d_Scale test# %d passed\n", i);

    printf("\n");

    free(test_point);
  }

  // test the function Point3d_GetOrigin.
  test_origin = Point3d_GetOrigin();
  assert(test_origin.x == 0);
  assert(test_origin.y == 0);
  assert(test_origin.z == 0);
  printf("Point3d_GetOrigin test passed\n");

  return EXIT_SUCCESS;
}

int32_t GetRandomNum(unsigned int* rand_seed) {
  return (rand_r(rand_seed) % (TEST_RANGE_UPPER - TEST_RANGE_LOWER + 1)) +
         TEST_RANGE_LOWER;
}
