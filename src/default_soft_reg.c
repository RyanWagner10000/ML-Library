/*
 * file: default_soft_reg.c
 * description: file used to demonstrate a basic softmax regression
 * author: Ryan Wagner
 * date: August 30, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/regression.h"
#include "../header/file_handling.h"
#include "../header/eval_matrics.h"

int run_softmax_regression_default()
{
    // Inputs: X
    // Outputs: y

    double x_vals[] = {
        1.0, 0.5, 0.3, 0.0, 1.2,
        0.9, 0.4, 0.2, 0.1, 1.1,
        1.2, 0.6, 0.3, 0.2, 1.3,
        0.8, 0.3, 0.1, 0.0, 1.0};
    double y_vals[] = {0, 1, 2, 0};

    DataType type = TYPE_DOUBLE;

    Model softmax_model;
    if (initModel(&softmax_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    softmax_model.type = SOFTMAX_REGRESSION;

    if (makeMatrix(softmax_model.X, 4, 5, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeMatrix(softmax_model.y, softmax_model.X->rows, 1, &y_vals, type) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    softmax_model.classes = 3;
    softmax_model.batch_size = 4;

    softmax_model.func = SOFTMAX;

    softmax_model.config.learning_rate = 0.1;
    softmax_model.config.epochs = 50000;
    softmax_model.config.lambda = 0.01;
    softmax_model.config.regularization = REG_NONE;

    if (trainModel(&softmax_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = \n");
    printMatrix(*softmax_model.weights);
    printf("Bias factor = \n");
    printVector(*softmax_model.bias);

    Matrix y_new = {0};
    if (makeMatrixZeros(&y_new, softmax_model.y->rows, softmax_model.y->cols) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (mat_mul(*softmax_model.X, *softmax_model.weights, &y_new) < 0)
    {
        printf("Error with matrix multiplication.\n");
        return -1;
    }

    // Apply bias to each class respectively
    for (int r = 0; r < y_new.rows; ++r)
    {
        for (int c = 0; c < y_new.cols; ++c)
        {
            int index = r * y_new.cols + c;
            y_new.data[index] += softmax_model.bias->data[c];
        }
    }

    // // Make Vector to hold one row of matrix
    Vector temp_row = {0};
    if (makeVectorZeros(&temp_row, y_new.cols) < 0)
    {
        printf("Making temp row variable was unsuccessful.\n");
        return -1;
    }

    // Apply Softmax Function
    for (int r = 0; r < y_new.rows; ++r)
    {
        getRowMatrix(y_new, r, &temp_row);

        for (int j = 0; j < y_new.cols; ++j)
        {
            int index = r * y_new.cols + j;
            if (softmax(y_new.data[index], temp_row, &y_new.data[index]) < 0)
            {
                printf("Softmax function was unsuccessful when computing logits.\n");
                freeVector(&temp_row);
                return -1;
            }
        }
    }
    freeVector(&temp_row);

    printf("y_old = \n");
    printMatrix(*softmax_model.y);
    printf("y_new = \n");
    printMatrix(y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, y_new, softmax_model.type) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(softmax_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    freeModel(&softmax_model);
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
    printf("\n---------------Softmax Regression---------------\n");
    if (run_softmax_regression_default() < 0)
    {
        printf("Default Softmax Regression test was unsuccessful.\n");
    }
    return 0;
}