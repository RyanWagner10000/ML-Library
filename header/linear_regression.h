/*
 * file: linear_regression.h
 * description: header file that gives access to all the linear regression funcitons
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include "../header/math_funcs.h"

typedef enum
{
    REG_NONE,
    REG_L1,
    REG_L2
} RegularizationType;

typedef struct
{
    double learning_rate;
    int epochs;
    double lambda;
    RegularizationType regularization;
} TrainConfig;

int predict(Vector *x, Vector *w, double b, double *result);

int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, RegularizationType regularize);

int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, RegularizationType regularize);

int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, TrainConfig *config);

#endif // LINEAR_REGRESSION_H