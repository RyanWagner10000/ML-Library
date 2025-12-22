/*
 * file: enums.h
 * description: header file that holds all the enums other files can use
 * author: Ryan Wagner
 * date: Dec 21, 2025
 * notes:
 */

#ifndef ENUMS_H
#define ENUMS_H

#include "vector.h"

// ENUMERATIONS

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

typedef enum
{
    ACT_NONE,
    SIGMOID,
    SIGMOID_DX,
    RELU,
    RELU_DX,
    TANH,
    TANH_DX,
    SOFTMAX
} Activation;

// STRUCTS

typedef struct
{
    Matrix train_features; // Matrix that holds the training features
    Matrix test_features;  // Matrix that holds the testing features
    Matrix valid_features; // Matrix that holds the validation features
    Matrix train_labels;   // Matrix that holds the training features
    Matrix test_labels;    // Matrix that holds the testing features
    Matrix valid_labels;   // Matrix that holds the validation features
} SplitData;

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

typedef struct
{
    Matrix *y_lables; // Vector of predicted classes
    double threshold; // Threshold value for comparison of real to predicted values
    int TP;           // True Positive value
    int FP;           // False Positive value
    int TN;           // True Negative value
    int FN;           // False Negative value
    double accuracy;  // Fraction of correct predictions
    double precision; // How many predicted 1's are correct for binary classification
    double recall;    // How many actual 1's were caught  for binary classification
    double f1;        // harminic mean of precision and recall
    double mse;       // Mean Squared Error, lower is better
    double rmse;      // Root Mean Squared Error, in same units as the output
    double mae;       // Mean Absolute Error, less sensitive to outliers thatn MSE
    double r2score;   // Coefficient of Determination, how well the model explains variance in the data
} EvalMetrics;

#endif // ENUMS_H