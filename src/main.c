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

    // TODO calculate eval metrics

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

    // TODO calculate eval metrics

    freeModel(&linear_model);
    freeMatrix(&y_new);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

int run_heart_disease_dataset()
{
    DataType type = TYPE_DOUBLE;

    // Extract the data from the CSV file into a Matrix
    const char *filename = "../datasets/heart_2020_cleaned.csv";
    Model logistic_model;
    if (initModel(&logistic_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    logistic_model.type = LOGISTIC_REGRESSION;
    if (makeMatrix(logistic_model.X, 0, 0, NULL, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    // Load the table in a CSV file into a Matrix object
    if (loadCSVtoMatrix(filename, 1, logistic_model.X) < 0)
    {
        printf("Reading CSV to Matrix was unsuccessful.\n");
        return -1;
    }

    // Copy the y-values from the matrix into a vector
    if (makeMatrixZeros(logistic_model.y, logistic_model.X->rows, 1) < 0)
    {
        printf("Problem initializing output Matrix\n");
        return -1;
    }
    if (getColMatrix(*logistic_model.X, 0, logistic_model.y) < 0)
    {
        printf("Couldn't get column from matrix.\n");
        return -1;
    }

    // Delete y-values from Matrix
    if (deleteColMatrix(logistic_model.X, 0) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    // Z-Score Normalize the input data for better results
    if (normalizeMatrix(logistic_model.X) < 0)
    {
        printf("Reading CSV to Matrix was unsuccessful.\n");
        return -1;
    }

    // Split the data into 80/20 Train/Test
    if (splitData(*logistic_model.X, *logistic_model.y, 80, 20, 0, &logistic_model.splitdata) < 0)
    {
        printf("Splitting input data was unsuccessful.\n");
        return -1;
    }

    logistic_model.classes = 1;
    logistic_model.batch_size = 64;
    logistic_model.func = SIGMOID;

    logistic_model.config.learning_rate = 0.01;
    logistic_model.config.epochs = 2000;
    logistic_model.config.lambda = 0.01;
    logistic_model.config.regularization = REG_L2;

    if (trainModel(&logistic_model) < 0)
    {
        printf("Error with training model.\n");
        return -1;
    }

    // printf("Weight factor = \n");
    // printMatrix(*logistic_model.weights);
    // printf("Bias factor = \n");
    // printVector(*logistic_model.bias);

    // Calculate the predicted labels
    Matrix computed_labels = makeMatrixEmpty();
    if (comptueLabels(logistic_model.splitdata.test_features, *logistic_model.weights, *logistic_model.bias, &computed_labels, logistic_model.func) < 0)
    {
        printf("Computing labels after training was unsuccessful.\n");
        return -1;
    }
    
    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, computed_labels, logistic_model.type) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }
    freeMatrix(&computed_labels);

    if (calculateAllMetrics(&eval_metrics, logistic_model.type, logistic_model.splitdata.test_labels) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    freeModel(&logistic_model);
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

    // printf("\n---------------File Wine Quality Linear Regression---------------\n");
    // if (run_wine_quality_dataset() < 0)
    // {
    //     printf("Wine Quality Dataset linear regression was unsuccessful.\n");
    // }

    printf("\n---------------File Heart Disease Logistic Regression---------------\n");
    if (run_heart_disease_dataset() < 0)
    {
        printf("Heart Disease Dataset Logistic regression was unsuccessful.\n");
    }

    return 0;
}
