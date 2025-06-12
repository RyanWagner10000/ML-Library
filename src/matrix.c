/*
 * file: matrix.c
 * description: file for all matrix related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#include "../header/matrix.h"

/**
 * @brief Basic printing of a Matrix object
 *
 * @param m Matrix object to print
 *
 * @return None
 */
void printMatrix(Matrix *m)
{
    for (int r = 0; r < m->rows; ++r)
    {
        printf("[");
        for (int c = 0; c < m->cols; ++c)
        {
            int idx = r * m->cols + c;

            switch (m->type)
            {
            case TYPE_INT:
                printf("%d", ((int *)m->data)[idx]);
                break;
            case TYPE_FLOAT:
                printf("%.2f", ((float *)m->data)[idx]);
                break;
            case TYPE_DOUBLE:
                printf("%.2lf", ((double *)m->data)[idx]);
                break;

            default:
                printf("?");
                break;
            }

            if (c < m->cols - 1)
                printf(", ");
        }
        printf("]\n");
    }
}

/**
 * @brief Free Matrix and set poitner to NULL
 *
 * @param m Matrix to free
 *
 * @return None
 */
void freeMatrix(Matrix *m)
{
    if (m && m->data)
    {
        free(m->data);
        m->data = NULL;
    }
}

/**
 * @brief Function to set the value at an index of a Matrix
 *
 * @param m Pointer to Matrix object
 * @param row Row index
 * @param col Col index
 * @param value Value to set in Matrix m
 *
 * @return 0 if successful, -1 otherwise
 */
int setMatrixValue(Matrix *m, int row, int col, void *value)
{
    if (!m || !m->data || row >= m->rows || row < 0 || col >= m->cols || col < 0)
    {
        printf("Out of bounds access to Matrix at row: %d col: %d\n", row, col);
        return -1;
    }

    int idx = row * m->cols + col;

    switch (m->type)
    {
    case TYPE_INT:
    {
        ((int *)m->data)[idx] = *(int *)value;
        break;
    }
    case TYPE_FLOAT:
    {
        ((float *)m->data)[idx] = *(float *)value;
        break;
    }
    case TYPE_DOUBLE:
    {
        ((double *)m->data)[idx] = *(double *)value;
        break;
    }
    default:
        break;
    }

    return 0;
}

/**
 * @brief Makes a Matrix object
 *
 * @param rows for matrix
 * @param cols for matrix
 * @param data Optional data to input into Matrix, NULL otherwise
 * @param type DataType enum to make this matrix
 *
 * @return Matrix object
 */
int makeMatrix(Matrix *m, int rows, int cols, void *data, DataType type)
{
    // Create Matrix and assign basic members
    m->rows = rows;
    m->cols = cols;
    m->type = type;

    // Calculate the size of memory to allocate given type
    size_t elem_size;
    switch (type)
    {
    case TYPE_INT:
    {
        elem_size = sizeof(int);
        break;
    }
    case TYPE_FLOAT:
    {
        elem_size = sizeof(float);
        break;
    }
    case TYPE_DOUBLE:
    {
        elem_size = sizeof(double);
        break;
    }
    default:
    {
        elem_size = 0;
        break;
    }
    }

    // Malloc memory for Matrix
    if (elem_size == 0 || rows <= 0 || cols <= 0)
    {
        m->data = NULL;
    }
    else
    {
        m->data = malloc(rows * cols * elem_size);
        if (!m->data)
        {
            printf("Failed to allocate matrix\n");
            return -1;
        }
    }

    // Assign values to matrix if given
    int idx = 0;
    if (data != NULL)
    {
        switch (type)
        {
        case TYPE_INT:
        {
            for (int r = 0; r < rows; ++r)
            {
                for (int c = 0; c < cols; ++c)
                {
                    idx = r * cols + c;
                    if (setMatrixValue(m, r, c, &((int *)data)[idx]) < 0)
                    {
                        printf("Setting of input data to Matrix was unsuccessful\n");
                        m->data = NULL;
                        return -1;
                    }
                }
            }
            break;
        }
        case TYPE_FLOAT:
        {
            for (int r = 0; r < rows; ++r)
            {
                for (int c = 0; c < cols; ++c)
                {
                    idx = r * cols + c;
                    if (setMatrixValue(m, r, c, &((float *)data)[idx]) < 0)
                    {
                        printf("Setting of input data to Matrix was unsuccessful\n");
                        m->data = NULL;
                        return -1;
                    }
                }
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            for (int r = 0; r < rows; ++r)
            {
                for (int c = 0; c < cols; ++c)
                {
                    idx = r * cols + c;
                    if (setMatrixValue(m, r, c, &((double *)data)[idx]) < 0)
                    {
                        printf("Setting of input data to Matrix was unsuccessful\n");
                        m->data = NULL;
                        return -1;
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return 0;
}

/**
 * @brief Makes a Matrix object of 0's given a size
 *
 * @param v pointer to Matrix object to make
 * @param rows for matrix
 * @param cols for matrix
 * @param type DataType enum to make this Matrix
 *
 * @return Matrix object filled with input array and size
 */
int makeMatrixZeros(Matrix *m, int rows, int cols, DataType type)
{
    // Create Matrix and assign basic members
    m->rows = rows;
    m->cols = cols;
    m->type = type;

    // Calculate the size of memory to allocate given type
    size_t elem_size;
    switch (type)
    {
    case TYPE_INT:
        elem_size = sizeof(int);
        break;
    case TYPE_FLOAT:
        elem_size = sizeof(float);
        break;
    case TYPE_DOUBLE:
        elem_size = sizeof(double);
        break;
    default:
        elem_size = 0;
        break;
    }

    // Malloc memory for Matrix
    if (elem_size == 0 || rows <= 0 || cols <= 0)
    {
        m->data = NULL;
    }
    else
    {
        m->data = calloc(rows * cols, elem_size);
        if (!m->data)
        {
            printf("Failed to allocate matrix\n");
            return -1;
        }
    }

    return 0;
}
