/*
 * Buggy code for CSE 333 Section 2
 * 1. Draw a memory diagram for the execution to identify errors.
 * 2. Use gdb and valgrind to identify sources of runtime, logical,
 *    and memory errors.
 * 3. Clean up the code style.
 */

#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, EXIT_SUCCESS, NULL
#include <string.h>  // strcpy, strlen

// WordCount is considered a "small" struct
typedef struct word_st {
  char* word;
  int count;
} WordCount;

// Capitalize the first letter in the word
void CapitalizeWord(WordCount* wc_ptr);

// Increment the count by 1
void IncreaseCount(WordCount wc);

// Return a new WordCount with the letters of word in reverse order and a
// count of 0.  Returns NULL on allocation failure.
WordCount ReverseWord(WordCount* wc_ptr);

int main(int argc, char* argv[]) {
  char comp[] = "computer";
  WordCount comp_count = {comp, 5};
  WordCount* comp_ptr = &comp_count;

  // expecting "1. computer, 5"
  printf("1. %s, %d\n", comp_ptr->word, comp_ptr->count);
  IncreaseCount(*comp_ptr);
  // expecting "2. computer, 6"
  printf("2. %s, %d\n", comp_ptr->word, comp_ptr->count);
  CapitalizeWord(comp_ptr);
  // expecting "3. Computer, 6"
  printf("3. %s, %d\n", comp_ptr->word, comp_ptr->count);
  *comp_ptr = ReverseWord(comp_ptr);
  // expecting "4. retupmoC, 0"
  printf("4. %s, %d\n", comp_ptr->word, comp_ptr->count);

  return EXIT_SUCCESS;
}

void IncreaseCount(WordCount wc) { wc.count += 1; }

void CapitalizeWord(WordCount* wc_ptr) { wc_ptr->word[0] &= ~0x20; }

WordCount ReverseWord(WordCount* wc_ptr) {
  WordCount* rev = (WordCount*)malloc(sizeof(WordCount));
  rev->word = (char*)malloc(strlen(wc_ptr->word) + 1);
  strcpy(rev->word, wc_ptr->word);

  char ch;
  int L = 0, R = strlen(rev->word);
  while (L < R) {
    ch = rev->word[L];
    rev->word[L] = rev->word[R] - 1;
    rev->word[R] = ch;
    L++;
    R--;
  }

  return *rev;
}
