/*
 * mul.c - Matrix multiply C = A * B
 *
 * Each multiply function must have a prototype of the form:
 * void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);
 *
 * A multiply function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);

/*
 * mul_submit - This is the solution multiply function that you will be
 *     graded on for Part C of the assignment. Do not change the
 *     description string "Multiply submission", as the driver searches
 *     for that string to identify the multiply function to be graded.
 *     C starts out holding garbage, so every element of C must be written.
 */
char mul_submit_desc[] = "Multiply submission";
void mul_submit(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, kk;
    int a, t0, t1, t2, t3, t4, t5, t6, t7;

    if (M == 32 && N == 32) {
        for (j = 0; j < N; j += 8) {
            for (kk = 0; kk < M; kk += 8) {
                for (i = 0; i < N; ++i) {
                    /* Only later panels can read a partial sum from C. */
                    if (kk == 0) {
                        t0 = t1 = t2 = t3 = 0;
                        t4 = t5 = t6 = t7 = 0;
                    } else {
                        t0 = C[i][j];
                        t1 = C[i][j + 1];
                        t2 = C[i][j + 2];
                        t3 = C[i][j + 3];
                        t4 = C[i][j + 4];
                        t5 = C[i][j + 5];
                        t6 = C[i][j + 6];
                        t7 = C[i][j + 7];
                    }
                    for (k = kk; k < kk + 8; ++k) {
                        a = A[i][k];
                        t0 += a * B[k][j];
                        t1 += a * B[k][j + 1];
                        t2 += a * B[k][j + 2];
                        t3 += a * B[k][j + 3];
                        t4 += a * B[k][j + 4];
                        t5 += a * B[k][j + 5];
                        t6 += a * B[k][j + 6];
                        t7 += a * B[k][j + 7];
                    }
                    C[i][j] = t0;
                    C[i][j + 1] = t1;
                    C[i][j + 2] = t2;
                    C[i][j + 3] = t3;
                    C[i][j + 4] = t4;
                    C[i][j + 5] = t5;
                    C[i][j + 6] = t6;
                    C[i][j + 7] = t7;
                }
            }
        }
    } else {
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j) {
                t0 = 0;
                for (k = 0; k < M; ++k)
                    t0 += A[i][k] * B[k][j];
                C[i][j] = t0;
            }
    }
}

/*
 * mul - A simple baseline multiply function, not optimized for the cache.
 */
char mul_desc[] = "Simple i-j-k multiply";
void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tmp = 0;
            for (k = 0; k < M; k++)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your multiply functions
 *     with the driver. At runtime, the driver will evaluate each of the
 *     registered functions and summarize their performance. This is a
 *     handy way to experiment with different multiply strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerMulFunction(mul_submit, mul_submit_desc);

    /* Register any additional multiply functions */
    registerMulFunction(mul, mul_desc);
}

/*
 * is_mul - This helper function checks if C is the product of A and B.
 *     You can check the correctness of your multiply by calling it
 *     before returning from the multiply function.
 */
int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, num;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            num = 0;
            for (k = 0; k < M; k++)
                num += A[i][k] * B[k][j];
            if (num != C[i][j])
                return 0;
        }
    }
    return 1;
}
