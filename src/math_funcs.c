#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../header/math_funcs.h"

Vector makeVector(double *array, int size)
{
    Vector v;
    v.size = size;
    v.data = malloc(sizeof(double) * size);

    for (int i = 0; i < size; ++i)
    {
        v.data[i] = array[i];
    }

    return v;
}

Matrix makeMatrix(double *array, int rows, int cols)
{
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = malloc(sizeof(double) * rows * cols);
    memcpy(m.data, array, sizeof(double) * rows * cols);
    return m;
}

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

int dot_product(Vector *x, Vector *y, double *result)
{

    if (x->size != y->size || x->size <= 0 || y->size <= 0)
    {
        return -1;
    }

    *result = 0.0;

    for (int i = 0; i < x->size; ++i)
    {
        *result += x->data[i] * y->data[i];
    }

    return 0;
}

int matvec_mult(Matrix *A, Vector *x, Vector *result)
{
    if (A->cols != x->size)
    {
        return -1;
    }

    result->data = malloc(sizeof(double) * x->size);

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

int transpose(Matrix *A, Matrix *A_t)
{
    if (A == NULL)
    {
        return -1;
    }

    A_t->data = malloc(sizeof(double) * A->rows * A->cols);
    A_t->rows = A->cols;
    A_t->cols = A->rows;

    for (int c = 0; c < A->cols; ++c)
    {
        for (int r = 0; r < A->rows; ++r)
        {
            A_t->data[c * A->rows + r] = A->data[r * A->cols + c];
        }
    }

    return 0;
}

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

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-1.0 * x));
}

double sigmoid_dx(double x)
{
    double x_ = sigmoid(x);
    return x_ * (1 - x_);
}

double relu(double x)
{
    return x > 0 ? x : 0;
}

double relu_dx(double x)
{
    return x > 0 ? 1 : 0;
}

double tanh_dx(double x)
{
    double x_ = tanh(x);
    return (1 - x_ * x_);
}

void applyToVector(Vector *v, double (*func)(double))
{
    for (int i = 0; i < v->size; ++i)
    {
        v->data[i] = func(v->data[i]);
    }
}

void applyToMatrix(Matrix *m, double (*func)(double))
{
    for (int i = 0; i < m->rows * m->cols; ++i)
    {
        m->data[i] = func(m->data[i]);
    }
}
