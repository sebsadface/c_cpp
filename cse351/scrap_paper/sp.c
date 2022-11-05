#include <stdio.h>

int main(int argc, char *argv[])
{
    short i = 0x0A12;
    short *pairing = &i;
    char *str = "3";

    printf("The score is %d \n", ((signed int)*pairing << 20) >> 28);
    printf("The string is %s \n", str);
    printf("The address of the string is %d \n", *str);

    return 0;
}
