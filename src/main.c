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
// #include "main.h"

int run_linear_regression_default()
{

    // Inputs: X
    // Outputs: y
    double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    DataType type = TYPE_DOUBLE;

    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR;

    // 10 examples, 1 feature
    if (makeMatrix(linear_model.X, 10, 1, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(linear_model.y, linear_model.X->rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(linear_model.weights, linear_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    linear_model.bias = 0.0;
    linear_model.func = NULL;

    linear_model.config.learning_rate = 0.01;
    linear_model.config.epochs = 1000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_L2;

    if (trainModel(&linear_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(linear_model.weights);
    printf("Bias factor = %.2lf\n", linear_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, linear_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(linear_model.X, linear_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += linear_model.bias;
    }

    printf("y_old = \n");
    printVector(linear_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_new) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeVector(&y_new);
    freeEvalMetrics(&eval_metrics);

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

    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR;

    // 5 examples, 2 features
    if (makeMatrix(linear_model.X, 5, 2, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(linear_model.y, linear_model.X->rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(linear_model.weights, linear_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    linear_model.bias = 0.0;

    linear_model.config.learning_rate = 0.02;
    linear_model.config.epochs = 2000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_NONE;

    if (trainModel(&linear_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(linear_model.weights);
    printf("Bias factor = %.2lf\n", linear_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, linear_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(linear_model.X, linear_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += linear_model.bias;
    }

    printf("y_old = \n");
    printVector(linear_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_new) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeVector(&y_new);
    freeEvalMetrics(&eval_metrics);

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

    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR;

    // 5 examples, 3 features
    if (makeMatrix(linear_model.X, 5, 3, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(linear_model.y, linear_model.X->rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(linear_model.weights, linear_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    linear_model.bias = 0.0;

    linear_model.config.learning_rate = 0.02;
    linear_model.config.epochs = 10000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_NONE;

    if (trainModel(&linear_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(linear_model.weights);
    printf("Bias factor = %.2lf\n", linear_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, linear_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(linear_model.X, linear_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += linear_model.bias;
    }

    printf("y_old = \n");
    printVector(linear_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_new) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeVector(&y_new);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

int run_linear_regression_from_file()
{
    int status = 0;

    // Extract the data from the CSV file into a Matrix
    const char *filename = "../datasets/Salary_dataset.csv";
    Model linear_model;
    if (initModel(&linear_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    linear_model.type = LINEAR;
    status = makeMatrix(linear_model.X, 0, 0, NULL, TYPE_DOUBLE);

    int header = 1;
    status = loadCSVtoMatrix(filename, header, linear_model.X);

    // Inputs: X = [[1.2], [1.4], [1.6], [...]]
    // Outputs: y = [39344.00, 46206.00, 37732.00, ...]

    // Delete the first column of the data, it's not needed
    if (deleteColMatrix(linear_model.X, 0) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    // Copy the y-values from the matrix into a vector
    if (makeVectorZeros(linear_model.y, linear_model.X->rows) < 0)
    {
        printf("Could not make y-values Vector.\n");
        return -1;
    }
    if (getColMatrix(linear_model.X, 1, linear_model.y) < 0)
    {
        printf("Couldn't get column from matrix.\n");
        return -1;
    }

    // Delete y-values from Matrix
    if (deleteColMatrix(linear_model.X, 1) < 0)
    {
        printf("Column deletion unsuccessful.\n");
        return -1;
    }

    if (makeVectorZeros(linear_model.weights, linear_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    linear_model.bias = 0.0;

    linear_model.config.learning_rate = 0.01;
    linear_model.config.epochs = 1000;
    linear_model.config.lambda = 0.01;
    linear_model.config.regularization = REG_L2;

    if (trainModel(&linear_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(linear_model.weights);
    printf("Bias factor = %.2lf\n", linear_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, linear_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(linear_model.X, linear_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += linear_model.bias;
    }

    printf("y_old = \n");
    printVector(linear_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_new) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&linear_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&linear_model);
    freeVector(&y_new);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

int run_logistic_regression_default()
{
    // Inputs: X
    // Outputs: y
    double x_vals[] = {0.5, 1.0, 1.5, 2.0, 3.0, 3.5, 4.0, 4.5};
    double y_vals[] = {0, 0, 0, 0, 1, 1, 1, 1};

    DataType datatype = TYPE_DOUBLE;

    Model logistic_model;
    if (initModel(&logistic_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    logistic_model.type = LOGISTIC;

    // 8 examples, 1 feature
    if (makeMatrix(logistic_model.X, 8, 1, &x_vals, datatype) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(logistic_model.y, logistic_model.X->rows, &y_vals, datatype) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(logistic_model.weights, logistic_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    logistic_model.bias = 0.0;

    logistic_model.func = sigmoid;

    logistic_model.config.learning_rate = 0.02;
    logistic_model.config.epochs = 5000;
    logistic_model.config.lambda = 0.01;
    logistic_model.config.regularization = REG_NONE;

    if (trainModel(&logistic_model) < 0)
    {
        printf("Error with training linear model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(logistic_model.weights);
    printf("Bias factor = %.2lf\n", logistic_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, logistic_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(logistic_model.X, logistic_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += logistic_model.bias;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
    }

    printf("y_old = \n");
    printVector(logistic_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    Vector y_labeled;
    if (makeVectorZeros(&y_labeled, y_new.size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }
    if (getPredictedLabels(&y_new, &y_labeled, 0.5))
    {
        printf("Problem calculating labels from predictions\n");
        return -1;
    }
    printf("y_labeled = \n");
    printVector(&y_labeled);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_labeled) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&logistic_model);
    freeVector(&y_new);
    freeVector(&y_labeled);
    freeEvalMetrics(&eval_metrics);

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
        7.0, 1.5};
    double y_vals[] = {0, 0, 0, 1, 1, 1};

    DataType type = TYPE_DOUBLE;

    Model logistic_model;
    if (initModel(&logistic_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    logistic_model.type = LOGISTIC;

    // 4 examples, 1 feature
    if (makeMatrix(logistic_model.X, 6, 2, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(logistic_model.y, logistic_model.X->rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(logistic_model.weights, logistic_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    logistic_model.bias = 0.0;

    logistic_model.func = sigmoid;

    logistic_model.config.learning_rate = 0.1;
    logistic_model.config.epochs = 10000;
    logistic_model.config.lambda = 0.001;
    logistic_model.config.regularization = REG_L2;

    if (trainModel(&logistic_model) < 0)
    {
        printf("Error with training logistic model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(logistic_model.weights);
    printf("Bias factor = %.2lf\n", logistic_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, logistic_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(logistic_model.X, logistic_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += logistic_model.bias;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
    }

    printf("y_old = \n");
    printVector(logistic_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    Vector y_labeled;
    if (makeVectorZeros(&y_labeled, y_new.size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }
    if (getPredictedLabels(&y_new, &y_labeled, 0.5))
    {
        printf("Problem calculating labels from predictions\n");
        return -1;
    }
    printf("y_labeled = \n");
    printVector(&y_labeled);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_labeled) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&logistic_model);
    freeVector(&y_new);
    freeVector(&y_labeled);
    freeEvalMetrics(&eval_metrics);

    return 0;
}

int run_logistic_regression_3d()
{

    // Inputs: X
    // Outputs: y
    double x_vals[] = {
        2.0, 3.0, 0.0,
        3.0, 2.0, 1.0,
        4.0, 3.0, 2.0,
        5.0, 2.0, 3.0,
        6.0, 2.0, 4.0};
    double y_vals[] = {0, 0, 1, 1, 1};

    DataType type = TYPE_DOUBLE;

    Model logistic_model;
    if (initModel(&logistic_model) < 0)
    {
        printf("Problem initializing Model object.\n");
        return 0;
    }
    logistic_model.type = LOGISTIC;

    // 4 examples, 1 feature
    if (makeMatrix(logistic_model.X, 5, 3, &x_vals, type) < 0)
    {
        printf("Problem initializing input Matrix\n");
        return -1;
    }

    if (makeVector(logistic_model.y, logistic_model.X->rows, &y_vals, type) < 0)
    {
        printf("Problem initializing output Vector\n");
        return -1;
    }

    if (makeVectorZeros(logistic_model.weights, logistic_model.X->cols) < 0)
    {
        printf("Problem initializing weight Vector\n");
        return -1;
    }

    logistic_model.bias = 0.0;

    logistic_model.func = sigmoid;

    logistic_model.config.learning_rate = 0.1;
    logistic_model.config.epochs = 10000;
    logistic_model.config.lambda = 0.001;
    logistic_model.config.regularization = REG_L2;

    if (trainModel(&logistic_model) < 0)
    {
        printf("Error with training logistic model.\n");
        return -1;
    }

    printf("Weight factor = ");
    printVector(logistic_model.weights);
    printf("Bias factor = %.2lf\n", logistic_model.bias);

    Vector y_new;
    if (makeVectorZeros(&y_new, logistic_model.y->size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }

    if (matvec_mult(logistic_model.X, logistic_model.weights, &y_new) < 0)
    {
        printf("Error with dot product.\n");
        return -1;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += logistic_model.bias;
        double temp = 0.0;
        if (sigmoid(&y_new.data[i], &temp) < 0)
        {
            printf("Error executing sigmoid on y_new data.\n");
            return -1;
        }

        y_new.data[i] = temp;
    }

    printf("y_old = \n");
    printVector(logistic_model.y);
    printf("y_new = \n");
    printVector(&y_new);

    Vector y_labeled;
    if (makeVectorZeros(&y_labeled, y_new.size) < 0)
    {
        printf("Problem initializing output predictions\n");
        return -1;
    }
    if (getPredictedLabels(&y_new, &y_labeled, 0.5))
    {
        printf("Problem calculating labels from predictions\n");
        return -1;
    }
    printf("y_labeled = \n");
    printVector(&y_labeled);

    // Perform evaluation metrics on the model
    EvalMetrics eval_metrics;
    if (initEvalMetrics(&eval_metrics, &y_labeled) < 0)
    {
        printf("Initialization of evaluation metrics object failed.\n");
        return -1;
    }

    if (calculateAllMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Calculating all performance metrics failed.\n");
        return -1;
    }

    if (printMetrics(&logistic_model, &eval_metrics) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    freeModel(&logistic_model);
    freeVector(&y_new);
    freeVector(&y_labeled);
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

    printf("\n---------------2 Variate Linear Regression---------------\n");
    if (run_two_variate_linear_regression() < 0)
    {
        printf("Two Variate Linear Regression test was unsuccessful.\n");
    }

    printf("\n---------------3 Variate Linear Regression---------------\n");
    if (run_three_variate_linear_regression() < 0)
    {
        printf("Three Variate Linear Regression test was unsuccessful.\n");
    }

    printf("\n---------------File Linear Regression---------------\n");
    if (run_linear_regression_from_file() < 0)
    {
        printf("CSV File Linear Regression test was unsuccessful.\n");
    }

    printf("\n---------------Default Logistic Regression---------------\n");
    if (run_logistic_regression_default() < 0)
    {
        printf("Default Logistic Regression test was unsuccessful.\n");
    }

    printf("\n---------------2D Logistic Regression---------------\n");
    if (run_logistic_regression_2d() < 0)
    {
        printf("Default Logistic Regression test was unsuccessful.\n");
    }

    printf("\n---------------3D Logistic Regression---------------\n");
    if (run_logistic_regression_3d() < 0)
    {
        printf("Default Logistic Regression test was unsuccessful.\n");
    }

    return 0;
}