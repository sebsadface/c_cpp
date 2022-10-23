#include <stdio.h>

int main(int argc, char** argv) {
  int val = 1;
  for (int i=0; i < 100; i++) {
    val *= val+1;
  }
  if (argc > 1) {
    printf("Your first argument was %s\n", argv[1]);
  }
  return 0;
}
