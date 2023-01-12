// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu
//
// Description:

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH 11

void DumpBytes(void* pData, int32_t bytelen);
void CopyAndSort(uint8_t arr_unsorted[], uint8_t arr_sorted[], int length);

int main(int argc, char* argv[]) {
  int32_t int_val = 1;
  float float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));
  DumpBytes(arr_unsorted, ARRAY_LENGTH * sizeof(uint8_t));
  CopyAndSort(arr_unsorted, arr_sorted, ARRAY_LENGTH);
  DumpBytes(arr_sorted, ARRAY_LENGTH * sizeof(uint8_t));

  return EXIT_SUCCESS;
}

void DumpBytes(void* pData, int32_t bytelen) {
  if ((pData == NULL) || (bytelen < 0)) {
    printf(
        "DumpBytes: invalid parameter. Usage: pData != NULL and bytelen >= "
        "0\n");
    exit(EXIT_FAILURE);
  }
  int i;
  printf(" The %d bytes starting at 0x%12" PRIx64 " are:", bytelen,
         (uint64_t)pData);
  for (i = 0; i < bytelen; i++) {
    printf(" %02" PRIx8, *(((uint8_t*)pData) + i));
  }
  printf("\n");
}

void CopyAndSort(uint8_t arr_unsorted[], uint8_t arr_sorted[], int length) {
  if ((arr_sorted == NULL) || (arr_unsorted == NULL) || (length < 0)) {
    printf(
        "CopyAndSort: invalid parameter. Usage: arr_unsorted != NULL, "
        "arr_sorted != NULL, and bytelen >= 0\n");
    exit(EXIT_FAILURE);
  }
  int i, j;
  uint8_t temp;
  DumpBytes(arr_unsorted, length * sizeof(uint8_t));
  for (i = 1; i < length; i++) {
    arr_sorted[i] = arr_unsorted[i];
    j = 0;
    while ((i - j) > 0 && arr_sorted[i] < arr_sorted[i - j]) {
      temp = arr_sorted[i - 1];
      arr_sorted[i - 1] = arr_sorted[i];
      arr_sorted[i] = temp;
      j++;
    }
  }
}