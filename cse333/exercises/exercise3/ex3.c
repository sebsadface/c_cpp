#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "Point3d.h"

int main(int argc, char* argv[]) {
  Point3d* test_point;
  Point3d test_origin;
  int32_t test_x, test_y, test_z, test_scale;
  int i;

  srand(time(NULL));
  for (i = 0; i < 1000; i++) {
    test_x = rand() % 1000;
    test_y = rand() % 1000;
    test_z = rand() % 1000;
    test_scale = rand() % 1000;

    printf("Testing with parameters: x = %d, y = %d, z = %d, scale = %d\n",
           test_x, test_y, test_z, test_scale);

    test_point = Point3d_Allocate(test_x, test_y, test_z);
    assert(test_point->x == test_x);
    assert(test_point->y == test_y);
    assert(test_point->z == test_z);
    printf("Point3d_Allocate trial %d passed\n", i);

    Point3d_Scale(test_point, test_scale);
    assert(test_point->x == (test_scale * test_x));
    assert(test_point->y == (test_scale * test_y));
    assert(test_point->z == (test_scale * test_z));
    printf("Point3d_Scale trial %d passed\n", i);

    test_origin = Point3d_GetOrigin();
    assert(test_origin.x == 0);
    assert(test_origin.y == 0);
    assert(test_origin.z == 0);
    printf("Point3d_GetOrigin trial %d passed\n", i);
    printf("\n");

    free(test_point);
  }
}
