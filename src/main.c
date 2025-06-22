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
    int status = 0;

    // Extract the data from the CSV file into a Matrix
    const char *filename = "../datasets/Salary_dataset.csv";
    Matrix X;
    status = makeMatrix(&X, 0, 0, NULL, TYPE_DOUBLE);

    int header = 1;
    status = loadCSVtoMatrix(filename, header, &X);

    // Inputs: X = [[1.2], [1.4], [1.6], [...]]
    // Outputs: y = [39344.00, 46206.00, 37732.00, ...]

    // Delete the first column of the data, it's not needed
    if (deleteColMatrix(&X, 0) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    // Copy the y-values from the matrix into a vector
    Vector y;
    if (makeVectorZeros(&y, X.rows))
    {
        printf("Could not make y-values Vector.\n");
        return -1;
    }
    if (getColMatrix(&X, 1, &y) < 0)
    {
        printf("Couldn't get column from matrix.\n");
        return -1;
    }

    // Delete y-values from Matrix
    if (deleteColMatrix(&X, 1) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    // printf("\nMatrix m =\n");
    // printMatrix(&X);

    // printf("\nVector v =\n");
    // printVector(&y);

    // // Inputs: X = [[1], [2], [3], [4]]
    // // Outputs: y = [2, 4, 6, 8]
    // double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    // DataType type = TYPE_DOUBLE;

    // // 4 examples, 1 feature
    // Matrix X;
    // if (makeMatrix(&X, 10, 1, &x_vals, type) < 0)
    // {
    //     printf("Problem initializing input Matrix\n");
    //     return -1;
    // }

    // Vector y;
    // if (makeVector(&y, X.rows, &y_vals, type) < 0)
    // {
    //     printf("Problem initializing output Vector\n");
    //     return -1;
    // }

    Vector w;
    if (makeVectorZeros(&w, 1) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 1000,
        .lambda = 0.01,
        .regularization = REG_L2};

    if (train_linear_model(&X, &y, &w, &b, &config) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(&w);
    printf("Bias factor = %.2lf\n", b);

    Vector y_new;
    if (makeVectorZeros(&y_new, y.size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(&X, &w, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] +=  b;
    }

    // printf("X = \n");
    // printMatrix(&X);
    printf("y_old = \n");
    printVector(&y);
    printf("y_new = \n");
    printVector(&y_new);

    freeMatrix(&X);
    freeVector(&y);
    freeVector(&w);
    freeVector(&y_new);

    return 0;
}