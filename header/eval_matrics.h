/*
 * file: eval_metrics.h
 * description: header file that gives access to all the evaluation metric funcitons
 * author: Ryan Wagner
 * date: July 1,, 2025
 * notes:
 */

#ifndef EVAL_METRICS_H
#define EVAL_METRICS_H

#include "matrix.h"
#include "vector.h"
#include "regression.h"

typedef struct
{
    Matrix *y_lables; // Vector of predicted classes
    double threshold; // Threshold value for comparison of real to predicted values
    int TP;           // True Positive value
    int FP;           // False Positive value
    int TN;           // True Negative value
    int FN;           // False Negative value
    double accuracy;  // Fraction of correct predictions
    double precision; // How many predicted 1's are correct for binary classification
    double recall;    // How many actual 1's were caught  for binary classification
    double f1;        // harminic mean of precision and recall
    double mse;       // Mean Squared Error, lower is better
    double rmse;      // Root Mean Squared Error, in same units as the output
    double mae;       // Mean Absolute Error, less sensitive to outliers thatn MSE
    double r2score;   // Coefficient of Determination, how well the model explains variance in the data
} EvalMetrics;

int initEvalMetrics(EvalMetrics *eval_metrics, Matrix y_pred, RegressionType type);
int getPredictedLabels(Matrix y_pred, Matrix *y_labels, double threshold);

int computeConfusionMatrix(Matrix y_true, Matrix y_pred, int *TP, int *FP, int *TN, int *FN);
int computeAccuracy(int TP, int FP, int TN, int FN, double *accuracy);
int computePrecision(int TP, int FP, double *precision);
int computeRecall(int TP, int FN, double *recall);
int computeF1(double precision, double recall, double *f1);

int computeMSE(Matrix y_true, Matrix y_pred, double *mse);
int computeRMSE(Matrix y_true, Matrix y_pred, double *rmse);
int computeMAE(Matrix y_true, Matrix y_pred, double *mae);
int computeR2Score(Matrix y_true, Matrix y_pred, double *r2score);

int calculateAllMetrics(Model model, EvalMetrics *eval_metrics);
int printMetrics(Model model, EvalMetrics eval_metrics);
void freeEvalMetrics(EvalMetrics *em);

#endif // EVAL_METRICS_H