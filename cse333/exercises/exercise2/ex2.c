// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu
//
// Description:

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void DumpBytes(void* pData, int32_t bytelen);
void CopyAndSort(uint8_t arr_unsorted[], uint8_t arr_sorted[], int length);

int main(int argc, char* argv[]) {
  int32_t int_val = 1;
  float float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));
  DumpBytes(arr_unsorted, _____);
  CopyAndSort(arr_unsorted, arr_sorted, _____);
  DumpBytes(arr_sorted, _____);

  return EXIT_SUCCESS;
}

void DumpBytes(void* pData, int32_t bytelen) {}

void CopyAndSort(uint8_t arr_sorted[], uint8_t arr_unsorted[], int length) {}