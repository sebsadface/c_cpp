#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// length of string
#define LEN 10

// string
char* str = "algorithms";

// Helper function that shuffles an array in place.
// Assumes small values (fit in char) and uses a globally-defined length.
void shuffle(int* array);

// Helper function that returns a random integer in the specified range [min, max].
int random(int min, int max);

// Prints the characters of the string in random order by cycling through the
// values in the shuffled array starting at the specified index.
// shuffle_print_r is called to do the actual printing.
void scramble(int* array, int start);

// A recursive function that cycles through the elements of an array starting
// at the specified offset and prints a character of the string.
void shuffle_print_r(int* array, int index, int offset);

// Sets up the shuffled array and calls scramble.
int main() {
  // seed random calls
  srand(time(NULL));

  // create array of indices and initialize it in order
  int rand_ar[LEN];
  for (int i = 0; i < LEN; i++)
    rand_ar[i] = i;
  // shuffle array
  shuffle(rand_ar);
  // call scramble with random starting index
  scramble(rand_ar, random(0, LEN-1));

  return EXIT_SUCCESS;
}

void shuffle(int* array) {
  for (int i = 0; i < LEN-1; i++) {
    int j = random(i, LEN-1);
    int temp = array[j];
    array[j] = array[i];
    array[i] = temp;
  }
}

int random(int min, int max) {
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void scramble(int* array, int start) {
  shuffle_print_r(array, 0, start);
  printf("\n");
}

void shuffle_print_r(int* array, int index, int offset) {
  if (index >= LEN)
    return;
  else {
    printf("%c", str[array[(index + offset) % LEN]]);
    shuffle_print_r(array, index+1, offset);
  }
}

