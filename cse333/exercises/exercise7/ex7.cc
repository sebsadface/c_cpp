// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <iostream>
#include <string>

#include "Vector.h"

static const float X1 = 7;
static const float Y1 = 9;
static const float Z1 = 24;
static const float X2 = -2.5;
static const float Y2 = 3.1;
static const float Z2 = 2.7;
static const float X3 = 3.5;
static const float Y3 = -2.1;
static const float Z3 = 4.2;
static const float K = 1.2;

using std::cout;
using std::endl;
using std::string;
using vector333::Vector;

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations

// Output the expected test values and the values of a Vector to std::cout in
// the format "(x, y, z)" for manual comparison.
void VerifyVector(const Vector& actual, const float& expectedX,
                  const float& expectedY, const float& expectedZ,
                  const string& testName);

//////////////////////////////////////////////////////////////////////////////
// Main
//
// Create several Vectors and test their operations
int main() {
  Vector v1, v2, v3, v4, v5;

  // test constructors
  v1 = Vector();
  VerifyVector(v1, 0.0, 0.0, 0.0, "Default ctor");
  v2 = Vector(X1, Y1, Z1);
  VerifyVector(v2, X1, Y1, Z1, "3-arg ctor");
  v3 = Vector(v2);
  VerifyVector(v3, X1, Y1, Z1, "cctor");

  // test assignment (including chaining)
  v4 = Vector(X2, Y2, Z2);
  v1 = v3 = v4;
  VerifyVector(v3, X2, Y2, Z2, "op=");
  VerifyVector(v1, X2, Y2, Z2, "op= chaining");

  // test updating assignment (including chaining)
  v1 = v3 += v2;
  VerifyVector(v3, X1 + X2, Y1 + Y2, Z1 + Z2, "op+=");
  VerifyVector(v1, X1 + X2, Y1 + Y2, Z1 + Z2, "op+= chaining");
  v1 = v3 -= v2;
  VerifyVector(v3, X2, Y2, Z2, "op-=");
  VerifyVector(v1, X2, Y2, Z2, "op-= chaining");

  // test vector scaling for both (k * v) and (v * k)
  VerifyVector(v1 * K, X2 * K, Y2 * K, Z2 * K, "op(v*k)");
  VerifyVector(K * v3, X2 * K, Y2 * K, Z2 * K, "op(k*v)");

  // test Vector addition and subtraction (including chaining)
  v5 = Vector(X3, Y3, Z3);
  VerifyVector(v2 + v4, X1 + X2, Y1 + Y2, Z1 + Z2, "op+");
  VerifyVector(v2 + v4 + v5, X1 + X2 + X3, Y1 + Y2 + Y3, Z1 + Z2 + Z3,
               "op+ chaining");
  VerifyVector(v2 - v4, X1 - X2, Y1 - Y2, Z1 - Z2, "op-");
  VerifyVector(v2 - v4 - v5, X1 - X2 - X3, Y1 - Y2 - Y3, Z1 - Z2 - Z3,
               "op- chaining");

  // test dot product
  cout << endl;
  cout << "Testing dot product..." << endl;
  cout << "Expected: " << X1 * X2 + Y1 * Y2 + Z1 * Z2 << endl;
  cout << "Actual  : " << v2 * v3 << endl;

  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
// Helper function definitions
void VerifyVector(const Vector& actual, const float& expectedX,
                  const float& expectedY, const float& expectedZ,
                  const string& testName) {
  // print an empty line
  cout << endl;

  // print the current test name
  cout << "Testing " << testName << "..." << endl;

  // print the expected result
  cout << "Expected: (" << expectedX << "," << expectedY << "," << expectedZ
       << ")" << endl;

  // print the actual result
  cout << "Actual  : " << actual << endl;
}
