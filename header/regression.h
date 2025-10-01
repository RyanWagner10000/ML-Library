/*
 * file: regression.h
 * description: header file that gives access to all the regression funcitons
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
    LINEAR_REGRESSION,
    LOGISTIC_REGRESSION,
    SOFTMAX_REGRESSION
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
    RegressionType type; // Type of regression to use
    ModelConfig config;  // Configuration for the model
    Matrix *X;           // Input matrix of NxM dimension
    Matrix *y;           // Input matrix of 1xP dimension
    SplitData splitdata; // Struct that holds all the split data
    Matrix *weights;     // Learned weights matrix of Nx1 dimension
    Vector *bias;        // Learned bias matrix of 1xM dimension
    Matrix *logits;      // Logits vector
    Activation func;     // Activation function
    int batch_size;      // Batch size for regression computation
    int classes;         // Number of classes to use for classification
} Model;

int comptueLabels(Matrix X, Matrix weights, Vector biases, Matrix *labels, Activation activation);

int initModel(Model *model);

int trainModel(Model *model);

void freeModel(Model *model);

#endif // REGRESSION_H