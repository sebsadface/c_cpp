#include <stdio.h>

int main(int argc, char *argv[])
{
    short *pairing;
    *pairing = 0x0F12;

    printf("The score is %d \n", ((signed int)*pairing << 20) >> 28);

    return 0;
}
