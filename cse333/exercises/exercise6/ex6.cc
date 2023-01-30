// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include "Vector.h"  // for Vector

#include <cstdlib>   // for EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>  // for cout, cerr, endl

using std::cerr;
using std::cout;
using std::endl;

#define X_COORD1 14.625
#define Y_COORD1 -25.25
#define Z_COORD1 3.75
#define X_COORD2 34.25
#define Y_COORD2 6.75
#define Z_COORD2 -64.5
#define AFTER_INCREMENT 1  // expected coordinate values after increment

// Test that the default (0-argument) constructor initializes a Vector to
// (0,0,0).
// Returns true if tests pass, false otherwise.
bool TestDefaultCtor();

// Test that the 3-argument constructor initializes a Vector to
// the given value (x,y,z).
// Returns true if tests pass, false otherwise.
bool Test3ArgCtor();

// Test that the copy constructor initializes a new Vector to
// have the same value as the given Vector.
// Returns true if tests pass, false otherwise.
bool TestCopyCtor();

// Test that the assignment operator "=", assign the coordinate values of the
// left hand side Vector to the right hand side Vector element-by-element.
// Returns true if tests pass, false otherwise.
bool TestAssignOptor();

// Test that the operator "+=" updates assignments on vectors that perform
// element-by-element addition of the Vector components.
// Returns true if tests pass, false otherwise.
bool TestAddOptor();

// Test that the operator "-=" updates assignments on vectors that perform
// element-by-element subtraction of the Vector components.
// Returns true if tests pass, false otherwise.
bool TestSubOptor();

// Test that the operator "*" computes the dot product of two Vectors as a
// float.
// Returns true if tests pass, false otherwise.
bool TestProductOptor();

// Accepts an instance of a Vector as an argument, and increments the values
// stored in the vector by one.
void Increment(Vector v);

// Accepts a Vector reference and a void* as arguments.
// Returns true if the address of the aliased Vector has the same numeric value
// as the passed-in void*, and returns false otherwise.
bool VerifyAddress(const Vector& vref, void* address);

int main(int argc, char** argv) {
  // Tests all functions defined in Vector.h.
  if (!TestDefaultCtor() || !Test3ArgCtor() || !TestCopyCtor() ||
      !TestAssignOptor() || !TestAddOptor() || !TestSubOptor() ||
      !TestProductOptor()) {
    cerr << stderr << "Testing failed" << endl;

    return EXIT_FAILURE;
  }

  Vector v;

  // Determine whether the Vector object v itself was passed by value or passed
  // by reference to Increment()
  Increment(v);
  if (v.get_x() != AFTER_INCREMENT || v.get_y() != AFTER_INCREMENT ||
      v.get_z() != AFTER_INCREMENT) {
    cout << "Vector: pass-by-value" << endl;
  } else {
    cout << "Vector: pass-by-reference" << endl;
  }

  // Determine whether the main() instance of Vector v is the same instance as
  // the one visible to VerifyAddress() by comparing their addresses.
  if (VerifyAddress(v, &v)) {
    cout << "Ref: same address" << endl;
  } else {
    cout << "Ref: different address" << endl;
  }

  return EXIT_SUCCESS;
}

bool TestDefaultCtor() {
  const Vector v;

  if (v.get_x() != 0 || v.get_y() != 0 || v.get_z() != 0) {
    cerr << stderr << " Default vector constructor failed" << endl;
    return false;
  }
  return true;
}

bool Test3ArgCtor() {
  const Vector v(X_COORD1, Y_COORD1, Z_COORD1);

  if (v.get_x() != X_COORD1 || v.get_y() != Y_COORD1 || v.get_z() != Z_COORD1) {
    cerr << stderr << " Three-parameter vector constructor failed" << endl;

    return false;
  }

  return true;
}

bool TestCopyCtor() {
  const Vector v(X_COORD1, Y_COORD1, Z_COORD1);
  const Vector cp(v);

  if (cp.get_x() != v.get_x() || cp.get_y() != v.get_y() ||
      cp.get_z() != v.get_z()) {
    cerr << stderr << " Copy vector constructor failed" << endl;
    return false;
  }
  return true;
}

bool TestAssignOptor() {
  Vector lhs;
  const Vector rhs(X_COORD1, Y_COORD1, Z_COORD1);

  lhs = rhs;

  if (lhs.get_x() != X_COORD1 || lhs.get_y() != Y_COORD1 ||
      lhs.get_z() != Z_COORD1) {
    cerr << stderr << " Overwritting assignment operator = failed" << endl;
    return false;
  }
  return true;
}

bool TestAddOptor() {
  Vector lhs(X_COORD1, Y_COORD1, Z_COORD1);
  const Vector rhs(X_COORD2, Y_COORD2, Z_COORD2);

  lhs += rhs;

  if (lhs.get_x() != X_COORD1 + X_COORD2 ||
      lhs.get_y() != Y_COORD1 + Y_COORD2 ||
      lhs.get_z() != Z_COORD1 + Z_COORD2) {
    cerr << stderr << " Overwritting operator += failed" << endl;
    return false;
  }
  return true;
}

bool TestSubOptor() {
  Vector lhs(X_COORD1, Y_COORD1, Z_COORD1);
  const Vector rhs(X_COORD2, Y_COORD2, Z_COORD2);

  lhs -= rhs;

  if (lhs.get_x() != X_COORD1 - X_COORD2 ||
      lhs.get_y() != Y_COORD1 - Y_COORD2 ||
      lhs.get_z() != Z_COORD1 - Z_COORD2) {
    cerr << stderr << " Overwritting operator -= failed" << endl;
    return false;
  }
  return true;
}

bool TestProductOptor() {
  const Vector v1(X_COORD1, Y_COORD1, Z_COORD1);
  const Vector v2(X_COORD2, Y_COORD2, Z_COORD2);

  float dotProduct = v1 * v2;

  if (dotProduct !=
      X_COORD1 * X_COORD2 + Y_COORD1 * Y_COORD2 + Z_COORD1 * Z_COORD2) {
    cerr << stderr << " Overwritting operator * failed" << endl;
    return false;
  }
  return true;
}

void Increment(Vector v) {
  const Vector one(1, 1, 1);
  v += one;
}

bool VerifyAddress(const Vector& vref, void* address) {
  if (&vref == address) {
    return true;
  } else {
    return false;
  }
}
