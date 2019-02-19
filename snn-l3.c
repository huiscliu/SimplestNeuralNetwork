
/* a three layer neural network huiscliu */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

static double sigmoid(double x, int deriv)
{
#if 0
    if (deriv)  return 1 * (1 - x);
#else
    if (deriv)  return exp(x) / (1 + exp(x)) / (1 + exp(x));
#endif

    return 1 / (1 + exp(-x));
}

static void blas_mm4x4(double L1[4][4], double L0[4][3], double syn0[3][4])
{
    int i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            L1[i][j] = 0.;

            for (k = 0; k < 3; k++) {
                L1[i][j] += L0[i][k] * syn0[k][j];
            }
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            L1[i][j] = sigmoid(L1[i][j], 0);
        }
    }
}

static void blas_mv4x4(double L2[4], double L1[4][4], double syn1[4])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        L2[i] = 0.;

        for (j = 0; j < 4; j++) {
            L2[i] += L1[i][j] * syn1[j];
        }
    }

    for (i = 0; i < 4; i++) L2[i] = sigmoid(L2[i], 0);
}

static double abs_mean(double *L2, int n)
{
    double t = 0.;
    int i;

    assert(n >= 0);

    if (n == 0) return 0.;

    for (i = 0; i < n; i++) t += fabs(L2[i]);

    return t / n;
}

static void update_syn1(double syn1[4], double L1[4][4], double L2_delta[4])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            syn1[i] += L1[j][i] * L2_delta[j];
        }
    }
}

static void update_syn0(double syn0[3][4], double L0[4][3], double L1_delta[4][4])
{
    int i, j, k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                syn0[i][j] += L0[k][i] * L1_delta[k][j];
            }
        }
    }
}

int main(void)
{
    double X[4][3] = {{0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    double Y[4] = {0, 1, 1, 0};

    double syn0[3][4];
    double syn1[4];

    double L0[4][3];
    double L1[4][4];
    double L2[4];

    double L2_error[4];
    double L2_delta[4];

    double L1_delta[4][4];
    double L1_error[4][4];

    int i, j, k;

    /* init */
    srand(1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            syn0[i][j] = rand() * 2. / RAND_MAX - 1;
        }
    }

    for (i = 0; i < 4; i++) {
        syn1[i] = rand() * 2. / RAND_MAX - 1;
    }

    for (i = 0; i < 60000; i++) {
        /* forward propagation */
        /* L0, move fowrward to speed */
        memcpy(L0, X, sizeof(double) * 12);

        /* L1 */
        blas_mm4x4(L1, L0, syn0);

        /* L2 */
        blas_mv4x4(L2, L1, syn1);

        /* how much did we miss? */
        for (j = 0; j < 4; j++) L2_error[j] = Y[j] - L2[j];

        if (i % 10000 == 0) printf("Error: %g\n", abs_mean(L2_error, 4));

        /* in what direction is the target value?  were we really sure? if so, don't change too much. */
        for (j = 0; j < 4; j++) L2_delta[j] = L2_error[j] * sigmoid(L2[j], 1);

        /* how much did each l1 value contribute to the l2 error (according to the weights)? */
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                L1_error[j][k] = L2_delta[j] * syn1[k];
            }
        }

        /* in what direction is the target l1?  were we really sure? if so, don't change too much. */
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                L1_delta[j][k] = L1_error[j][k] * sigmoid(L1[j][k], 1);
            }
        }

        /* update syn1 and syn0 */
        update_syn1(syn1, L1, L2_delta);
        update_syn0(syn0, L0, L1_delta);
    }

    return 0;
}
