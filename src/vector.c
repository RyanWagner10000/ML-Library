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
        switch (v->type)
        {
        case TYPE_INT:
            printf("%d", ((int *)v->data)[i]);
            break;
        case TYPE_FLOAT:
            printf("%.2f", ((float *)v->data)[i]);
            break;
        case TYPE_DOUBLE:
            printf("%.2lf", ((double *)v->data)[i]);
            break;

        default:
            printf("?");
            break;
        }

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
 * @brief Function to set the value at an index of a Matrix
 *
 * @param m Pointer to Vector object
 * @param row Row index
 * @param col Col index
 * @param value Value to set in Vector v
 *
 * @return 0 if successful, -1 otherwise
 */
int setVectorValue(Vector *v, int idx, void *value)
{
    if (!v || !v->data || idx >= v->size || idx < 0)
    {
        printf("Out of bounds access to Vector at idx: %d\n", idx);
        return -1;
    }

    switch (v->type)
    {
    case TYPE_INT:
        ((int *)v->data)[idx] = *(int *)value;
        break;
    case TYPE_FLOAT:
        ((float *)v->data)[idx] = *(float *)value;
        break;
    case TYPE_DOUBLE:
        ((double *)v->data)[idx] = *(double *)value;
        break;
    default:
        break;
    }

    return 0;
}

/**
 * @brief Makes a Vector object from an array and a size
 *
 * @param v pointer to Vector object to make
 * @param size of the array being put into Vector
 * @param data Optional data to input into Vector, NULL otherwise
 * @param type DataType enum to make this Vector
 *
 * @return Vector object filled with input array and size
 */
int makeVector(Vector *v, int size, void *data, DataType type)
{
    // Create vector and allocate memory the size of input array
    v->size = size;
    v->type = type;

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

    if (elem_size == 0 || size <= 0)
    {
        v->data = NULL;
        return -1;
    }
    else
    {
        v->data = malloc(size * elem_size);
        if (!v->data)
        {
            printf("Failed to allocate vector\n");
        }
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
                if (setVectorValue(v, i, &((int *)data)[i]) < 0)
                {
                    printf("Setting of input data to Vector was unsuccessful\n");
                    v->data = NULL;
                    return -1;
                }
            }
            break;
        }
        case TYPE_FLOAT:
        {
            for (int i = 0; i < size; ++i)
            {
                if (setVectorValue(v, i, &((float *)data)[i]) < 0)
                {
                    printf("Setting of input data to Vector was unsuccessful\n");
                    v->data = NULL;
                    return -1;
                }
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            for (int i = 0; i < size; ++i)
            {
                if (setVectorValue(v, i, &((double *)data)[i]) < 0)
                {
                    printf("Setting of input data to Vector was unsuccessful\n");
                    v->data = NULL;
                    return -1;
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
 * @brief Makes a Vector object of 0's given a size
 *
 * @param v pointer to Vector object to make
 * @param size of the array being put into Vector
 * @param type DataType enum to make this Vector
 *
 * @return Vector object filled with input array and size
 */
int makeVectorZeros(Vector *v, int size, DataType type)
{
    // Create vector and allocate memory the size of input array
    v->size = size;
    v->type = type;

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

    if (elem_size == 0 || size <= 0)
    {
        v->data = NULL;
    }
    else
    {
        v->data = calloc(size, elem_size);
        if (!v->data)
        {
            printf("Failed to allocate vector\n");
        }
    }

    return 0;
}
