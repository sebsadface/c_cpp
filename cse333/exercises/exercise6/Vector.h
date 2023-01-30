// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
 public:
  // Default constructor (sets x_, y_, z_ to zero)
  Vector();

  // 3-argument constructor (sets x_, y_, z_ to given arguments)
  Vector(const float x, const float y, const float z);

  // Copy constructor (sets x_, y_, z_ to given vector coordinates)
  Vector(const Vector& copyMe);

  // Overload operator (=) to elementwise assignment
  Vector& operator=(const Vector& rhs);

  // Overload operator (+=) to elementwise addition
  Vector& operator+=(const Vector& rhs);

  // Overload operator (-=) to elementwise subtraction
  Vector& operator-=(const Vector& rhs);

  // Overload operator (*) to vector dot product
  float operator*(const Vector& rhs) const;

  ~Vector();  // destructor

  float get_x() const { return x_; }  // inline member function
  float get_y() const { return y_; }  // inline member function
  float get_z() const { return z_; }  // inline member function

 private:
  float x_, y_, z_;  // data members
};                   // class Vector

#endif  // VECTOR_H_
