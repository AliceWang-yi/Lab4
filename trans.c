/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, r, c;
    int a0, a1, a2, a3, a4, a5, a6, a7;

    if (M == 32 && N == 32) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (r = i; r < i + 8; ++r) {
                    a0 = A[r][j];
                    a1 = A[r][j + 1];
                    a2 = A[r][j + 2];
                    a3 = A[r][j + 3];
                    a4 = A[r][j + 4];
                    a5 = A[r][j + 5];
                    a6 = A[r][j + 6];
                    a7 = A[r][j + 7];
                    B[j][r] = a0;
                    B[j + 1][r] = a1;
                    B[j + 2][r] = a2;
                    B[j + 3][r] = a3;
                    B[j + 4][r] = a4;
                    B[j + 5][r] = a5;
                    B[j + 6][r] = a6;
                    B[j + 7][r] = a7;
                }
            }
        }
    } else if (M == 64 && N == 64) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                /* Stage the top-right tile beside the top-left tile. */
                for (r = i; r < i + 4; ++r) {
                    a0 = A[r][j];
                    a1 = A[r][j + 1];
                    a2 = A[r][j + 2];
                    a3 = A[r][j + 3];
                    a4 = A[r][j + 4];
                    a5 = A[r][j + 5];
                    a6 = A[r][j + 6];
                    a7 = A[r][j + 7];
                    B[j][r] = a0;
                    B[j + 1][r] = a1;
                    B[j + 2][r] = a2;
                    B[j + 3][r] = a3;
                    B[j][r + 4] = a4;
                    B[j + 1][r + 4] = a5;
                    B[j + 2][r + 4] = a6;
                    B[j + 3][r + 4] = a7;
                }
                /* Save each staged row before its final values overwrite it. */
                for (c = j; c < j + 4; ++c) {
                    a0 = A[i + 4][c];
                    a1 = A[i + 5][c];
                    a2 = A[i + 6][c];
                    a3 = A[i + 7][c];
                    a4 = B[c][i + 4];
                    a5 = B[c][i + 5];
                    a6 = B[c][i + 6];
                    a7 = B[c][i + 7];
                    B[c][i + 4] = a0;
                    B[c][i + 5] = a1;
                    B[c][i + 6] = a2;
                    B[c][i + 7] = a3;
                    B[c + 4][i] = a4;
                    B[c + 4][i + 1] = a5;
                    B[c + 4][i + 2] = a6;
                    B[c + 4][i + 3] = a7;
                }
                for (r = i + 4; r < i + 8; ++r) {
                    a0 = A[r][j + 4];
                    a1 = A[r][j + 5];
                    a2 = A[r][j + 6];
                    a3 = A[r][j + 7];
                    B[j + 4][r] = a0;
                    B[j + 5][r] = a1;
                    B[j + 6][r] = a2;
                    B[j + 7][r] = a3;
                }
            }
        }
    } else {
        for (i = 0; i < N; i += 17)
            for (j = 0; j < M; j += 17)
                for (r = i; r < N && r < i + 17; ++r)
                    for (c = j; c < M && c < j + 17; ++c)
                        B[c][r] = A[r][c];
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
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
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

