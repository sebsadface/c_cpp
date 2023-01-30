#include "Vector.h"

#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

#define X_COORD 14.625
#define Y_COORD -25.25
#define Z_COORD 3.5
#define X_COORD2 34.25
#define Y_COORD2 6.75
#define Z_COORD2 -64.5
#define AFTER_INCREMENT 1

bool TestCtorAndOptor();
void Increment(Vector v);
bool VerifyAddress(Vector& vref, void* address);
void IncrementTest(const Vector v);

int main(int argc, char** argv) {
  if (!TestCtorAndOptor()) {
    cerr << stderr << "Testing failed" << endl;
    return EXIT_FAILURE;
  }

  Vector v;
  Increment(v);
  IncrementTest((Vector&)v);

  if (VerifyAddress(v, &v)) {
    cout << "Ref: same address" << endl;
  } else {
    cout << "Ref: different address" << endl;
  }
}

bool TestCtorAndOptor() {
  Vector v1;
  if (v1.get_x() != 0 || v1.get_y() != 0 || v1.get_z() != 0) {
    cerr << stderr << " Default vector constructor failed" << endl;
    return false;
  }

  Vector v2(X_COORD, Y_COORD, Z_COORD);
  Vector v3(X_COORD2, Y_COORD2, Z_COORD2);
  if (v2.get_x() != X_COORD || v2.get_y() != Y_COORD || v2.get_z() != Z_COORD ||
      v3.get_x() != X_COORD2 || v3.get_y() != Y_COORD2 ||
      v3.get_z() != Z_COORD2) {
    cerr << stderr << " Three-parameter vector constructor failed" << endl;
    return false;
  }

  Vector cp(v2);
  if (cp.get_x() != X_COORD || cp.get_y() != Y_COORD || cp.get_z() != Z_COORD) {
    cerr << stderr << " Copy vector constructor failed" << endl;
    return false;
  }

  v1 = v2;
  if (v1.get_x() != X_COORD || v1.get_y() != Y_COORD || v1.get_z() != Z_COORD) {
    cerr << stderr << " Overwritting assignment operator = failed" << endl;
    return false;
  }

  v2 += v3;
  if (v2.get_x() != X_COORD + X_COORD2 || v2.get_y() != Y_COORD + Y_COORD2 ||
      v2.get_z() != Z_COORD + Z_COORD2) {
    cerr << stderr << " Overwritting operator += failed" << endl;
    return false;
  }

  v2 -= v3;
  if (v2.get_x() != X_COORD || v2.get_y() != Y_COORD || v2.get_z() != Z_COORD) {
    cerr << stderr << " Overwritting operator -= failed" << endl;
    return false;
  }

  float dotProduct = v2 * v3;
  if (dotProduct != v2.get_x() * v3.get_x() + v2.get_y() * v3.get_y() +
                        v2.get_z() * v3.get_z()) {
    cerr << stderr << " Overwritting operator * failed" << endl;
    return false;
  }

  return true;
}

void Increment(Vector v) {
  Vector one(1, 1, 1);
  v += one;
}

bool VerifyAddress(Vector& vref, void* address) {
  if (&vref == address) {
    return true;
  } else {
    return false;
  }
}

void IncrementTest(const Vector v) {
  if (v.get_x() != AFTER_INCREMENT || v.get_y() != AFTER_INCREMENT ||
      v.get_z() != AFTER_INCREMENT) {
    cout << "Vector: pass-by-value" << endl;
  } else {
    cout << "Vector: pass-by-reference" << endl;
  }
}