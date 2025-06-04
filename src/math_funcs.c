/*
 * file: math_funcs.c
 * description: script with all the self written math functions
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: All math functions are row-wise vectors/matrices in memory
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../header/math_funcs.h"

/**
 * @brief Makes a Vector object from an array and a size
 *
 * @param array of doubles for vector
 * @param size of the array being put into vector
 *
 * @return Vector object filled with input array and size
 */
Vector makeVector(double *array, int size)
{
    // Create vector and allocate memory the size of input array
    Vector v;
    v.size = size;
    v.data = malloc(sizeof(double) * size);

    for (int i = 0; i < size; ++i)
    {
        v.data[i] = array[i];
    }

    return v;
}

/**
 * @brief Makes a Matrix object from an array and number of rows and cols
 *
 * @param array of doubles for matrix
 * @param rows for matrix
 * @param cols for matrix
 *
 * @return Matrix object filled with input array of or M rows and N cols
 */
Matrix makeMatrix(double *array, int rows, int cols)
{
    // Create Matrix and allocate memory the size of input rows X cols
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = malloc(sizeof(double) * rows * cols);
    memcpy(m.data, array, sizeof(double) * rows * cols);
    return m;
}

/**
 * @brief Basic printing of a vector object
 *
 * @param v Vector object to print
 *
 * @return None
 */
void printVector(Vector *v)
{
    printf("[");
    for (int i = 0; i < v->size; ++i)
    {
        printf("%.2f", v->data[i]);
        if (i < v->size - 1)
            printf(", ");
    }
    printf("]\n");
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param m Matrix object to print
 *
 * @return None
 */
void printMatrix(Matrix *m)
{
    for (int i = 0; i < m->rows; ++i)
    {
        printf("[");
        for (int j = 0; j < m->cols; ++j)
        {
            printf("%.2f", m->data[i * m->cols + j]);
            if (j < m->cols - 1)
                printf(", ");
        }
        printf("]\n");
    }
}

/**
 * @brief Performs the dot product of two vectors
 *
 * @param x Pointer to x of Vector type
 * @param y Pointer to y of Vector type
 * @param result of the dot product operation
 *
 * @return 0 on success and -1 on failure
 */
int dot_product(Vector *x, Vector *y, double *result)
{
    // If they ain't the right size then exit on failure
    if (x->size != y->size || x->size <= 0 || y->size <= 0)
    {
        return -1;
    }

    *result = 0.0;

    // Perform sum of products
    for (int i = 0; i < x->size; ++i)
    {
        *result += x->data[i] * y->data[i];
    }

    return 0;
}

/**
 * @brief Matrix and Vector multiplication operation
 *
 * @param A Matrix of size MxN of Matrix type
 * @param x Vector of size N of Vector type
 * @param result Calculated vector of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int matvec_mult(Matrix *A, Vector *x, Vector *result)
{
    // If sizes don't match, then exit on failure
    if (A->cols != x->size)
    {
        return -1;
    }

    result->data = malloc(sizeof(double) * x->size);

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A->rows; ++i)
    {
        result->data[i] = 0.0;

        for (int j = 0; j < A->cols; ++j)
        {
            result->data[i] += A->data[i * A->cols + j] * x->data[j];
        }
    }

    return 0;
}

/**
 * @brief Performs the transpose of the input matrix
 *
 * @param A Matrix to be transposed
 * @param A_t Transposed matrix
 *
 * @return 0 on success and -1 on failure
 */
int transpose(Matrix *A, Matrix *A_t)
{
    if (A == NULL)
    {
        return -1;
    }

    A_t->data = malloc(sizeof(double) * A->rows * A->cols);
    A_t->rows = A->cols;
    A_t->cols = A->rows;

    // Iterate COLUMN-wise through row-matrix to transpose it
    for (int c = 0; c < A->cols; ++c)
    {
        for (int r = 0; r < A->rows; ++r)
        {
            A_t->data[c * A->rows + r] = A->data[r * A->cols + c];
        }
    }

    return 0;
}

/**
 * @brief Makes an identity matrix of given size
 *
 * @param A Matrix pointer to fill with identity values
 * @param size of the MxM identity matrix desired
 *
 * @return 0 on success and -1 on failure
 */
int identity(Matrix *A, int size)
{
    if (size < 1)
    {
        return -1;
    }

    A->cols = size;
    A->rows = size;
    A->data = calloc(size * size, sizeof(double));

    for (int i = 0; i < size; ++i)
    {
        A->data[i * size + i] = 1.0;
    }

    return 0;
}

/**
 * @brief Sigmoid function: (1/(1 + e^(-x)))
 *
 * @param x Value to apply to sigmoid function
 *
 * @return Value calculated from function
 */
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-1.0 * x));
}

/**
 * @brief Derivative of sigmoid function: (1/(1 + e^(-x))) * (1 - (1/(1 + e^(-x))))
 *
 * @param x Value to apply to function
 *
 * @return Value calculated from function
 */
double sigmoid_dx(double x)
{
    double x_ = sigmoid(x);
    return x_ * (1 - x_);
}

/**
 * @brief ReLu function: maximum of 0 or x
 *
 * @param x Value to apply to sigmoid function
 *
 * @return Value calculated from function
 */
double relu(double x)
{
    return x > 0 ? x : 0;
}

/**
 * @brief ReLu derivative function
 *
 * @param x Value to apply to function
 *
 * @return Value calculated from function
 */
double relu_dx(double x)
{
    return x > 0 ? 1 : 0;
}

/**
 * @brief Tanh derivative function: 1-(tanh(x))^2
 *
 * @param x Value to apply to function
 *
 * @return Value calculated from function
 */
double tanh_dx(double x)
{
    double x_ = tanh(x);
    return (1 - x_ * x_);
}

/**
 * @brief Function to apply an activation function to a Vector
 *
 * @param v Vector pointer to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
void applyToVector(Vector *v, double (*func)(double))
{
    for (int i = 0; i < v->size; ++i)
    {
        v->data[i] = func(v->data[i]);
    }
}

/**
 * @brief Function to apply an activation function to a Matrix
 *
 * @param m Matrix to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
void applyToMatrix(Matrix *m, double (*func)(double))
{
    for (int i = 0; i < m->rows * m->cols; ++i)
    {
        m->data[i] = func(m->data[i]);
    }
}
