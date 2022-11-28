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
    int array[8], i, j, k, l, o;
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

        int n, m;      // Indecies for rows and columns in matrix
        int row, col;  // Track current row and column in matrix
        int d_val = 0; // Hold value of diagonal element found in matrix (detailed in below code)
        int diag = 0;  // Hold position of diagonal element found in matrix (detailed in below code)

        // Iterates through each column and row
        for (col = 0; col < N; col += 4)
        {
            for (row = 0; row < N; row += 4)
            {

                // For each row and column in the designated block, until end of matrix
                for (n = row; n < (row + 4); n++)
                {
                    for (m = col; m < (col + 4); m++)
                    {

                        // If row and column number do not match, transposition will occur
                        if (n != m)
                        {
                            B[m][n] = A[n][m];
                            // Else, row and column number are same and element in matrix is defined as a diagonal
                        }
                        else
                        {

                            // Assign diagonal element to a temporary variable
                            // This saves an individual cache miss on each run through the matrix where the columns and rows still match up
                            diag = n;
                            d_val = A[n][m];
                        }
                    }
                    // If row and column are same, element is defined as a diagonal and our temporarily saved element is assigned
                    if (row == col)
                    {
                        B[diag][diag] = d_val;
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
