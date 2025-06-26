/*
 * file: linear_regression.h
 * description: header file that gives access to all the linear regression funcitons
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#ifndef LOGISTIC_REGRESSION_H
#define LOGISTIC_REGRESSION_H

#include "../header/math_funcs.h"
#include "../header/linear_regression.h"

// int predict(Vector *x, Vector *w, double b, double *result);

// int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, RegularizationType regularize);

// int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, RegularizationType regularize);

int train_logistic_model(Matrix *x, Vector *y_true, Vector *w, double *b, TrainConfig *config);

#endif // LOGISTIC_REGRESSION_H