/*
 * file: matrix.h
 * description: header file for all matrix related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
// #include "../header/vector.h"

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE
} DataType;

typedef struct
{
    int rows;
    int cols;
    double *data;
} Matrix;

void printMatrix(Matrix *m);

void freeMatrix(Matrix *m);

int makeMatrix(Matrix *m, int rows, int cols, void *data, DataType type);

int makeMatrixZeros(Matrix *m, int rows, int cols);

int deleteColMatrix(Matrix *m, int col);

#endif // MATRIX_H