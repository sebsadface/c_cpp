#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Accepts an array of 32-bit unsigned integers and a length
// Reverses the elements of the array in place
// Returns nothing(void)
void ee1(uint32_t arr[], uint32_t dst[], int len);

// determines and prints out whether
// the computer it is running on is little endian or big endian
void ee3();

void printArray(uint32_t arr[], int len);

int main(int argc, char *rgv[])
{
    int len = 5;
    uint32_t arr[] = {1, 2, 3, 4, 5};
    uint32_t dst[len];

    ee1(arr, dst, len);
    printArray(dst, len);
    return EXIT_SUCCESS;
}

void printArray(uint32_t arr[], int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void ee1(uint32_t arr[], uint32_t dst[], int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        dst[len - i - 1] = arr[i];
    }
}
