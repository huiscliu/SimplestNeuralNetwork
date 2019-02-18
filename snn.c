
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static double sigmoid(double x, int deriv)
{
    if (deriv)  return 1 * (1 - x);

    return 1 / (1 + exp(-x));
}

static void get_l1(double *l1, double l0[4][3], double syn0[3])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        l1[i] = 0;

        for (j = 0; j < 3; j++) {
            l1[i] += l0[i][j] * syn0[j];
        }
    }

    for (i = 0; i < 4; i++) l1[i] = sigmoid(l1[i], 0);
}

static void update_weight(double syn0[3], double l0[4][3], double l1_delta[4])
{
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            syn0[i] += l0[j][i] * l1_delta[j];
        }
    }
}

int main(void)
{
    double X[4][3] = {{0, 0, 1},{0, 1, 1},{1, 0, 1},{1, 1, 1}};
    double y[4] = {0, 0, 1, 1};
    double syn0[3];
    double l0[4][3];
    double l1[4];
    double l1_error[4];
    double l1_delta[4];

    int i, j;

    /* init */
    srand(1);
    for (i = 0; i < 3; i++) syn0[i] = rand() * 1. / RAND_MAX * 2. - 1;

    for (i = 0; i < 10000; i++) {
        /* forward propagation */
        memcpy(l0, X, sizeof(double) * 12);
        get_l1(l1, l0, syn0);

        /* how much did we miss? */
        for (j = 0; j < 4; j++) l1_error[j] = y[j] - l1[j];

        /* multiply how much we missed by the
         * slope of the sigmoid at the values in l1 */
        for (j = 0; j < 4; j++) l1_delta[j] = l1_error[j] * sigmoid(l1[j], 1);

        /* update weights */
        update_weight(syn0, l0, l1_delta);
    }

    printf("Output After Training:\n");
    for (j = 0; j < 4; j++) printf("%g\n", l1[j]);

    return 0;
}
