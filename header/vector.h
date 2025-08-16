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

#define initialized_vector(v) \
    ((v) && (v)->data != NULL && (v)->size > 0)

int clearVector(Vector *v);

void printVector(Vector v);

void freeVector(Vector *v);

int copyVector(Vector v1, Vector *v2);

int makeVector(Vector *v, int size, void *data, DataType type);

int makeVectorZeros(Vector *v, int size);

int deleteElemVector(Vector *v, int elem);

int getColMatrix(Matrix m, int col, Vector *v);

int getRowMatrix(Matrix m, int row, Vector *v);

#endif // VECTOR_H