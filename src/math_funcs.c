/*
 * file: math_funcs.c
 * description: script with all the self written math functions
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: All math functions are row-wise vectors/matrices in memory
 */

#include "../header/math_funcs.h"

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
    if (x->size != y->size)
    {
        printf("Size of input Vectors do not match for dot product operation.\n");
        printf("Size of Vector x = %d.\n", x->size);
        printf("Size of Vector y = %d.\n", y->size);
        return -1;
    }
    if (x->size <= 0)
    {
        printf("Size of input Vector x in <= 0.\n");
        return -1;
    }
    if (y->size <= 0)
    {
        printf("Size of input Vector y in <= 0.\n");
        return -1;
    }

    // Perform sum of products
    *result = 0.0;
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

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A->rows; ++i)
    {

        for (int j = 0; j < A->cols; ++j)
        {
            result->data[i] += A->data[i * A->cols + j] * x->data[j];
            // printf("result->data[i] = %lf\n", result->data[i]);
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
    if (A == NULL || A->data == NULL)
    {
        return -1;
    }

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
 * @param x type-less pointer to input variable in Sigmoid function
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid(double *x, double *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    double temp = 1.0 / (1.0 + exp(-1.0 * *x));
    *x_out = temp;

    return 0;
}

/**
 * @brief Derivative of sigmoid function: (1/(1 + e^(-x))) * (1 - (1/(1 + e^(-x))))
 *
 * @param x type-less pointer to input variable in Sigmoid Derivative function
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid_dx(double *x, double *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    double temp_out = 0.0;
    if (sigmoid(x, &temp_out) < 0)
    {
        x_out = NULL;
        return -1;
    }
    *x_out = temp_out * (1 - temp_out);

    return 0;
}

/**
 * @brief ReLu function: maximum of 0 or x
 *
 * @param x type-less pointer to input variable in Relu Derivative function
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu(double *x, double *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    *x_out = *x > 0 ? *x : 0;

    return 0;
}

/**
 * @brief ReLu derivative function
 *
 * @param x type-less pointer to input variable in Relu Derivative function
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu_dx(double *x, double *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    *x_out = *x > 0 ? 1.0 : 0;

    return 0;
}

/**
 * @brief Tanh derivative function: 1-(tanh(x))^2
 *
 * @param x type-less pointer to input variable in Tanh Derivative function
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int tanh_dx(double *x, double *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    double x_ = tanh(*x);
    *x_out = (1 - x_ * x_);

    return 0;
}

/**
 * @brief Function to apply an activation function to a Vector
 *
 * @param v Vector pointer to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
void applyToVector(Vector *v, int (*func)(double *, double *))
{
    double temp_out = 0;
    for (int i = 0; i < v->size; ++i)
    {
        if (func(&(v->data)[i], &temp_out) < 0)
        {
            printf("Error applying function to each element in vector.\n");
            v->data = NULL;
            return;
        }
        v->data[i] = temp_out;
    }

    return;
}

/**
 * @brief Function to apply an activation function to a Matrix
 *
 * @param m Matrix to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
void applyToMatrix(Matrix *m, int (*func)(double *, double *))
{

    double temp_out = 0;
    for (int i = 0; i < m->rows * m->cols; ++i)
    {
        if (func(&(m->data)[i], &temp_out) < 0)
        {
            printf("Error applying function to each element in vector.\n");
            m->data = NULL;
            return;
        }
        m->data[i] = temp_out;
    }

    return;
}
