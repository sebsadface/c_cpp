// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <iostream>
#include <string>

#include "Vector.h"

static const float X1 = 1;
static const float X2 = 2;
static const float Y1 = 3;
static const float Y2 = 3.1;
static const float Z1 = -2.5;
static const float Z2 = 2.7;

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using vector333::Vector;

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations
void VerifyVector(const float& expectedX, const float& expectedY,
                  const float& expectedZ, const Vector& actual,
                  const string& testName);

//////////////////////////////////////////////////////////////////////////////
// Main
//
// Create several Vectors and test their operations
int main() {
  // test constructors
  Vector v1, v2, v3, v4;

  v1 = Vector();
  VerifyVector(0.0, 0.0, 0.0, v1, "Default ctor");

  v2 = Vector(X1, Y1, Z1);
  VerifyVector(X1, Y1, Z1, v2, "3-arg ctor");

  v3 = Vector(v2);
  VerifyVector(X1, Y1, Z1, v3, "cctor");

  v4 = Vector(X2, Y2, Z2);
  v1 = v3 = v4;
  VerifyVector(X2, Y2, Z2, v3, "assignment'='");
  VerifyVector(X2, Y2, Z2, v1, "assignment'=' chaining");

  return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
// Helper function definitions
void VerifyVector(const float& expectedX, const float& expectedY,
                  const float& expectedZ, const Vector& actual,
                  const string& testName) {
  cout << endl;
  cout << "Testing " << testName << "..." << endl;
  cout << "Expected: (" << expectedX << "," << expectedY << "," << expectedZ
       << ")" << endl;
  cout << "Actual: " << actual << endl;
}