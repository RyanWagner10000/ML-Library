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

int run_salary_dataset()
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

int run_wine_quality_dataset()
{
    DataType type = TYPE_DOUBLE;

    // Extract the data from the CSV file into a Matrix
    const char *filename = "../datasets/winequality-red.csv";
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

    // Load the table in a CSV file into a Matrix object
    if (loadCSVtoMatrix(filename, 1, linear_model.X) < 0)
    {
        printf("Reading CSV to Matrix was unsuccessful.\n");
        return -1;
    }

    // Copy the y-values from the matrix into a vector
    if (makeMatrixZeros(linear_model.y, linear_model.X->rows, 1) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    if (getColMatrix(*linear_model.X, 11, linear_model.y) < 0)
    {
        printf("Couldn't get column from matrix.\n");
        return -1;
    }
    
    linear_model.classes = 1;
    linear_model.batch_size = 32;

    // Delete y-values from Matrix
    if (deleteColMatrix(linear_model.X, 11) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }
    
    linear_model.func = ACT_NONE;

    linear_model.config.learning_rate = 0.0002;
    linear_model.config.epochs = 20000;
    linear_model.config.lambda = 0.0001;
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

/**
 * @brief Main function
 *
 * @param None
 *
 * @return 0 on success, -1 on failure
 */
int main(void)
{
    // printf("\n---------------File Salary Dataset Linear Regression---------------\n");
    // if (run_salary_dataset() < 0)
    // {
    //     printf("Salary Dataset linear regression was unsuccessful.\n");
    // }

    printf("\n---------------File Wine Quality Linear Regression---------------\n");
    if (run_wine_quality_dataset() < 0)
    {
        printf("Wine Quality Dataset linear regression was unsuccessful.\n");
    }

    return 0;
}
