/*
 * file: vector.h
 * description: header file for all vector related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "../header/matrix.h"

typedef struct
{
    int size;
    double *data;
} Vector;

void printVector(Vector *v);

void freeVector(Vector *v);

int makeVector(Vector *v, int size, void *data, DataType type);

int makeVectorZeros(Vector *v, int size);

int deleteElemVector(Vector *v, int elem);

int getColMatrix(Matrix *m, int col, Vector *v);

#endif // VECTOR_H