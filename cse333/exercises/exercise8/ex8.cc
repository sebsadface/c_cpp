// Copyright © 2023 Sebatian Liu.
//
// Name : Sebastian Liu
// CSE Email Address : ll57 @cs.washington.edu

#include <iostream>
#include <cstdlib>  // for EXIT_SUCCESS and EXIT_FAILURE
#include <map>
#include <fstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::pair;
using std::string;

void Usage(string message);

template <typename T>
bool ReadValue(ifstream& in, T* const output);

int main(int argc, char** argv) {
  if (argc != 2) {
    Usage("Invalid number of arguments.");
  }

  map<string, int> mp;
  map<string, int>::iterator it;
  ifstream ifs;
  string* const str = new string;

  ifs.open(argv[1], ifstream::in);
  if (!ifs.is_open()) {
    Usage("Open file failed.");
  }

  while (ReadValue<string>(ifs, str)) {
    it = mp.find(*str);
    if (it != mp.end()) {
      it->second++;
    } else {
      mp[*str] = 1;
    }
  }

  for (pair<string, int> element : mp) {
    cout << element.first << " " << element.second << endl;
  }

  delete str;
  return EXIT_SUCCESS;
}

template <typename T>
bool ReadValue(ifstream& in, T* const output) {
  in >> *output;

  return in.good();
}

void Usage(string message) {
  cerr << message << " "
       << "Usage: ./ex8 <filename>" << endl;
  exit(EXIT_FAILURE);
}
