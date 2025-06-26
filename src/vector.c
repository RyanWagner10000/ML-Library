/*
 * file: vector.c
 * description: file for all vector related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#include "../header/vector.h"

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
        printf("%.2lf", ((double *)v->data)[i]);

        if (i < v->size - 1)
            printf(", ");
    }
    printf("]\n");
}

/**
 * @brief Free Matrix and set poitner to NULL
 *
 * @param m Matrix to free
 *
 * @return None
 */
void freeVector(Vector *v)
{
    if (v && v->data)
    {
        free(v->data);
        v->data = NULL;
    }
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
    if (size <= 0)
    {
        printf("Size <= 0 when making this vector.\n");
        v->data = NULL;
        return -1;
    }

    // Create vector and allocate memory the size of input array
    v->size = size;

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
    if (!v || !v->data || elem >= v->size || elem < 0)
    {
        printf("Error deleting element #%d from Vector", elem);
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
            temp_array[elem_counter] = v->data[elem_counter];
            ++elem_counter;
        }
    }

    // Free the old vector data, then copy the new stuff over
    free(v->data);
    memcpy(v->data, temp_array, new_size * sizeof(double));
    free(temp_array);

    --v->size;

    return 0;
}

/**
 * @brief Get column from matrix and fill vector
 *
 * @param m pointer to Matrix object to edit
 * @param col to get data from Matrix m
 * @param v pointer to Vector object to fill
 *
 * @return 0 if successful, -1 otherwise
 */
int getColMatrix(Matrix *m, int col, Vector *v)
{
    if (!m || !m->data || col >= m->cols || col < 0 || !v || !v->data)
    {
        printf("Error getting column #%d from Matrix for Vector.\n", col);
        printf("Could not pass initial tests.\n");
        return -1;
    }

    freeVector(v);
    if (makeVectorZeros(v, m->rows) < 0)
    {
        printf("Error initializing zero vector.\n");
        return -1;
    }

    int elem_num = 0;

    for (int r = 0; r < m->rows; ++r)
    {
        for (int c = 0; c < m->cols; ++c)
        {
            if (c == col)
            {
                v->data[elem_num] = m->data[r * m->cols + c];
                elem_num++;
            }
        }
    }

    return 0;
}