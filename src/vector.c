/*
 * file: vector.c
 * description: file for all vector related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#include "../header/vector.h"

/**
 * @brief Clears a Vector by making all values 0
 *
 * @param v Vector to clear
 *
 * @return 0 on success and -1 on failure
 */
int clearVector(Vector *v)
{
    if (!v || !v->data)
    {
        printf("Incompatible input to clearVector operation,\n");
        return -1;
    }

    for (int i = 0; i < v->size; ++i)
    {
        v->data[i] = 0.0;
    }

    return 0;
}

/**
 * @brief Basic printing of a vector object
 *
 * @param v Vector object to print
 *
 * @return None
 */
void printVector(Vector v)
{
    printf("[");
    for (int i = 0; i < v.size; ++i)
    {
        printf("%.6lf", ((double *)v.data)[i]);

        if (i < v.size - 1)
            printf(", ");
    }
    printf("]\n");
}

/**
 * @brief Free Vector and set poitner to NULL
 *
 * @param v Vector to free
 *
 * @return None
 */
void freeVector(Vector *v)
{
    // Test inputs
    if (v && v->data)
    {
        free(v->data);
        v->data = NULL;
    }
}

/**
 * @brief Copy Vector values from one to another
 *
 * @param v1 Vector to copy values FROM
 * @param v2 Vector to copy values TO
 *
 * @return 0 if successful, -1 otherwise
 */
int copyVector(Vector v1, Vector *v2)
{
    if (!v1.data)
    {
        printf("Input vector V1 was not properly initialized.\n");
        return -1;
    }
    if (!v2 || !v2->data)
    {
        printf("Input vector V2 was not properly initialized.\n");
        return -1;
    }
    if (v1.size != v2->size)
    {
        printf("Input vector V1 and V2 do not have matching sizes.\n");
        return -1;
    }

    for (int i = 0; i < v1.size; ++i)
    {
        v2->data[i] = v1.data[i];
    }
    return 0;
}

/**
 * @brief Makes a Vector object from an array and a size
 *
 * @param v pointer to Vector object to make
 * @param size of the array being put into Vector
 * @param data Optional data to input into Vector, NULL otherwise
 * @param type DataType enum of input data
 *
 * @return 0 if successful, -1 otherwise
 */
