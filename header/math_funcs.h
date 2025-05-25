// main header file
#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

typedef struct
{
    double *data;
    int rows;
    int cols;
} Matrix;

typedef struct
{
    double *data;
    int size;
} Vector;

Vector makeVector(double *array, int size);
Matrix makeMatrix(double *array, int rows, int cols);

void printVector(Vector *v);
void printMatrix(Matrix *m);

int dot_product(Vector *x, Vector *y, double *result);
int matvec_mult(Matrix *A, Vector *y, Vector *result);

int transpose(Matrix *A, Matrix *A_t);
int identity(Matrix *A, int size);

double sigmoid(double x);
double sigmoid_dx(double x);
double relu(double x);
double relu_dx(double x);
double tanh_dx(double x);
void applyToVector(Vector *v, double (*func)(double));
void applyToMatrix(Matrix *m, double (*func)(double));


#endif