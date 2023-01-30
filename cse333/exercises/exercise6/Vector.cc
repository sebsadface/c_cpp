#include "Vector.h"

Vector::Vector() : x_(0), y_(0), z_(0) {}

Vector::Vector(const float x, const float y, const float z)
    : x_(x), y_(y), z_(z) {}

Vector::Vector(const Vector& copyMe)
    : x_(copyMe.x_), y_(copyMe.y_), z_(copyMe.z_) {}

Vector& Vector::operator=(const Vector& rhs) {
  if (this != &rhs) {
    x_ = rhs.x_;
    y_ = rhs.y_;
    z_ = rhs.z_;
  }

  return *this;
}

Vector& Vector::operator+=(const Vector& rhs) {
  if (this != &rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
  }

  return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
  if (this != &rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
  }

  return *this;
}

float Vector::operator*(const Vector& rhs) const {
  return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
}

Vector::~Vector() {}