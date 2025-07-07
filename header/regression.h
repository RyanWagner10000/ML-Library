/*
 * file: linear_regression.h
 * description: header file that gives access to all the linear regression funcitons
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#ifndef REGRESSION_H
#define REGRESSION_H

#include "../header/math_funcs.h"

typedef enum
{
    REG_NONE,
    REG_L1,
    REG_L2
} RegularizationType;

typedef enum
{
    LINEAR,
    LOGISTIC
} RegressionType;

typedef struct
{
    double learning_rate;              // Learning rate of the regression
    int epochs;                        // Number of iterations to train the model
    double lambda;                     // Effect of the regularization every iteration
    RegularizationType regularization; // Regularization type
} ModelConfig;

typedef struct
{
    RegressionType type;             // Type of regression to use
    ModelConfig config;              // Configuration for the model
    Matrix *X;                       // Input matrix of NxM dimension
    Vector *y;                       // Input vector of N dimension
    Vector *weights;                 // Learned weights
    double bias;                     // Learned bias
    int (*func)(double *, double *); // Activation function
    int input_dim;                   // number of features
    int output_dim;                  // 1 for now
} Model;

int initModel(Model *model);

int trainModel(Model *model);

void freeModel(Model *model);

#endif // REGRESSION_H