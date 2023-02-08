// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

using std::ostream;

namespace vector333 {
// A Vector represents a vector in 3d-space.
class Vector {
 public:
  // Default Constructor: construct the vector (0,0,0)
  Vector() : coord_(new float[3]{0.0, 0.0, 0.0}) {}

  // Construct the vector (x,y,z)
  Vector(const float x, const float y, const float z)
      : coord_(new float[3]{x, y, z}) {}

  // Copy constructor
  Vector(const Vector& v)
      : coord_(new float[3]{v.coord_[0], v.coord_[1], v.coord_[2]}) {}

  // Destructor
  ~Vector() { delete[] coord_; }

  // Assignment operator
  Vector& operator=(const Vector& rhs);

  // Updating assignment
  Vector& operator+=(const Vector& rhs);
  Vector& operator-=(const Vector& rhs);

  // Dot product
  friend float operator*(const Vector& v1, const Vector& v2);

  // Vector scaling (v * k)
  friend Vector operator*(const Vector& v, const float k);

  // Stream output as (a,b,c)
  friend ostream& operator<<(ostream& out, const Vector& v);

 private:
  // The representation of a Vector is a three-element floating-point array
  // giving the x, y, and z magnitudes.
  float* coord_;
};  // class Vector

// Vector scaling (k * v)
Vector operator*(const float k, const Vector& v);

// Vector addition and subtraction operations
Vector operator+(const Vector& v1, const Vector& v2);
Vector operator-(const Vector& v1, const Vector& v2);

}  // namespace vector333

#endif  // VECTOR_H_
