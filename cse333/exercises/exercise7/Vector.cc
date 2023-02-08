// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include "Vector.h"

namespace vector333 {
//////////////////////////////////////////////////////////////////////////////
// Member functions

Vector& Vector::operator=(const Vector& rhs) {
  if (this != &rhs) {
    coord_[0] = rhs.coord_[0];
    coord_[1] = rhs.coord_[1];
    coord_[2] = rhs.coord_[2];
  }

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
float operator*(const Vector& v1, const Vector& v2) {
  return v1.coord_[0] * v2.coord_[0] + v1.coord_[1] * v2.coord_[1] +
         v1.coord_[2] * v2.coord_[2];
}

Vector operator*(const Vector& v, const float f) {
  Vector ret = Vector(v);
  ret.coord_[0] *= f;
  ret.coord_[1] *= f;
  ret.coord_[2] *= f;

  return ret;
}

std::ostream& operator<<(std::ostream& out, const Vector& v) {
  out << "(" << v.coord_[0] << "," << v.coord_[1] << "," << v.coord_[2] << ")";
  return out;
}

Vector operator+(const Vector& v1, const Vector& v2) {
  Vector ret = Vector(v1);
  return ret += v2;
}

Vector operator-(const Vector& v1, const Vector& v2) {
  Vector ret = Vector(v1);
  return ret -= v2;
}

}  // namespace vector333
