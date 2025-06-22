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

            printf("%.2lf", m->data[idx]);

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

/**
 * @brief Makes a Matrix object
 *
 * @param rows for matrix
 * @param cols for matrix
 * @param data Optional data to input into Matrix, NULL otherwise
 * @param type DataType enum of input data
 *
 * @return 0 if successful, -1 otherwise
 */
int makeMatrix(Matrix *m, int rows, int cols, void *data, DataType type)
{
    // Create Matrix and assign basic members
    m->rows = rows;
    m->cols = cols;

    m->data = malloc(rows * cols * sizeof(double));
    if (!m->data)
    {
        printf("Failed to allocate matrix\n");
        return -1;
    }

    // Assign values to matrix if given
    int idx_m = 0;
    int idx_d = 0;
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
                    idx_m = r * cols + c;
                    m->data[idx_m] = (double)((int *)data)[idx_d];
                    idx_d++;
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
                    idx_m = r * cols + c;
                    m->data[idx_m] = (double)((float *)data)[idx_d];
                    idx_d++;
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
                    idx_m = r * cols + c;
                    m->data[idx_m] = ((double *)data)[idx_d];
                    idx_d++;
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
 * @param m pointer to Matrix object to make
 * @param rows for matrix
 * @param cols for matrix
 *
 * @return 0 if successful, -1 otherwise
 */
int makeMatrixZeros(Matrix *m, int rows, int cols)
{
    if (rows <= 0 || cols <= 0)
    {
        m->data = NULL;
        return -1;
    }
    // Create Matrix and assign basic members
    m->rows = rows;
    m->cols = cols;

    m->data = calloc(rows * cols, sizeof(double));
    if (!m->data)
    {
        printf("Failed to allocate matrix\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Remove column from matrix
 *
 * @param m pointer to Matrix object to edit
 * @param col to delete from Matrix m
 *
 * @return 0 if successful, -1 otherwise
 */
int deleteColMatrix(Matrix *m, int col)
{
    if (!m || !m->data || col >= m->cols || col < 0)
    {
        printf("Error deleting column #%d from Matrix.\n", col);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    int new_cols = m->cols - 1;
    int col_counter = 0;

    double *temp_array = calloc((m->cols - 1) * m->rows, sizeof(double));
    for (int r = 0; r < m->rows; ++r)
    {
        col_counter = 0;
        for (int c = 0; c < m->cols; ++c)
        {
            if (c != col)
            {
                ((double *)temp_array)[r * new_cols + col_counter] = ((double *)m->data)[r * m->cols + c];
                ++col_counter;
            }
        }
    }

    // Free the old matrix data, then copy the new stuff over
    free(m->data);
    memcpy(m->data, temp_array, m->rows * new_cols * sizeof(double));
    free(temp_array);

    --m->cols;

    return 0;
}
