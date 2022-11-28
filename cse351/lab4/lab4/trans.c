/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Sebastian Liu
 * NetID(s): ll57
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);

/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M])
{
    int array[8], a, b, i, j, k, l, o;
    if (M == 32 && N == 32)
    {
        for (i = 0; i < M; i += 8)
        {
            for (j = 0; j < N; j += 8)
            {
                for (k = 0; k < 8; k++)
                {
                    for (o = 0; o < 8; o++)
                    {
                        array[o] = A[i + k][j + o];
                    }

                    for (l = 0; l < 8; l++)
                    {
                        B[j + l][i + k] = array[l];
                    }
                }
            }
        }
    }
    else
    {
        int i, j, p, q;
        int xDash, yDash; // These variables help for translating the matrix
        int temp;

        for (i = 0; i < M; i += 8)
        {
            for (j = 0; j < N; j += 8)
            {
                xDash = j - i;
                yDash = i - j;
                // A's 1,2 rows to B's 3,4
                for (p = i; p < (i + 2); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        B[p + 2 + xDash][q + yDash] = A[p][q];
                    }
                }
                // A's 3,4 rows to B's 1,2
                for (p = i + 2; p < (i + 4); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        B[p - 2 + xDash][q + yDash] = A[p][q];
                    }
                }
                // Now, re-translate in B
                // B rows 1,2 and 3,4
                for (p = i; p < (i + 2); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p + 2 + xDash][q + yDash];
                        B[p + 2 + xDash][q + yDash] = temp;
                    }
                }
                // Now transpose the first 4 by 4 quadrant of upper half
                for (p = i + xDash; p < (i + 4 + xDash); p++)
                {
                    for (q = j + yDash; q < (j + 4 + yDash); q++)
                    {
                        if ((q - (j + yDash)) < (p - (i + xDash)))
                        {
                            temp = B[p][q];
                            B[p][q] = B[q + xDash][p + yDash];
                            B[q + xDash][p + yDash] = temp;
                        }
                    }
                }
                // Now transpose the second 4 by 4 quadrant of upper half
                for (p = i + xDash; p < (i + 4 + xDash); p++)
                {
                    for (q = j + 4 + yDash; q < (j + 8 + yDash); q++)
                    {
                        if ((q - (j + 4 + yDash)) < (p - (i + xDash)))
                        {
                            temp = B[p][q];
                            B[p][q] = B[q + xDash - 4][p + yDash + 4];
                            B[q + xDash - 4][p + yDash + 4] = temp;
                        }
                    }
                }
                // Swap the second quadrant of B's 12 with 34
                // B rows 1,2 and 3,4
                for (p = i; p < (i + 2); p++)
                {
                    for (q = j + 4; q < (j + 8); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p + 2 + xDash][q + yDash];
                        B[p + 2 + xDash][q + yDash] = temp;
                    }
                }

                // Now repeat the same for the lower half
                // A's 5,6 rows to B's 7,8
                for (p = i + 4; p < (i + 6); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        B[p + 2 + xDash][q + yDash] = A[p][q];
                    }
                }
                // A's 7,8 rows to B's 5,6
                for (p = i + 6; p < (i + 8); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        B[p - 2 + xDash][q + yDash] = A[p][q];
                    }
                }
                // Now re translate in B
                // B rows 5,6 and 7,8
                for (p = i + 4; p < (i + 6); p++)
                {
                    for (q = j; q < (j + 8); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p + 2 + xDash][q + yDash];
                        B[p + 2 + xDash][q + yDash] = temp;
                    }
                }
                // Now transpose the first 4 by 4 quadrant of the lower half
                for (p = i + 4 + xDash; p < (i + 8 + xDash); p++)
                {
                    for (q = j + yDash; q < (j + 4 + yDash); q++)
                    {
                        if ((q - (j + yDash)) < (p - (i + 4 + xDash)))
                        {
                            temp = B[p][q];
                            B[p][q] = B[q + xDash + 4][p + yDash - 4];
                            B[q + xDash + 4][p + yDash - 4] = temp;
                        }
                    }
                }
                // Now transpose the second 4 by 4 quadrant of lower half
                for (p = i + 4 + xDash; p < (i + 8 + xDash); p++)
                {
                    for (q = j + 4 + yDash; q < (j + 8 + yDash); q++)
                    {
                        if ((q - (j + 4 + yDash)) < (p - (i + 4 + xDash)))
                        {
                            temp = B[p][q];
                            B[p][q] = B[q + xDash][p + yDash];
                            B[q + xDash][p + yDash] = temp;
                        }
                    }
                }
                // Swap the first quadrant of B's 56 with 78
                // B rows 5,6 and 7,8
                for (p = i + 4; p < (i + 6); p++)
                {
                    for (q = j + 0; q < (j + 4); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p + 2 + xDash][q + yDash];
                        B[p + 2 + xDash][q + yDash] = temp;
                    }
                }

                /*
                 *Consider Quadrants to be defined as :
                 * 1 2
                 * 3 4
                 */
                /*Swap the:
                 * upper half of the third quadrant  with the
                 * lower half of the second quadrant
                 */
                for (p = i + 4; p < (i + 6); p++)
                {
                    for (q = j + 0; q < (j + 4); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p - 2 + xDash][q + 4 + yDash];
                        B[p - 2 + xDash][q + 4 + yDash] = temp;
                    }
                }
                /*Now, swap the:
                 * lower half of the third quadrant with the
                 * upper half of the second quadrant
                 */
                for (p = i + 6; p < (i + 8); p++)
                {
                    for (q = j + 0; q < (j + 4); q++)
                    {
                        temp = B[p + xDash][q + yDash];
                        B[p + xDash][q + yDash] = B[p - 6 + xDash][q + 4 + yDash];
                        B[p - 6 + xDash][q + 4 + yDash] = temp;
                    }
                }
            }
        }
    }
}

// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M])
{
    int i, j, tmp;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M])
{
    int i, j;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
