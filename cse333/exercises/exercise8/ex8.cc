#include <iostream>
#include <cstdlib>
#include <map>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::pair;
using std::string;

template <typename T>
bool ReadValue(ifstream& in, T* const output);

int main(int argc, char** argv) {
  map<string, int> mp;
  map<string, int>::iterator it;
  ifstream ifs;
  string* const str = new string;
  ifs.open(argv[1], ifstream::in);
  while (ReadValue<string>(ifs, str)) {
    it = mp.find(*str);
    if (it != mp.end()) {
      it->second++;
    } else {
      mp.insert(pair<string, int>(*str, 1));
    }
  }

  for (auto pair : mp) {
    cout << pair.first << " " << pair.second << endl;
  }

  delete str;
  return EXIT_SUCCESS;
}

template <typename T>
bool ReadValue(ifstream& in, T* const output) {
  in >> *output;

  if (in.good()) {
    return true;
  } else {
    return false;
  }
}
