/*
 * file: main.c
 * description: file used to practice or setup a scenario for learning
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/linear_regression.h"
// #include "main.h"

/**
 * @brief Main function to run a linear regression
 *
 * @param None
 *
 * @return 0 on success, -1 on failure
 */
int main(void)
{

    // Inputs: X = [[1], [2], [3], [4]]
    // Outputs: y = [2, 4, 6, 8]
    double x_vals[] = {1, 2, 3, 4};
    double y_vals[] = {2, 4, 6, 8};

    Matrix X = makeMatrix(x_vals, 4, 1); // 4 examples, 1 feature
    Vector y = makeVector(y_vals, 4);

    Vector w = {malloc(sizeof(double)), 1};
    w.data[0] = 0.0;
    double b = 0.0;
    double lambda = 0.01;
    bool regularize = true;

    if (train_linear_model(&X, &y, &w, &b, 0.01, 200, lambda, regularize) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor: ");
    printVector(&w);

    Vector y_new = {malloc(sizeof(double)), 4};

    if (matvec_mult(&X, &w, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    printf("y_new = ");
    printVector(&y_new);

    return 0;
}