// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <iostream>  // for cout, endl, cin, cerr
#include <cstdlib>   // for EXIT_SUCCESS, EXIT_FAILURE

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  int64_t num = 0;
  int i;

  cout << "Which positive integer would you like me to factorize? ";

  // Getting user input from stdin. If user's input starts with a number, "-" or
  // "+", num will store all the numbers until we encounter a non-number
  // character. Otherwise, num will evaluate to 0.
  cin >> num;

  // Returns print out error message and exit, if num is not strictly greater
  // than zero.
  if (num <= 0) {
    cerr << "Error: Invalid input. Input n must be stricly greater than zero "
            "(n > 0)."
         << endl;
    return EXIT_FAILURE;
  }

  // 1 is a fractor for all positive integers.
  cout << 1;

  // Get the rest of the fractors of the given positive integer
  for (i = 2; i <= num; i++) {
    if (num % i == 0) {
      cout << " " << i;
    }
  }
  // Writes newline to the stdout.
  cout << endl;

  return EXIT_SUCCESS;
}
