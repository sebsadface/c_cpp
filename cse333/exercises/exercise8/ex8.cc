// Copyright © 2023 Sebatian Liu.
//
// Name : Sebastian Liu
// CSE Email Address : ll57 @cs.washington.edu

#include <iostream>  // for cout, endl, cerr
#include <string>    // for string
#include <cstdlib>   // for EXIT_SUCCESS, EXIT_FAILURE
#include <map>       // for map, pair
#include <fstream>   // for ifstream

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::pair;
using std::string;

// Prints a usage message and exits the program.
void Usage(string message);

// Reads a value of type T from the input stream in and stores it in.
template <typename T>
bool ReadValue(ifstream& in, T* const output);

// Reads a file and counts the number of times each word appears.
// Prints the final list of the strings, sorted alphabetically, and the
// number of times each word appears in the file.
int main(int argc, char** argv) {
  // Check the number of arguments.
  if (argc != 2) {
    Usage("Invalid number of arguments.");
  }

  map<string, int> mp;
  map<string, int>::iterator it;
  ifstream ifs;
  string* const str = new string;

  // Open the file with the given file name in command line argument.
  ifs.open(argv[1], ifstream::in);

  // Check if the file is open.
  if (!ifs.is_open()) {
    Usage("Open file failed.");
  }

  // Read the file and count the number of times each word appears.
  while (ReadValue<string>(ifs, str)) {
    it = mp.find(*str);
    if (it != mp.end()) {
      it->second++;
    } else {
      mp[*str] = 1;
    }
  }

  // Print the final list of the strings, sorted alphabetically, and the
  // number of times each word appears in the file. (Because the map
  // sorts the keys automatically so we don't need to additional sortings)
  for (pair<string, int> element : mp) {
    cout << element.first << " " << element.second << endl;
  }

  delete str;
  return EXIT_SUCCESS;
}

template <typename T>
bool ReadValue(ifstream& in, T* const output) {
  // Read a value from the input stream in and parse it into type T, store it
  // into output.
  char c;
  while (in.get(c) && !isspace(c)) {
    c >> *output;
  }
  // Check if the input stream is in a good state (none of eofbit, failbit and
  // badbit is set).
  return in.good();
}

void Usage(string message) {
  cerr << message << " "
       << "Usage: ./ex8 <filename>" << endl;
  exit(EXIT_FAILURE);
}
