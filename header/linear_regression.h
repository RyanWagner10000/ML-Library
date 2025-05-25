// main header file
#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include "../header/math_funcs.h"

int predict(Vector *x, Vector *w, double b, double *result);

int mse(Matrix *x, Vector *y, Vector *w, double b, double *result);

int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b);

int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, double lr, int epochs);

#endif // LINEAR_REGRESSION_H