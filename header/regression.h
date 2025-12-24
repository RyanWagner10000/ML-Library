/*
 * file: regression.h
 * description: header file that gives access to all the regression funcitons
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#ifndef REGRESSION_H
#define REGRESSION_H

#include "math_funcs.h"
#include "eval_matrics.h"
#include "enums_and_structs.h"

ModelConfig makeDefaultConfig();

ModelMetrics makeDefaultMetrics();

int comptueLabels(Matrix X, Matrix weights, Vector biases, Matrix *labels, Activation activation);

int initModel(Model *model);

int testModel(Model *model);

int trainModel(Model *model);

void freeModel(Model *model);

#endif // REGRESSION_H