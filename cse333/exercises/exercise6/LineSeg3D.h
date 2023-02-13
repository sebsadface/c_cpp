#include "Vector.h"

class LineSeg3D {
 public:
  // method declarations go here
  LineSeg3D(const Vector& end) : start_(Vector()), end_(Vector(end)) {}

 private:
  Vector start_, end_;
};