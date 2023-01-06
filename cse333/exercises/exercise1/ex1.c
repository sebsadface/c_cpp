#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool isNonNegativeInteger(char arg[]);
double getPi(int n);

int main(int argc, char *argv[])
{
    int n;

    if (argc < 2 || !isNonNegativeInteger(argv[1]))
    {
        printf("Please give me an integer n (n>=0)");

        return EXIT_FAILURE;
    }

    n = atoi(argv[1]);
    printf("Our estimate of Pi is %f \n", getPi(n));

    return EXIT_SUCCESS;
}

bool isNonNegativeInteger(char arg[])
{
    int i;
    for (i = 0; i < strlen(arg); i++)
    {
        if (!isdigit(arg[i]))
        {
            return false;
        }
    }
    return true;
}

double getPi(int n)
{
    int i;
    double result = 0.0;
    for (i = 0; i < n + 1; i++)
    {
        if (i % 2 == 0)
        {
            result += ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
        }
        else
        {
            result -= ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
        }
    }
    return result;
}