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

int dot_product(Vector *x, Vector *y, void *result);
int matvec_mult(Matrix *A, Vector *y, Vector *result);

int transpose(Matrix *A, Matrix *A_t);
int identity(Matrix *A, int size);

int sigmoid(void *x, DataType type, void *x_out);
int sigmoid_dx(void *x, DataType type, void *x_out);
int relu(void *x, DataType type, void *x_out);
int relu_dx(void *x, DataType type, void *x_out);
int tanh_dx(void *x, DataType type, void *x_out);
void applyToVector(Vector *v, int (*func)(void *, DataType, void *), DataType type);
void applyToMatrix(Matrix *m, int (*func)(void *, DataType, void *), DataType type);

#endif