int makeVector(Vector *v, int size, void *data, DataType type)
{
    // Test inputs
    if (size <= 0)
    {
        v->data = NULL;
        return -1;
    }

    // Create vector and allocate memory the size of input array
    v->size = size;

    v->data = malloc(size * sizeof(double));
    if (!v->data)
    {
        printf("Failed to allocate vector\n");
        return -1;
    }

    // Assign values to matrix if given
    if (data != NULL)
    {
        switch (type)
        {
        case TYPE_INT:
        {
            for (int i = 0; i < size; ++i)
            {
                ((double *)v->data)[i] = (double)((int *)data)[i];
            }
            break;
        }
        case TYPE_FLOAT:
        {
            for (int i = 0; i < size; ++i)
            {
                ((double *)v->data)[i] = (double)((float *)data)[i];
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            for (int i = 0; i < size; ++i)
            {
                v->data[i] = ((double *)data)[i];
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
 * @brief Makes a Vector object of 0's given a size
 *
 * @param v pointer to Vector object to make
 * @param size of the array being put into Vector
 *
 * @return 0 if successful, -1 otherwise
 */
int makeVectorZeros(Vector *v, int size)
{
    // Test inputs
    if (size <= 0)
    {
        printf("Size <= 0 when making this vector.\n");
        v->data = NULL;
        return -1;
    }

    // Create vector and allocate memory the size of input array
    v->size = size;

    // Use calloc to set memory size for Vector data and set values to 0.0
    v->data = calloc(size, sizeof(double));
    if (!v->data)
    {
        printf("Failed to allocate vector\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Remove element from Vector
 *
 * @param v pointer to Vector object to edit
 * @param elem to delete from Vector v
 *
 * @return 0 if successful, -1 otherwise
 */
int deleteElemVector(Vector *v, int elem)
{
    // Test inputs
    if (!v || !v->data || elem >= v->size || elem < 0)
    {
        printf("Error deleting element #%d from Vector.\n", elem);
        return -1;
    }

    int new_size = v->size - 1;
    int elem_counter = 0;

    // Make temp array with data w/o desired elemeent;
    double *temp_array = calloc(v->size - 1, sizeof(double));

    for (int e = 0; e < v->size; ++e)
    {
        if (e != elem)
        {
            temp_array[elem_counter] = v->data[e];
            ++elem_counter;
        }
    }

    // Free the old vector data, then copy the new stuff over
    free(v->data);
    v->data = malloc(new_size * sizeof(double));
    memcpy(v->data, temp_array, new_size * sizeof(double));
    free(temp_array);

    --v->size;

    return 0;
}

/**
 * @brief Get column from matrix and fill vector
 *
 * @param m Matrix object
 * @param col Column index
 * @param v Pointer to Vector object to fill
 *
 * @return 0 if successful, -1 otherwise
 */
int getColMatrix_v(Matrix m, int col, Vector *v)
{
    // Test inputs
    if (!m.data || col >= m.cols || col < 0 || !v)
    {
        printf("Error getting column #%d from Matrix for Vector.\n", col);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    if (!initialized_vector(v) || v->size <= 0)
    {
        if (makeVectorZeros(v, m.rows) < 0)
        {
            printf("Error initializing zero vector.\n");
            return -1;
        }
    }
    else
    {
        if (clearVector(v) < 0)
        {
            printf("Could not set Matrix object to all 0's\n");
            return -1;
        }
    }

    for (int r = 0; r < m.rows; ++r)
    {
        v->data[r] = m.data[r * m.cols + col];
    }

    return 0;
}

/**
 * @brief Get column from matrix and fill matrix
 *
 * @param m Matrix object with data
 * @param col Column index
 * @param mf Pointer to Matrix object to fill
 *
 * @return 0 if successful, -1 otherwise
 */
int getColMatrix_m(Matrix m, int col, Matrix *mf)
{
    // Test inputs
    if (!m.data || !mf || col >= m.cols || col < 0)
    {
        printf("Error getting column #%d from Matrix for Matrix.\n", col);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    freeMatrix(mf);
    if (makeMatrixZeros(mf, m.rows, 1) < 0)
    {
        printf("Recreating mf matrix was unsuccessful.\n");
        return -1;
    }

    int index = 0;
    for (int r = 0; r < m.rows; ++r)
    {
        index = r * m.cols + col;
        mf->data[r] = m.data[index];
    }

    return 0;
}

/**
 * @brief Get row from matrix and fill vector
 *
 * @param m Matrix object
 * @param row Row index
 * @param v Pointer to Vector object to fill
 *
 * @return 0 if successful, -1 otherwise
 */
int getRowMatrix_v(Matrix m, int row, Vector *v)
{
    // Test inputs
    if (!m.data || row >= m.rows || row < 0 || !v)
    {
        printf("Error getting row #%d from Matrix for Vector.\n", row);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    if (!initialized_vector(v) || v->size <= 0)
    {
        if (makeVectorZeros(v, m.cols) < 0)
        {
            printf("Error initializing zero vector.\n");
            return -1;
        }
    }
    else
    {
        if (clearVector(v) < 0)
        {
            printf("Could not set Matrix object to all 0's\n");
            return -1;
        }
    }

    v->size = m.cols;

    for (int i = 0; i < v->size; ++i)
    {
        v->data[i] = m.data[row * m.cols + i];
    }

    return 0;
}

/**
 * @brief Get row from matrix and fill matrix
 *
 * @param m Matrix object
 * @param row Row index
 * @param mf Pointer to Matrix object to fill
 *
 * @return 0 if successful, -1 otherwise
 */
int getRowMatrix_m(Matrix m, int row, Matrix *mf)
{
    // Test inputs
    if (!m.data || row >= m.rows || row < 0 || !mf)
    {
        printf("Error getting row #%d from Matrix for Matrix.\n", row);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    freeMatrix(mf);
    if (makeMatrixZeros(mf, 1, m.cols) < 0)
    {
        printf("Recreating mf matrix was unsuccessful.\n");
        return -1;
    }

    for (int c = 0; c < m.cols; ++c)
    {
        mf->data[c] = m.data[row * m.cols + c];
    }

    return 0;
}

/**
 * @brief Set column in matrix with vector values
 *
 * @param m Matrix object to edit
 * @param col Column index
 * @param v Pointer to Vector object with values
 *
 * @return 0 if successful, -1 otherwise
 */
int setColMatrix(Matrix *m, int col, Vector v)
{
    // Test inputs
    if (!m->data || col >= m->cols || col < 0 || !v.data || v.size != m->rows)
    {
        printf("Error setting column #%d in Matrix with Vector.\n", col);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    for (int r = 0; r < m->rows; ++r)
    {
        m->data[r * m->cols + col] = v.data[r];
    }

    return 0;
}

/**
 * @brief Set row in matrix with vector values
 *
 * @param m Matrix object to edit
 * @param row Row index
 * @param v Pointer to Vector object with values
 *
 * @return 0 if successful, -1 otherwise
 */
int setRowMatrix(Matrix *m, int row, Vector v)
{
    // Test inputs
    if (!m->data || row >= m->rows || row < 0 || !v.data || v.size != m->cols)
    {
        printf("Error setting row #%d from Matrix for Vector.\n", row);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    for (int c = 0; c < m->cols; ++c)
    {
        m->data[row * m->cols + c] = v.data[c];
    }

    return 0;
}
