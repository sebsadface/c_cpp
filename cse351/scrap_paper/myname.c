/* myname.c - for use in CSE 351 HW25 */

#include <stdio.h>
#include <float.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 12

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: %s given_name surname\n", argv[0]);
    return 1;
  }
  char* given_name = (char*) malloc(SIZE);
  char* surname = (char*) malloc(SIZE);
  strncpy(given_name, argv[1], SIZE-1);
  strncpy(surname, argv[2], SIZE-1);

  printf("Hello, %s!\n", given_name);

  return 0;
}
