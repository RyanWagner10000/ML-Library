/*
 * file: main.c
 * description: file used to practice or setup a scenario for learning
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/linear_regression.h"
#include "../header/file_handling.h"
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

    // const char* filename = "../datasets/Salary_dataset.csv";
    // Matrix m;
    // int header = 0;
    // int status = loadCSVtoMatrix(filename, header, &m);

    // Inputs: X = [[1], [2], [3], [4]]
    // Outputs: y = [2, 4, 6, 8]
    double x_vals[] = {1, 2, 3, 4};
    double y_vals[] = {2, 4, 6, 8};

    // 4 examples, 1 feature
    Matrix X;
    if (makeMatrix(&X, 4, 1, &x_vals, TYPE_DOUBLE) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }
    else
    {
        printf("Initialized Matrix X\n");
    }

    Vector y;
    if (makeVector(&y, 4, &y_vals, TYPE_DOUBLE) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }
    else
    {
        printf("Initialized Vector y\n");
    }

    Vector w;
    if (makeVectorZeros(&w, 1, TYPE_DOUBLE) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }
    else
    {
        printf("Initialized Vector w\n");
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 100,
        .lambda = 0.001,
        .regularization = REG_L2};

    if (train_linear_model(&X, &y, &w, &b, &config) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor: ");
    printVector(&w);

    Vector y_new;
    if (makeVectorZeros(&y_new, 4, TYPE_DOUBLE) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(&X, &w, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    printf("y_new = ");
    printVector(&y_new);

    freeMatrix(&X);
    freeVector(&y);
    freeVector(&w);
    freeVector(&y_new);

    return 0;
}