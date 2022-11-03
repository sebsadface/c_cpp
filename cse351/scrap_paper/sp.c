#include <stdio.h>

int main(int argc, char *argv[])
{
    // short *pairing;
    // *pairing = 0x0F12;
    short pairing = 0x0F12;

    printf("The score is %i \n", (signed int)((pairing & 0x0000000000000F00) >> 8));

    return 0;
}
