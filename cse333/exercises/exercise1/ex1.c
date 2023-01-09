// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu
//
// Description: This program estimates π by adding together terms 0 through n,
// inclusive, in the Nilakantha series, and prints out that estimate to 20
// decimal places.

#include <ctype.h>    // isdigit()
#include <stdbool.h>  // boolean types and values
#include <stdio.h>    // printf()
#include <stdlib.h>   // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>   // atoi(), strlen()

#define INIT 3         // for the intial value of Pi
#define INPUT_INDEX 1  // for the index in argv[] that is used as input param
#define INPUT_SIZE 2   // for the number of inputs required

// Returns a boolean value indicating whether the given pointer points to a
// character array that represents a non-negative integer.
bool isNonNegativeInteger(char arg[]);  // function declaration

// Calculates π by adding together terms 0 through n inclusive in the Nilakantha
// series, and updates the give output pointer to point to the result.
void getPi(int n, double *result);  // function declaration

// Returns EXIT_SUCCESS and prints out the estimated Pi value iff the user
// inputs exactly one non-negative integer as the parameter. Otherwise, returns
// EXIT_FAILURE and prints out instructions according to the number and type of
// arguments.
int main(int argc, char *argv[]) {
  int n;
  double result = INIT;  // initialize the value of Pi

  if (argc != INPUT_SIZE || !isNonNegativeInteger(argv[INPUT_INDEX])) {
    if (argc < INPUT_SIZE) {
      printf("More argument needed");
    } else if (argc > INPUT_SIZE) {
      printf("Too many arguments");
    } else {
      printf("Wrong argument type");
    }

    printf(", please give me exactly one integer n (n>=0) as argument\n");

    return EXIT_FAILURE;
  }

  n = atoi(argv[INPUT_INDEX]);  // converts str input to int and updates n
  getPi(n, &result);
  printf("Our estimate of Pi is %.20f\n", result);

  return EXIT_SUCCESS;
}

// Iterates through the given character array and checks if each character is a
// number. If the character array contains only numbers (0-9), then character
// array represents a non-negative integer and the function returns true.
// Otherwise the function returns false.
bool isNonNegativeInteger(char arg[]) {
  int i;
  for (i = 0; i < strlen(arg); i++) {
    if (!isdigit(arg[i])) {
      return false;
    }
  }
  return true;
}

// Sums up the first n terms(inclusive) in the Nilakantha series and updates the
// value which the output pointer is pointing to. Determines the symbol (- or +)
// of each term by modding the index of each term by 2 (even index terms have -,
// and odd index terms have +).
void getPi(int n, double *result) {
  int i;
  for (i = 1; i < n + 1; i++) {
    if (i % 2 == 0) {
      *result -= ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
    } else {
      *result += ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
    }
  }
}
