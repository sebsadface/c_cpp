#include <stdio.h>

int main(int argc, char* argv[]) {
  int count = 0;
  float cap = 16777218;

  for (float i = 0; i < cap; i++) {
    count++;
  }

  printf("This iterated %d times\n", count);

  return 0;
}
