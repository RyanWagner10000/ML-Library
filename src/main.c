/*
 * file: main.c
 * description: file used to practice or setup a scenario for learning
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */
#include "../header/math_funcs.h"
#include "../header/regression.h"
#include "../header/file_handling.h"
#include "../header/eval_matrics.h"

int run_linear_regression_from_file()
{
    DataType type = TYPE_DOUBLE;

    // Extract the data from the CSV file into a Matrix
    const char *filename = "../datasets/Salary_dataset.csv";
    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR_REGRESSION;
    if (makeMatrix(linear_model.X, 0, 0, NULL, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (loadCSVtoMatrix(filename, 1, linear_model.X) < 0)
    {
        printf("Reading CSV to Matrix was unsuccessful.\n");
        return -1;
    }

    // Inputs: X = [[1.2], [1.4], [1.6], [...]]
    // Outputs: y = [39344.00, 46206.00, 37732.00, ...]

    // Delete the first column of the data, it's not needed
    if (deleteColMatrix(linear_model.X, 0) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    // Copy the y-values from the matrix into a vector
    if (makeMatrixZeros(linear_model.y, linear_model.X->rows, 1) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    if (getColMatrix(*linear_model.X, 1, linear_model.y) < 0)
    {
        printf("Couldn't get column from matrix.\n");
        return -1;
    }

    linear_model.classes = 1;
    linear_model.batch_size = 5;

    // Delete y-values from Matrix
    if (deleteColMatrix(linear_model.X, 1) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    linear_model.func = ACT_NONE;

    linear_model.config.learning_rate = 0.01;
    linear_model.config.epochs = 2000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_L2;

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

    // printf("y_old = \n");
    // printMatrix(*linear_model.y);
    // printf("y_new = \n");
    // printMatrix(y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, y_new, linear_model.type) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeMatrix(&y_new);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

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

    if (makeMatrixZeros(linear_model.logits, linear_model.X->rows, linear_model.classes) < 0)
    {
        printf("Problem initializing logits Matrix\n");
        return -1;
    }

    if (makeMatrixZeros(linear_model.weights, linear_model.X->cols, linear_model.classes) < 0)
    {
        printf("Problem initializing weight Matrix\n");
        return -1;
    }

    if (makeVectorZeros(linear_model.bias, linear_model.classes) < 0)
    {
        printf("Problem initializing bias Matrix\n");
        return -1;
    }
    linear_model.func = ACT_NONE;

    linear_model.config.learning_rate = 0.1;
    linear_model.config.epochs = 50000;
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

    if (calculateAllMetrics(linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeMatrix(&y_new);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

int run_logistic_regression_default()
{
    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        1.0, 0.5, 0.3, 0.0, 1.2,
        0.9, 0.4, 0.2, 0.1, 1.1,
        1.2, 0.6, 0.3, 0.2, 1.3,
        0.8, 0.3, 0.1, 0.0, 1.0};
    double y_vals[] = {0, 0, 1, 1};

    DataType type = TYPE_DOUBLE;

    Model logistic_model;
    if (initModel(&logistic_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    logistic_model.type = LOGISTIC_REGRESSION;

    if (makeMatrix(logistic_model.X, 4, 5, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeMatrix(logistic_model.y, logistic_model.X->rows, 1, &y_vals, type) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    logistic_model.classes = 1;

    if (makeMatrixZeros(logistic_model.logits, logistic_model.X->rows, logistic_model.classes) < 0)
    {
        printf("Problem initializing logits Matrix\n");
        return -1;
    }

    if (makeMatrixZeros(logistic_model.weights, logistic_model.X->cols, logistic_model.classes) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    if (makeVectorZeros(logistic_model.bias, logistic_model.classes) < 0)
    {
        printf("Problem initializing bias Matrix\n");
        return -1;
    }
    logistic_model.func = SIGMOID;

    logistic_model.config.learning_rate = 0.1;
    logistic_model.config.epochs = 100000;
    logistic_model.config.lambda = 0.01;
    logistic_model.config.regularization = REG_NONE;

    if (trainModel(&logistic_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = \n");
    printMatrix(*logistic_model.weights);
    printf("Bias factor = \n");
    printVector(*logistic_model.bias);

    Matrix y_new = {0};
    if (makeMatrixZeros(&y_new, logistic_model.y->rows, logistic_model.y->cols) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (mat_mul(*logistic_model.X, *logistic_model.weights, &y_new) < 0)
    {
        printf("Error with matrix multiplication.\n");
        return -1;
    }

    if (mat_add(y_new, logistic_model.bias->data[0], &y_new) < 0)
    {
        printf("Error with matrix addition.\n");
        return -1;
    }

    if (applyToMatrix(&y_new, SIGMOID) < 0)
    {
        printf("Applying sigmoid function was unsuccessful.\n");
        return -1;
    }

    printf("y_old = \n");
    printMatrix(*logistic_model.y);
    printf("y_new = \n");
    printMatrix(y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, y_new, logistic_model.type) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(logistic_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    freeModel(&logistic_model);
    // freeEvalMetrics(&eval_metrics);

    return 0;
}

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

    if (makeMatrixZeros(softmax_model.logits, softmax_model.X->rows, softmax_model.classes) < 0)
    {
        printf("Problem initializing logits Matrix\n");
        return -1;
    }

    if (makeMatrixZeros(softmax_model.weights, softmax_model.X->cols, softmax_model.classes) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    if (makeVectorZeros(softmax_model.bias, softmax_model.classes) < 0)
    {
        printf("Problem initializing bias Matrix\n");
        return -1;
    }
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
    printf("\n---------------File Linear Regression---------------\n");
    if (run_linear_regression_from_file() < 0)
    {
        printf("CSV File Linear Regression test was unsuccessful.\n");
    }

    // printf("\n---------------Default Linear Regression---------------\n");
    // if (run_linear_regression_default() < 0)
    // {
    //     printf("Default Linear Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------Default Logistic Regression---------------\n");
    // if (run_logistic_regression_default() < 0)
    // {
    //     printf("Default Logistic Regression test was unsuccessful.\n");
    // }

    // printf("\n---------------Softmax Regression---------------\n");
    // if (run_softmax_regression_default() < 0)
    // {
    //     printf("Default Softmax Regression test was unsuccessful.\n");
    // }

    return 0;
}
