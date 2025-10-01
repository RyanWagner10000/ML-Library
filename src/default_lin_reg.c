/*
 * file: default_lin_reg.c
 * description: file used to demonstrate a basic linear regression
 * author: Ryan Wagner
 * date: August 30, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/regression.h"
#include "../header/eval_matrics.h"

int run_linear_regression_default()
{
    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        1.0, 0.5, 0.3, 0.0, 1.2,
        0.9, 0.4, 0.2, 0.1, 1.1,
        1.2, 0.6, 0.3, 0.2, 1.3,
        0.8, 0.3, 0.1, 0.0, 1.0};
    double y_vals[] = {1.0, 2.0, 3.0, 4.0};

    DataType type = TYPE_DOUBLE;

    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR_REGRESSION;

    if (makeMatrix(linear_model.X, 4, 5, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix.\n");
        return -1;
    }

    if (makeMatrix(linear_model.y, linear_model.X->rows, 1, &y_vals, type) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    linear_model.classes = 1;
    linear_model.batch_size = 2;

    linear_model.func = ACT_NONE;

    linear_model.config.learning_rate = 0.01;
    linear_model.config.epochs = 80000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_NONE;

    if (trainModel(&linear_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = \n");
    printMatrix(*linear_model.weights);
    printf("Bias factor = \n");
    printVector(*linear_model.bias);

    Matrix y_new = {0};
    if (makeMatrixZeros(&y_new, linear_model.y->rows, linear_model.y->cols) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (mat_mul(*linear_model.X, *linear_model.weights, &y_new) < 0)
    {
        printf("Error with matrix multiplication.\n");
        return -1;
    }

    if (mat_add(y_new, linear_model.bias->data[0], &y_new) < 0)
    {
        printf("Error with matrix addition.\n");
        return -1;
    }

    printf("y_old = \n");
    printMatrix(*linear_model.y);
    printf("y_new = \n");
    printMatrix(y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, y_new, linear_model.type) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    // if (calculateAllMetrics(linear_model, &eval_metrics) < 0)
    // {
    //     printf("Calculating all performance metrics failed.\n");
    //     return -1;
    // }

    freeModel(&linear_model);
    freeMatrix(&y_new);
    freeEvalMetrics(&eval_metrics);

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
    printf("\n---------------Default Linear Regression---------------\n");
    if (run_linear_regression_default() < 0)
    {
        printf("Default Linear Regression test was unsuccessful.\n");
    }

    return 0;
}