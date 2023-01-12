// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <inttypes.h>  // for PRIx8 and PRIx64
#include <stdint.h>    // for int32_t and int8_t
#include <stdio.h>     // for printf
#include <stdlib.h>    // for EXIT_SUCCESS / EXIT_FAILURE and exit

#define ARRAY_LENGTH 11  // the length of the given array in main

// Prints out bytelen bytes of data in hexadecimal starting from the given
// pointer.
void DumpBytes(void* pData, int32_t bytelen);

// Calls DumpBytes on the first array and copys the entries from the first array
// into the second array in non-descending sorted order.
void CopyAndSort(uint8_t arr_unsorted[], uint8_t arr_sorted[], int length);

int main(int argc, char* argv[]) {
  int32_t int_val = 1;
  float float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));

  // Since the array has 11 elements, the byte length of the array is the
  // product of the number of elements and the size of each element
  DumpBytes(arr_unsorted, ARRAY_LENGTH * sizeof(uint8_t));
  CopyAndSort(arr_unsorted, arr_sorted, ARRAY_LENGTH);
  DumpBytes(arr_sorted, ARRAY_LENGTH * sizeof(uint8_t));

  return EXIT_SUCCESS;
}

void DumpBytes(void* pData, int32_t bytelen) {
  // Make sure the given genric pointer is not NULL and the byte length is
  // non-negative
  if ((pData == NULL) || (bytelen < 0)) {
    printf(
        "DumpBytes: invalid parameter. Usage: pData != NULL and bytelen >= "
        "0\n");
    exit(EXIT_FAILURE);
  }

  int i;
  // Prints out the byte length and the address that is pointed to.
  printf(" The %d bytes starting at 0x%012" PRIx64 " are:", bytelen,
         (uint64_t)pData);

  // Prints out the pointer values byte by byte.
  for (i = 0; i < bytelen; i++) {
    printf(" %02" PRIx8, *(((uint8_t*)pData) + i));
  }
  // New line
  printf("\n");
}

void CopyAndSort(uint8_t arr_unsorted[], uint8_t arr_sorted[], int length) {
  // Make sure the given two arrays are both not NULL and they given length is
  // non-negative.
  if ((arr_sorted == NULL) || (arr_unsorted == NULL) || (length < 0)) {
    printf(
        "CopyAndSort: invalid parameter. Usage: arr_unsorted != NULL, "
        "arr_sorted != NULL, and bytelen >= 0\n");
    exit(EXIT_FAILURE);
  }

  // Call DumpBytes on the first array and its sizeof
  DumpBytes(arr_unsorted, length * sizeof(uint8_t));

  int i, j;
  uint8_t current;
  // Iterate over the first array and copy the data into the second array in
  // assending order using insertion sort.
  for (i = 0; i < length; i++) {
    current = arr_unsorted[i];
    j = i - 1;
    while (j >= 0 && arr_sorted[j] > current) {
      arr_sorted[j + 1] = arr_sorted[j];
      j--;
    }
    arr_sorted[j + 1] = current;
  }
}
