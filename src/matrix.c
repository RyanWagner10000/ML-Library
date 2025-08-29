/*
 * file: matrix.c
 * description: file for all matrix related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#include "../header/matrix.h"

/**
 * @brief Clears a Matrix by making all values 0
 *
 * @param m Matrix to clear
 *
 * @return 0 on success and -1 on failure
 */
int clearMatrix(Matrix *m)
{
    if (!m || !m->data)
    {
        printf("Incompatible input to clearMatrix operation.\n");
        return -1;
    }

    for (int r = 0; r < m->rows; ++r)
    {
        for (int c = 0; c < m->cols; ++c)
        {
            m->data[r * m->cols + c] = 0.0;
        }
    }

    return 0;
}

/**
 * @brief Copy a Matrix to another Matrix
 *
 * @param m Matrix with values to copy
 * @param mc Matrix to get copied valeus
 *
 * @return 0 on success and -1 on failure
 */
int copyMatrix(Matrix m, Matrix *mc)
{
    if (!m.data || !mc || !mc->data)
    {
        printf("Incompatible input to copyMatrix operation.\n");
        return -1;
    }
    if (m.rows != mc->rows || m.cols != mc->cols)
    {
        printf("Matrices are not the same shape. Cannot perform copy operation.\n");
        return -1;
    }

    for (int r = 0; r < m.rows; ++r)
    {
        for (int c = 0; c < m.cols; ++c)
        {
            mc->data[r * mc->cols + c] = m.data[r * m.cols + c];
        }
    }

    return 0;
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param m Matrix object to print
 *
 * @return None
 */
void printMatrix(Matrix m)
{
    for (int r = 0; r < m.rows; ++r)
    {
        printf("[");
        for (int c = 0; c < m.cols; ++c)
        {
            int idx = r * m.cols + c;

            printf("%.6lf", m.data[idx]);

            if (c < m.cols - 1)
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
    if (m && m->data != NULL)
    {
        free(m->data);
        m->data = NULL;
    }
}

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
        printf("Input row {%d} or col {%d} value(s) was incompatible.\n", rows, cols);
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
                temp_array[r * new_cols + col_counter] = m->data[r * m->cols + c];
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

/**
 * @brief Remove row from matrix
 *
 * @param m pointer to Matrix object to edit
 * @param row to delete from Matrix m
 *
 * @return 0 if successful, -1 otherwise
 */
int deleteRowMatrix(Matrix *m, int row)
{
    if (!m || !m->data || row >= m->rows || row < 0)
    {
        printf("Error deleting row #%d from Matrix.\n", row);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    int new_rows = m->rows - 1;

    double *temp_array = calloc(new_rows * m->cols, sizeof(double));
    int array_row = 0;
    for (int r = 0; r < m->rows; ++r)
    {
        if (r != row)
        {
            for (int c = 0; c < m->cols; ++c)
            {
                temp_array[array_row * m->cols + c] = m->data[r * m->cols + c];
            }
            array_row++;
        }
    }

    // Free the old matrix data, then copy the new stuff over
    free(m->data);
    m->data = malloc(new_rows * m->cols * sizeof(double));
    memcpy(m->data, temp_array, new_rows * m->cols * sizeof(double));
    free(temp_array);

    --m->rows;

    return 0;
}
