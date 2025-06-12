/*
 * file: vector.h
 * description: header file for all vector related functions, create, free, set, get, etc.
 * author: Ryan Wagner
 * date: June 8, 2025
 * notes:
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"

typedef struct
{
    int size;
    void *data;
    DataType type;
} Vector;

void printVector(Vector *v);

void freeVector(Vector *v);

int makeVector(Vector *v, int size, void *data, DataType type);

int makeVectorZeros(Vector *v, int size, DataType type);

#endif // VECTOR_H