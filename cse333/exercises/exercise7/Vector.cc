// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include "Vector.h"

namespace vector333 {
//////////////////////////////////////////////////////////////////////////////
// Member functions

Vector& Vector::operator=(const Vector& rhs) {
  // Replace state of 'this' with values from rhs; do nothing if
  // self-asignment.
  if (this != &rhs) {
    coord_[0] = rhs.coord_[0];
    coord_[1] = rhs.coord_[1];
    coord_[2] = rhs.coord_[2];
  }
  // Return reference to lhs of assignment
  return *this;
}

Vector& Vector::operator+=(const Vector& rhs) {
  coord_[0] += rhs.coord_[0];
  coord_[1] += rhs.coord_[1];
  coord_[2] += rhs.coord_[2];
  return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
  coord_[0] -= rhs.coord_[0];
  coord_[1] -= rhs.coord_[1];
  coord_[2] -= rhs.coord_[2];
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
// Non-member functions

// operator* as a friend non-member function for dot product
float operator*(const Vector& v1, const Vector& v2) {
  return v1.coord_[0] * v2.coord_[0] + v1.coord_[1] * v2.coord_[1] +
         v1.coord_[2] * v2.coord_[2];
}

// operator* as a friend non-member function for scaling a vector by a floating
// point number (v * k)
Vector operator*(const Vector& v, const float k) {
  Vector ret = Vector(v);
  ret.coord_[0] *= k;
  ret.coord_[1] *= k;
  ret.coord_[2] *= k;

  return ret;
}

// operator* as a non-friend non-member function for scaling a vector by a
// floating point number (k * v)
Vector operator*(const float k, const Vector& v) { return v * k; }

// operator<< as a friend non-member function
std::ostream& operator<<(std::ostream& out, const Vector& v) {
  out << "(" << v.coord_[0] << "," << v.coord_[1] << "," << v.coord_[2] << ")";
  return out;
}

// operator+ as a non-friend non-member function
Vector operator+(const Vector& v1, const Vector& v2) {
  Vector ret = Vector(v1);
  return ret += v2;
}

// operator+ as a non-friend non-member function
Vector operator-(const Vector& v1, const Vector& v2) {
  Vector ret = Vector(v1);
  return ret -= v2;
}

}  // namespace vector333
