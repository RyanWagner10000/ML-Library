/*
 * file: main.c
 * description: file used to practice or setup a scenario for learning
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/linear_regression.h"
#include "../header/logistic_regression.h"
#include "../header/file_handling.h"
// #include "main.h"

int run_linear_regression_from_file()
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

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
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
        y_new.data[i] += b;
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

int run_linear_regression_default()
{

    // Inputs: X = [[1], [2], [3], [4]]
    // Outputs: y = [2, 4, 6, 8]
    double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    if (makeMatrix(&X, 10, 1, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
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
        y_new.data[i] += b;
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

int run_two_variate_linear_regression()
{
    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        1.0, 2.0,
        2.0, 1.0,
        3.0, 4.0,
        4.0, 3.0,
        5.0, 5.0};
    double y_vals[] = {13.0, 12.0, 23.0, 22.0, 30.0};

    DataType type = TYPE_DOUBLE;

    // 5 examples, 2 features
    Matrix X;
    if (makeMatrix(&X, 5, 2, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.02,
        .epochs = 2000,
        .lambda = 0.01,
        .regularization = REG_NONE};

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
        y_new.data[i] += b;
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

int run_three_variate_linear_regression()
{

    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        1.0, 2.0, 3.0,
        2.0, 1.0, 0.0,
        3.0, 4.0, 5.0,
        4.0, 3.0, 2.0,
        5.0, 5.0, 4.0};
    double y_vals[] = {11.0, 8.0, 21.0, 18.0, 27.0};

    DataType type = TYPE_DOUBLE;

    // 5 examples, 3 features
    Matrix X;
    if (makeMatrix(&X, 5, 3, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.02,
        .epochs = 10000,
        .lambda = 0.01,
        .regularization = REG_NONE};

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
        y_new.data[i] += b;
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

int run_logistic_regression_default()
{

    // Inputs: X = [[1], [2], [3], [4]]
    // Outputs: y = [2, 4, 6, 8]
    double x_vals[] = {0.5, 1.0, 1.5, 2.0, 3.0, 3.5, 4.0, 4.5};
    double y_vals[] = {0, 0, 0, 0, 1, 1, 1, 1};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    if (makeMatrix(&X, 8, 1, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 1000,
        .lambda = 0.01,
        .regularization = REG_NONE};

    if (train_logistic_model(&X, &y, &w, &b, &config) < 0)
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
        y_new.data[i] += b;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
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

int run_logistic_regression_2d()
{

    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        1.0, 2.0,
        2.0, 3.0, 
        3.0, 2.0,
        5.0, 2.0,
        6.0, 3.0,
        7.0, 1.5
    };
    double y_vals[] = {0, 0, 0, 1, 1, 1};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    if (makeMatrix(&X, 6, 2, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.1,
        .epochs = 10000,
        .lambda = 0.001,
        .regularization = REG_L2};

    if (train_logistic_model(&X, &y, &w, &b, &config) < 0)
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
        y_new.data[i] += b;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
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

int run_logistic_regression_3d()
{

    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        2.0 ,3.0, 0.0,
        3.0, 2.0, 1.0,
        4.0, 3.0, 2.0,
        5.0, 2.0, 3.0,
        6.0, 2.0, 4.0
    };
    double y_vals[] = {0, 0, 1, 1, 1};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    if (makeMatrix(&X, 5, 3, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    Vector y;
    if (makeVector(&y, X.rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    Vector w;
    if (makeVectorZeros(&w, X.cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.07,
        .epochs = 10000,
        .lambda = 0.01,
        .regularization = REG_NONE};

    if (train_logistic_model(&X, &y, &w, &b, &config) < 0)
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
        y_new.data[i] += b;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
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

/**
 * @brief Main function
 *
 * @param None
 *
 * @return 0 on success, -1 on failure
 */
int main(void)
{
    // printf("\n---------------Default Linear Regression---------------\n");
    // if (run_linear_regression_default() < 0)
    // {
    //     printf("Default Linear Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------2 Variate Linear Regression---------------\n");
    // if (run_two_variate_linear_regression() < 0)
    // {
    //     printf("Two Variate Linear Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------3 Variate Linear Regression---------------\n");
    // if (run_three_variate_linear_regression() < 0)
    // {
    //     printf("Three Variate Linear Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------File Linear Regression---------------\n");
    // if (run_linear_regression_from_file() < 0)
    // {
    //     printf("CSV File Linear Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------Default Logistic Regression---------------\n");
    // if (run_logistic_regression_default() < 0)
    // {
    //     printf("Default Logistic Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------2D Logistic Regression---------------\n");
    // if (run_logistic_regression_2d() < 0)
    // {
    //     printf("Default Logistic Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------3D Logistic Regression---------------\n");
    // if (run_logistic_regression_3d() < 0)
    // {
    //     printf("Default Logistic Regression test was unsuccessful.\n");
    // }

    return 0;
}