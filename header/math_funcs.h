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
#define mat_mul(a, b, c) _Generic((b), \
    double: mat_mul_double,            \
    Matrix: mat_mul_matrix)(a, b, c)
#define mat_add(a, b, c) _Generic((b), \
    double: mat_add_double,            \
    Matrix: mat_add_matrix)(a, b, c)
#define mat_sub(a, b, c) _Generic((b), \
    double: mat_sub_double,            \
    Matrix: mat_sub_matrix)(a, b, c)
#define mat_div(a, b, c) _Generic((b), \
    double: mat_div_double)(a, b, c)

#define vect_mul(a, b, c) _Generic((b), \
    double: vect_mul_double,            \
    Vector: vect_mul_vector)(a, b, c)
#define vect_add(a, b, c) _Generic((b), \
    double: vect_add_double,            \
    Vector: vect_add_vector)(a, b, c)
#define vect_sub(a, b, c) _Generic((b), \
    double: vect_sub_double,            \
    Vector: vect_sub_vector)(a, b, c)

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

int dot_product(Vector x, Vector y, double *result);
int matvec_mult(Matrix A, Vector y, Vector *result);

int mat_mul_matrix(Matrix A, Matrix B, Matrix *result);
int mat_mul_double(Matrix A, double B, Matrix *result);

int mat_add_matrix(Matrix A, Matrix B, Matrix *result);
int mat_add_double(Matrix A, double B, Matrix *result);

int mat_sub_matrix(Matrix A, Matrix B, Matrix *result);
int mat_sub_double(Matrix A, double B, Matrix *result);

int mat_div_double(Matrix A, double B, Matrix *result);

int vect_mul_vector(Vector A, Vector B, Vector *result);
int vect_mul_double(Vector A, double B, Vector *result);

int vect_add_vector(Vector A, Vector B, Vector *result);
int vect_add_double(Vector A, double B, Vector *result);

int vect_sub_vector(Vector A, Vector B, Vector *result);
int vect_sub_double(Vector A, double B, Vector *result);

int transpose(Matrix A, Matrix *A_t);
int identity(Matrix *A, int size);

int sigmoid(double x, double *x_out);
int sigmoid_dx(double x, double *x_out);
int relu(double x, double *x_out);
int relu_dx(double x, double *x_out);
int tanh_(double x, double *x_out);
int tanh_dx(double x, double *x_out);
int softmax(double x_i, Vector x_j, double *x_out);
int applyToVector(Vector *v, Activation func);
int applyToMatrix(Matrix *m, Activation func);

#endif