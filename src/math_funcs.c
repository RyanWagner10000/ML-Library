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
int dot_product(Vector *x, Vector *y, void *result)
{
    // If they ain't the right size then exit on failure
    if (x->size != y->size || x->size <= 0 || y->size <= 0 || x->type != y->type)
    {
        return -1;
    }

    // Perform sum of products
    switch (x->type)
    {
    case TYPE_INT:
        *(int *)result = 0;
        for (int i = 0; i < x->size; ++i)
        {
            *(int *)result += ((int *)x->data)[i] * ((int *)y->data)[i];
        }
        break;
    case TYPE_FLOAT:
        *(float *)result = 0;
        for (int i = 0; i < x->size; ++i)
        {
            *(float *)result += ((float *)x->data)[i] * ((float *)y->data)[i];
        }
        break;
    case TYPE_DOUBLE:
        *(double *)result = 0;
        for (int i = 0; i < x->size; ++i)
        {
            *(double *)result += ((double *)x->data)[i] * ((double *)y->data)[i];
        }
        break;
    default:
        break;
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
    if (A->cols != x->size || A->type != x->type || A->type != result->type)
    {
        return -1;
    }

    // Perform sum of products for rows in Matrix
    switch (x->type)
    {
    case TYPE_INT:
        if (result->data != NULL)
        {
            freeVector(result);
        }
        result->data = malloc(sizeof(int) * x->size);

        for (int i = 0; i < A->rows; ++i)
        {
            ((int *)result->data)[i] = 0.0;

            for (int j = 0; j < A->cols; ++j)
            {
                ((int *)result->data)[i] += ((int *)A->data)[i * A->cols + j] * ((int *)x->data)[j];
            }
        }
        break;
    case TYPE_FLOAT:
        if (result->data != NULL)
        {
            freeVector(result);
        }
        result->data = malloc(sizeof(float) * x->size);

        for (int i = 0; i < A->rows; ++i)
        {
            ((float *)result->data)[i] = 0.0;

            for (int j = 0; j < A->cols; ++j)
            {
                ((float *)result->data)[i] += ((float *)A->data)[i * A->cols + j] * ((float *)x->data)[j];
            }
        }
        break;
    case TYPE_DOUBLE:
        if (result->data != NULL)
        {
            freeVector(result);
        }
        result->data = malloc(sizeof(double) * x->size);

        for (int i = 0; i < A->rows; ++i)
        {
            ((double *)result->data)[i] = 0.0;

            for (int j = 0; j < A->cols; ++j)
            {
                ((double *)result->data)[i] += ((double *)A->data)[i * A->cols + j] * ((double *)x->data)[j];
            }
        }
        break;
    default:
        break;
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
    if (A == NULL || A->data == NULL || A->type != A_t->type)
    {
        return -1;
    }

    A_t->rows = A->cols;
    A_t->cols = A->rows;

    // Iterate COLUMN-wise through row-matrix to transpose it
    switch (A->type)
    {
    case TYPE_INT:
        for (int c = 0; c < A->cols; ++c)
        {
            for (int r = 0; r < A->rows; ++r)
            {
                ((int *)A_t->data)[c * A->rows + r] = ((int *)A->data)[r * A->cols + c];
            }
        }
        break;
    case TYPE_FLOAT:
        for (int c = 0; c < A->cols; ++c)
        {
            for (int r = 0; r < A->rows; ++r)
            {
                ((float *)A_t->data)[c * A->rows + r] = ((float *)A->data)[r * A->cols + c];
            }
        }
        break;
    case TYPE_DOUBLE:
        for (int c = 0; c < A->cols; ++c)
        {
            for (int r = 0; r < A->rows; ++r)
            {
                ((double *)A_t->data)[c * A->rows + r] = ((double *)A->data)[r * A->cols + c];
            }
        }
        break;
    default:
        break;
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

    switch (A->type)
    {
    case TYPE_INT:
        A->data = calloc(size * size, sizeof(int));
        for (int i = 0; i < size; ++i)
        {
            ((int *)A->data)[i * size + i] = 1;
        }
        break;
    case TYPE_FLOAT:
        A->data = calloc(size * size, sizeof(float));
        for (int i = 0; i < size; ++i)
        {
            ((float *)A->data)[i * size + i] = 1.0;
        }
        break;
    case TYPE_DOUBLE:
        A->data = calloc(size * size, sizeof(double));
        for (int i = 0; i < size; ++i)
        {
            ((double *)A->data)[i * size + i] = 1.0;
        }
        break;
    default:
        break;
    }

    return 0;
}

/**
 * @brief Sigmoid function: (1/(1 + e^(-x)))
 *
 * @param x type-less pointer to input variable in Sigmoid function
 * @param type Datatype of x used in Matrix/Vector
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid(void *x, DataType type, void *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    switch (type)
    {
    case TYPE_INT:
    {
        // Scales decimal to percentage
        // Ex: sig(2.0) = 0.88 == 0 as an int
        //     0.88 * 100 = 88, which can be stored in int
        double temp = 1.0 / (1.0 + exp(-1.0 * *(int *)x));
        *(int *)x_out = (int)(temp * 100);
        break;
    }
    case TYPE_FLOAT:
    {
        float temp = 1.0 / (1.0 + exp(-1.0 * *(float *)x));
        *(float *)x_out = temp;
        break;
    }
    case TYPE_DOUBLE:
    {
        double temp = 1.0 / (1.0 + exp(-1.0 * *(double *)x));
        *(double *)x_out = temp;
        break;
    }
    default:
    {
        return -1;
    }
    }

    return 0;
}

/**
 * @brief Derivative of sigmoid function: (1/(1 + e^(-x))) * (1 - (1/(1 + e^(-x))))
 *
 * @param x type-less pointer to input variable in Sigmoid Derivative function
 * @param type Datatype of x used in Matrix/Vector
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid_dx(void *x, DataType type, void *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    switch (type)
    {
    case TYPE_INT:
    {
        int temp_out = 0;
        if (sigmoid((int *)x, type, &temp_out) < 0)
        {
            x_out = NULL;
            return -1;
        }
        *(int *)x_out = temp_out * (1 - temp_out);
        break;
    }
    case TYPE_FLOAT:
    {
        float temp_out = 0.0;
        if (sigmoid((float *)x, type, &temp_out) < 0)
        {
            x_out = NULL;
            return -1;
        }
        *(float *)x_out = temp_out * (1 - temp_out);
        break;
    }
    case TYPE_DOUBLE:
    {
        double temp_out = 0.0;
        if (sigmoid((double *)x, type, &temp_out) < 0)
        {
            x_out = NULL;
            return -1;
        }
        *(double *)x_out = temp_out * (1 - temp_out);
        break;
    }
    default:
    {
        return -1;
    }
    }

    return 0;
}

/**
 * @brief ReLu function: maximum of 0 or x
 *
 * @param x type-less pointer to input variable in Relu Derivative function
 * @param type Datatype of x used in Matrix/Vector
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu(void *x, DataType type, void *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    switch (type)
    {
    case TYPE_INT:
    {
        *(int *)x_out = *(int *)x > 0 ? *(int *)x : 0;
        break;
    }
    case TYPE_FLOAT:
    {
        *(float *)x_out = *(float *)x > 0 ? *(float *)x : 0;
        break;
    }
    case TYPE_DOUBLE:
    {
        *(double *)x_out = *(double *)x > 0 ? *(double *)x : 0;
        break;
    }
    default:
    {
        return -1;
    }
    }

    return 0;
}

/**
 * @brief ReLu derivative function
 *
 * @param x type-less pointer to input variable in Relu Derivative function
 * @param type Datatype of x used in Matrix/Vector
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu_dx(void *x, DataType type, void *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    switch (type)
    {
    case TYPE_INT:
    {
        *(int *)x_out = *(int *)x > 0 ? 1 : 0;
        break;
    }
    case TYPE_FLOAT:
    {
        *(float *)x_out = *(float *)x > 0 ? 1.0 : 0;
        break;
    }
    case TYPE_DOUBLE:
    {
        *(double *)x_out = *(double *)x > 0 ? 1.0 : 0;
        break;
    }
    default:
    {
        return -1;
    }
    }

    return 0;
}

/**
 * @brief Tanh derivative function: 1-(tanh(x))^2
 *
 * @param x type-less pointer to input variable in Tanh Derivative function
 * @param type Datatype of x used in Matrix/Vector
 * @param x_out type-less pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int tanh_dx(void *x, DataType type, void *x_out)
{
    if (!x || !x_out)
    {
        return -1;
    }

    switch (type)
    {
    case TYPE_INT:
    {
        // Scales decimal to percentage
        // Ex: tanh_dx(1.0) = 0.42 == 0 as an int
        //     0.42 * 100 = 42, which can be stored in int
        double x_ = tanh(*(int *)x);
        *(int *)x_out = (1 - x_ * x_) * 100;
        break;
    }
    case TYPE_FLOAT:
    {
        float x_ = tanh(*(float *)x);
        *(float *)x_out = 1 - x_ * x_;
        break;
    }
    case TYPE_DOUBLE:
    {
        double x_ = tanh(*(double *)x);
        *(double *)x_out = (1 - x_ * x_);
        break;
    }
    default:
    {
        return -1;
    }
    }

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
void applyToVector(Vector *v, int (*func)(void *, DataType, void *), DataType type)
{
    switch (type)
    {
    case TYPE_INT:
    {
        int temp_out = 0;
        for (int i = 0; i < v->size; ++i)
        {
            if (func(&((int *)v->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                v->data = NULL;
                return;
            }
            ((int *)v->data)[i] = temp_out;
        }
        break;
    }
    case TYPE_FLOAT:
    {
        float temp_out = 0;
        for (int i = 0; i < v->size; ++i)
        {
            if (func(&((float *)v->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                v->data = NULL;
                return;
            }
            ((float *)v->data)[i] = temp_out;
        }
        break;
    }
    case TYPE_DOUBLE:
    {
        double temp_out = 0;
        for (int i = 0; i < v->size; ++i)
        {
            if (func(&((double *)v->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                v->data = NULL;
                return;
            }
            ((double *)v->data)[i] = temp_out;
        }
        break;
    }
    default:
        break;
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
void applyToMatrix(Matrix *m, int (*func)(void *, DataType, void *), DataType type)
{
    switch (type)
    {
    case TYPE_INT:
    {
        int temp_out = 0;
        for (int i = 0; i < m->rows * m->cols; ++i)
        {
            if (func(&((int *)m->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                m->data = NULL;
                return;
            }
            ((int *)m->data)[i] = temp_out;
        }
        break;
    }
    case TYPE_FLOAT:
    {
        float temp_out = 0;
        for (int i = 0; i < m->rows * m->cols; ++i)
        {
            if (func(&((float *)m->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                m->data = NULL;
                return;
            }
            ((float *)m->data)[i] = temp_out;
        }
        break;
    }
    case TYPE_DOUBLE:
    {
        double temp_out = 0;
        for (int i = 0; i < m->rows * m->cols; ++i)
        {
            if (func(&((double *)m->data)[i], type, &temp_out) < 0)
            {
                printf("Error applying function to each element in vector.\n");
                m->data = NULL;
                return;
            }
            ((double *)m->data)[i] = temp_out;
        }
        break;
    }
    default:
        break;
    }

    return;
}
