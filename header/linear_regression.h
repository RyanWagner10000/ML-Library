// main header file
#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../header/math_funcs.h"

int predict(Vector *x, Vector *w, double b, double *result);

int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, bool regularize);

int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, bool regularize);

int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, double lr, int epochs, double lambda, bool regularize);

#endif // LINEAR_REGRESSION_H