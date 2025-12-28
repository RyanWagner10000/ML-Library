/*
 * file: eval_metrics.h
 * description: header file that gives access to all the evaluation metric funcitons
 * author: Ryan Wagner
 * date: July 1,, 2025
 * notes:
 */

#ifndef EVAL_METRICS_H
#define EVAL_METRICS_H

#include "vector.h"
#include "file_handling.h"
#include "enums_and_structs.h"

int initEvalMetrics(EvalMetrics *eval_metrics, Matrix y_pred, RegressionType type);
int applyLabelThreshold(Matrix y_pred, Matrix *y_labels, double threshold);

int computeConfusionMatrix(Matrix y_true, Matrix y_pred, int *TP, int *FP, int *TN, int *FN);
int computeAccuracy(int TP, int FP, int TN, int FN, double *accuracy);
int computePrecision(int TP, int FP, double *precision);
int computeRecall(int TP, int FN, double *recall);
int computeF1(double precision, double recall, double *f1);

int computeMSE(Matrix y_true, Matrix y_pred, double *mse);
int computeRMSE(Matrix y_true, Matrix y_pred, double *rmse);
int computeMAE(Matrix y_true, Matrix y_pred, double *mae);
int computeR2Score(Matrix y_true, Matrix y_pred, double *r2score);

int calculateAllMetrics(EvalMetrics *eval_metrics, RegressionType model_type, Matrix true_labels);
int printMetrics(EvalMetrics eval_metrics, RegressionType model_type);
void freeEvalMetrics(EvalMetrics *em);

#endif // EVAL_METRICS_H