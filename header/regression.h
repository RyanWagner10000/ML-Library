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

typedef enum
{
    CONSTANT,
    LINEAR_DECAY,
    EXPONENTIAL_DECAY,
    STEP_DECAY,
    COSINE_ANNEALING
} DecayType;

typedef struct
{
    double init_learning_rate; // Initial learning rate of the regression
    double min_learning_rate;  // Minimum learning rate used for Cosine Annealing
    double curr_learning_rate; // Current learning rate used for all functions
    double max_epoch_cycle;    // Maximum Epochs in cycle for Cosine Annealing
    DecayType decay_type;      // Learning rate decay type
    int decay_step;            // Learning rate step decay value
    float decay_constant;      // Exponential learning rate decay rate
} LearningRate;

typedef struct
{
    int epochs;                        // Number of iterations to train the model
    double lambda;                     // Effect of the regularization every iteration
    RegularizationType regularization; // Regularization type
    LearningRate learning_rate;        // Learning rate information
} ModelConfig;

typedef struct
{
    double *loss_vs_epochs; // Array of loss over epochs
} ModelMetrics;

typedef struct
{
    RegressionType type;  // Type of regression to use
    ModelConfig config;   // Configuration for the model
    ModelMetrics metrics; // Configuration for the model
    Matrix *X;            // Input matrix of NxM dimension
    Matrix *y;            // Input matrix of 1xP dimension
    SplitData splitdata;  // Struct that holds all the split data
    Matrix *weights;      // Learned weights matrix of Nx1 dimension
    Vector *bias;         // Learned bias matrix of 1xM dimension
    Matrix *logits;       // Logits vector
    Activation func;      // Activation function
    int batch_size;       // Batch size for regression computation
    int classes;          // Number of classes to use for classification
    double beta;          // Number to control momentum
} Model;

ModelConfig makeDefaultConfig();

int comptueLabels(Matrix X, Matrix weights, Vector biases, Matrix *labels, Activation activation);

int initModel(Model *model);

int trainModel(Model *model);

void freeModel(Model *model);

#endif // REGRESSION_H