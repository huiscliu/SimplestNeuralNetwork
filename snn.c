
/* a two layer neural network huiscliu */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static double sigmoid(double x, int deriv)
{
    if (deriv)  return 1 * (1 - x);

    return 1 / (1 + exp(-x));
}

static void update_L1(double *L1, double L0[4][3], double syn0[3])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        L1[i] = 0;

        for (j = 0; j < 3; j++) {
            L1[i] += L0[i][j] * syn0[j];
        }
    }

    for (i = 0; i < 4; i++) L1[i] = sigmoid(L1[i], 0);
}

static void update_weight(double syn0[3], double L0[4][3], double L1_delta[4])
{
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            syn0[i] += L0[j][i] * L1_delta[j];
        }
    }
}

int main(void)
{
    double X[4][3] = {{0, 0, 1},{0, 1, 1},{1, 0, 1},{1, 1, 1}};
    double Y[4] = {0, 0, 1, 1};

    double syn0[3];
    double L0[4][3];
    double L1[4];
    double L1_error[4];
    double L1_delta[4];

    int i, j;

    /* init */
    srand(1);
    for (i = 0; i < 3; i++) syn0[i] = rand() * 1. / RAND_MAX * 2. - 1;

    for (i = 0; i < 10000; i++) {
        /* forward propagation */
        memcpy(L0, X, sizeof(double) * 12);
        update_L1(L1, L0, syn0);

        /* how much did we miss? */
        for (j = 0; j < 4; j++) L1_error[j] = Y[j] - L1[j];

        /* multiply how much we missed by the
         * slope of the sigmoid at the values in L1 */
        for (j = 0; j < 4; j++) L1_delta[j] = L1_error[j] * sigmoid(L1[j], 1);

        /* update weights */
        update_weight(syn0, L0, L1_delta);
    }

    printf("Output After Training:\n");
    for (j = 0; j < 4; j++) printf("%g\n", L1[j]);

    return 0;
}
