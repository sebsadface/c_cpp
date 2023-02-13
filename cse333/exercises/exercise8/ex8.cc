#include <iostream>
#include <cstdlib>
#include <map>

using std::map;
using std::string;

template <typename T>
bool ReadValue(ifStream& in, T* const output);

template <typename T>
int compare(const T& a, const T& b);

int main(int argc, char** argv) {
  map<string, int> mp;

  return EXIT_SUCCESS;
}

template <typename T>
bool ReadValue(ifStream& in, T* const output) {
  in >> *output;

  if (in.good()) {
    return true;
  } else {
    return false;
  }
}

template <typename T>
int compare(const T& a, const T& b) {
  if (a < b) {
    return -1;
  }
  if (a > b) {
    return 1;
  }
  return 0;
}
