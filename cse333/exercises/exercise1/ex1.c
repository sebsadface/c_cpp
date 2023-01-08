#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INIT 3
#define READ_INDEX 1
#define MIN_INPUT_SIZE 2

bool isNonNegativeInteger(char arg[]);
void getPi(int n, double *result);

int main(int argc, char *argv[])
{
    int n;
    double result = INIT;

    if (argc < MIN_INPUT_SIZE || !isNonNegativeInteger(argv[READ_INDEX]))
    {
        printf("Please give me an integer n (n>=0)\n");

        return EXIT_FAILURE;
    }

    n = atoi(argv[READ_INDEX]);
    getPi(n, &result);
    printf("Our estimate of Pi is %.20f\n", result);

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

void getPi(int n, double *result)
{
    int i;
    for (i = 1; i < n + 1; i++)
    {
        if (i % 2 == 0)
        {
            *result -= ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
        }
        else
        {
            *result += ((double)4) / ((double)(2 * i * (2 * i + 1) * (2 * i + 2)));
        }
    }
}