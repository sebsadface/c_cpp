#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
 public:
  Vector();                                             // constructor
  Vector(const float x, const float y, const float z);  // constructor
  Vector(const Vector& copyMe);                         // copy constructor
  Vector& operator=(const Vector& rhs);      // assignment operator (=)
  Vector& operator+=(const Vector& rhs);     // assignment operator (+=)
  Vector& operator-=(const Vector& rhs);     // assignment operator (-=)
  float operator*(const Vector& rhs) const;  // assignment operator (*)
  ~Vector();                                 // destructor

  float get_x() const { return x_; }  // inline member function
  float get_y() const { return y_; }  // inline member function
  float get_z() const { return z_; }  // inline member function

 private:
  float x_, y_, z_;  // data members
};                   // class Vector

#endif  // VECTOR_H_
