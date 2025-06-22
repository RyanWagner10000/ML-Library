/*
 * file: math_funcs.h
 * description: header file to cover all the funcs that perform math operations
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#include <math.h>

#include "matrix.h"
#include "vector.h"

#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

int dot_product(Vector *x, Vector *y, double *result);
int matvec_mult(Matrix *A, Vector *y, Vector *result);

int transpose(Matrix *A, Matrix *A_t);
int identity(Matrix *A, int size);

int sigmoid(double *x, double *x_out);
int sigmoid_dx(double *x, double *x_out);
int relu(double *x, double *x_out);
int relu_dx(double *x, double *x_out);
int tanh_dx(double *x, double *x_out);
void applyToVector(Vector *v, int (*func)(double *, double *));
void applyToMatrix(Matrix *m, int (*func)(double *, double *));

#endif