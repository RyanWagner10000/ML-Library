/*
 * file: eval_metrics.c
 * description: script of all evaluation metric functions
 * author: Ryan Wagner
 * date: July 1, 2025
 * notes: script encompasses confusion matris, accuracy, precision, recall, and f1
 */

#include "../header/eval_matrics.h"

/**
 * @brief Fill a created EvalMetrics object with default values
 *
 * @param eval_metrics Pointer to EvalMetrics object
 * @param y_pred Matrix pointer to predicted values from training
 * @param type RegressionType enum of the trained models regression type
 *
 * @return 0 if successful, -1 if failure
 */
int initEvalMetrics(EvalMetrics *eval_metrics, Matrix y_pred, RegressionType type)
{
    // Copy the predicted values to the predicted labels Matrix
    eval_metrics->y_lables = malloc(sizeof(Matrix));

    if (makeMatrixZeros(eval_metrics->y_lables, y_pred.rows, y_pred.cols) < 0)
    {
        printf("Could not initialize y_labels Matrix for EvalMetrics object.\n");
        return -1;
    }
    if (copyMatrix(y_pred, eval_metrics->y_lables) < 0)
    {
        printf("Could not copy y_pred to y_labels Matrix for EvalMetrics object.\n");
        return -1;
    }

    // Set the default threshold value appropriately based on the regression type
    switch (type)
    {
    case LINEAR_REGRESSION:
    {
        eval_metrics->threshold = 1.0;
        break;
    }
    case LOGISTIC_REGRESSION:
    {
        eval_metrics->threshold = 0.5;
        break;
    }
    case SOFTMAX_REGRESSION:
    {
        eval_metrics->threshold = 0.5;
        break;
    }
    default:
    {
        break;
    }
    }

    eval_metrics->TP = 0;
    eval_metrics->FP = 0;
    eval_metrics->TN = 0;
    eval_metrics->FN = 0;
    eval_metrics->accuracy = 0.0;
    eval_metrics->precision = 0.0;
    eval_metrics->recall = 0.0;
    eval_metrics->f1 = 0.0;
    eval_metrics->mse = 0.0;
    eval_metrics->rmse = 0.0;
    eval_metrics->mae = 0.0;
    eval_metrics->r2score = 0.0;

    return 0;
}

/**
 * @brief Takes the predicted values and computes the labels based on a threshold value
 *
 * @param y_true Matrix with predicted values
 * @param y_pred Pointer to Matrix for output of predicted classification values
 * @param threshold Threshold value for classification
 *
 * @return 0 if successful, -1 if failure
 */
int applyLabelThreshold(Matrix y_pred, Matrix *y_labels, double threshold)
{
    if (y_labels == NULL || y_labels->data == NULL || y_pred.data == NULL || y_labels->rows != y_pred.rows || y_labels->cols != y_pred.cols)
    {
        printf("Failed to predict labels.\n");
        return -1;
    }

    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r * y_labels->cols + c;
            y_labels->data[idx] = (y_pred.data[idx] >= threshold) ? 1 : 0;
        }
    }

    return 0;
}

/**
 * @brief
 *
 * @param y_true Pointer to Matrix with real/known values
 * @param y_pred Pointer to Matrix with predicted classification values
 * @param TP Int pointer for True Positive
 * @param FP Int pointer for False Positive
 * @param TN Int pointer for True Negative
 * @param FN Int pointer for False Negative
 *
 * @return 0 if successful, -1 if failure
 */
int computeConfusionMatrix(Matrix y_true, Matrix y_pred, int *TP, int *FP, int *TN, int *FN)
{
    if (y_true.data == NULL || y_pred.data == NULL || y_true.rows != y_pred.rows || y_true.cols != y_pred.cols)
    {
        printf("Failed to compute confusion matrix.\n");
        return -1;
    }

    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r + y_true.cols + c;
            if ((int)y_true.data[idx] == 0 && (int)y_pred.data[idx] == 0)
            {
                ++*TN;
            }
            else if ((int)y_true.data[idx] == 0 && (int)y_pred.data[idx] == 1)
            {
                ++*FP;
            }
            else if ((int)y_true.data[idx] == 1 && (int)y_pred.data[idx] == 0)
            {
                ++*FN;
            }
            else if ((int)y_true.data[idx] == 1 && (int)y_pred.data[idx] == 1)
            {
                ++*TP;
            }
            else
            {
                printf("Problem with computing confusing matrix at an index.\n");
                printf(" y_true[%d x %d] = %d\n", r, c, (int)y_true.data[idx]);
                printf(" y_pred[%d x %d] = %d\n", r, c, (int)y_pred.data[idx]);
                return -1;
            }
        }
    }

    return 0;
}

/**
 * @brief Function to calcualte the accuracy of a model given confusion matrix values
 *
 * @param TP Int for True Positive
 * @param FP Int for False Positive
 * @param TN Int for True Negative
 * @param FN Int for False Negative
 * @param accuracy Pointer to double for result of accuracy operation
 *
 * @return 0 if successful, -1 if failure
 */
int computeAccuracy(int TP, int FP, int TN, int FN, double *accuracy)
{
    double denom = TP + TN + FP + FN;
    if (fabs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; would result in error. Setting accuracy to 0.\n");
        *accuracy = 0.0;
        return 0;
    }

    *accuracy = (double)(TP + TN) / denom;

    return 0;
}

/**
 * @brief Function to calculate the precision of a model
 *
 * @param TP Int for True Positive
 * @param FP Int for False Positive
 * @param precision Pointer to double for result of precision operation
 *
 * @return 0 if successful, -1 if failure
 */
int computePrecision(int TP, int FP, double *precision)
{
    double denom = TP + FP;

    if (fabs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; would result in error. Setting Precision to 0.\n");
        *precision = 0.0;
    }

    *precision = (double)TP / (TP + FP);

    return 0;
}

/**
 * @brief Function to calculate the recall of a model
 *
 * @param TP Int for True Positive
 * @param FP Int for False Positive
 * @param recall Pointer to double for result of recall operation
 *
 * @return 0 if successful, -1 if failure
 */
int computeRecall(int TP, int FN, double *recall)
{
    double denom = TP + FN;
    if (fabs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; would result in error. Setting Recall to 0.\n");
        *recall = 0;
        return 0;
    }

    *recall = (double)TP / denom;

    return 0;
}

/**
 * @brief Function to calculate the F1 score of a model
 *
 * @param precision Double with precision value
 * @param recall Double with recall value
 * @param f1 Pointer to double for result of F1 operation
 *
 * @return 0 if successful, -1 if failure
 */
int computeF1(double precision, double recall, double *f1)
{
    double denom = precision + recall;
    if (fabs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value (%.4lf) is too close to 0; would result in error. Setting F1 to 0.\n", denom);
        *f1 = 0.0;
        return 0;
    }

    *f1 = 2.0 * (precision * recall) / denom;

    return 0;
}

/**
 * @brief Function to calculate the Mean Squared Error of two matrices
 *
 * @param y_true Matrix object with true y values for model
 * @param y_pred Matrix object with predicted y values for model
 * @param mse Pointer to double that will hold the mse value
 *
 * @return 0 if successful, -1 if failure
 */
int computeMSE(Matrix y_true, Matrix y_pred, double *mse)
{
    if (y_true.data == NULL || y_pred.data == NULL || y_true.rows != y_pred.rows || y_true.cols != y_pred.cols)
    {
        printf("Problem with input paramters for computing MSE.\n");
        return -1;
    }

    double sum_square = 0.0;
    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r + y_pred.cols + c;
            sum_square += (y_true.data[idx] - y_pred.data[idx]) * (y_true.data[idx] - y_pred.data[idx]);
        }
    }

    *mse = sum_square / (double)y_true.rows;

    return 0;
}

/**
 * @brief Function to calculate the Root Mean Squared Error of two Matrices
 *
 * @param y_true Pointer to Matrix object with true y values for model
 * @param y_pred Pointer to Matrix object with predicted y values for model
 * @param rmse Pointer to double that will hold the rmse value
 *
 * @return 0 if successful, -1 if failure
 */
int computeRMSE(Matrix y_true, Matrix y_pred, double *rmse)
{
    if (y_true.data == NULL || y_pred.data == NULL || y_true.rows != y_pred.rows || y_true.cols != y_pred.cols)
    {
        printf("Problem with input paramters for computing RMSE.\n");
        return -1;
    }

    double mse = 0.0;
    if (computeMSE(y_true, y_pred, &mse) < 0)
    {
        printf("MSE calculation was not successful when trying to calculate RMSE.\n");
        return -1;
    }

    *rmse = sqrt(mse);

    return 0;
}

/**
 * @brief Function to calculate the Mean Absolute Error of two matrices
 *
 * @param y_true Pointer to Matrix object with true y values for model
 * @param y_pred Pointer to Matrix object with predicted y values for model
 * @param mae Pointer to double that will hold the mae value
 *
 * @return 0 if successful, -1 if failure
 */
int computeMAE(Matrix y_true, Matrix y_pred, double *mae)
{
    if (y_true.data == NULL || y_pred.data == NULL || y_true.rows != y_pred.rows || y_true.cols != y_pred.cols)
    {
        printf("Problem with input paramters for computing MAE.\n");
        return -1;
    }

    double sum_absolute = 0.0;
    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r * y_pred.cols + c;
            sum_absolute += fabs(y_true.data[idx] - y_pred.data[idx]);
        }
    }

    *mae = sum_absolute / (double)y_true.rows;

    return 0;
}

/**
 * @brief Function to calculate the R^2 Score (Coefficient of Determination)
 *
 * @param y_true Pointer to Matrix object with true y values for model
 * @param y_pred Pointer to Matrix object with predicted y values for model
 * @param r2score Pointer to double that will hold the r2score value
 *
 * @return 0 if successful, -1 if failure
 */
int computeR2Score(Matrix y_true, Matrix y_pred, double *r2score)
{
    if (y_true.data == NULL || y_pred.data == NULL || y_pred.rows != y_true.rows || y_pred.cols != y_true.cols)
    {
        printf("Problem with input paramters for computing MAE.\n");
        return -1;
    }

    double y_true_avg = 0.0;
    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r * y_pred.cols + c;
            y_true_avg += y_true.data[idx];
        }
    }
    y_true_avg /= (double)y_true.rows;

    double sum_numer = 0.0;
    double sum_denom = 0.0;
    for (int r = 0; r < y_pred.rows; ++r)
    {
        for (int c = 0; c < y_pred.cols; ++c)
        {
            int idx = r * y_pred.cols + c;
            sum_numer += (y_true.data[idx] - y_pred.data[idx]) * (y_true.data[idx] - y_pred.data[idx]);
            sum_denom += (y_true.data[idx] - y_true_avg) * (y_true.data[idx] - y_true_avg);
        }
    }

    *r2score = 1.0 - (sum_numer / sum_denom);
    return 0;
}

/**
 * @brief Function to calculate all the metrics given the models type. Initialize EvalMetrics object prior to calulating metrics.
 *
 * @param eval_metrics Pointer to initialized EvalMetrics object with predicted labels for testing
 * @param model_type RegressionType enum of the model type
 * @param true_labels Matrix of the true labels or test labels
 *
 * @return 0 if successful, -1 if failure
 */
int calculateAllMetrics(EvalMetrics *eval_metrics, RegressionType model_type, Matrix true_labels)
{
    if (!eval_metrics || !eval_metrics->y_lables || !eval_metrics->y_lables->data || !true_labels.data)
    {
        printf("Eval Metrics object or predicted labels Matrix object is undefined.\n");
        return -1;
    }

    if (model_type == LINEAR_REGRESSION)
    {
        // Linear model gets MSE, RMSE, MAE, and R2 Score

        // MSE
        if (computeMSE(true_labels, *eval_metrics->y_lables, &eval_metrics->mse) < 0)
        {
            printf("Failure to compute MSE.\n");
            return -1;
        }

        // RMSE
        if (computeRMSE(true_labels, *eval_metrics->y_lables, &eval_metrics->rmse) < 0)
        {
            printf("Failure to compute RMSE.\n");
            return -1;
        }

        // MAE
        if (computeMAE(true_labels, *eval_metrics->y_lables, &eval_metrics->mae) < 0)
        {
            printf("Failure to compute MAE.\n");
            return -1;
        }

        // R2 Score
        if (computeR2Score(true_labels, *eval_metrics->y_lables, &eval_metrics->r2score) < 0)
        {
            printf("Failure to compute R2 Score.\n");
            return -1;
        }
    }
    else if (model_type == LOGISTIC_REGRESSION || model_type == SOFTMAX_REGRESSION)
    {
        // Logistic model gets confusion matrix, accuracy, precision, recall, and F1 score

        // Turn the predicted values into respective labels given threshold
        if (applyLabelThreshold(*eval_metrics->y_lables, eval_metrics->y_lables, eval_metrics->threshold) < 0)
        {
            printf("Operation to predict labels for Logistic or Softmax Regression was unsuccessful.\n");
            return -1;
        }

        // Confusion Matrix
        if (computeConfusionMatrix(true_labels, *eval_metrics->y_lables, &eval_metrics->TP, &eval_metrics->FP, &eval_metrics->TN, &eval_metrics->FN) < 0)
        {
            printf("Failure to compute Confusion Matrix.\n");
            return -1;
        }

        // Accuracy
        if (computeAccuracy(eval_metrics->TP, eval_metrics->FP, eval_metrics->TN, eval_metrics->FN, &eval_metrics->accuracy) < 0)
        {
            printf("Failure to compute Accuracy.\n");
            return -1;
        }

        // Precision
        if (computePrecision(eval_metrics->TP, eval_metrics->FP, &eval_metrics->precision) < 0)
        {
            printf("Failure to compute Precision.\n");
            return -1;
        }

        // Recall
        if (computeRecall(eval_metrics->TP, eval_metrics->FN, &eval_metrics->recall) < 0)
        {
            printf("Failure to compute Recall.\n");
            return -1;
        }

        // F1 Score
        if (computeF1(eval_metrics->precision, eval_metrics->recall, &eval_metrics->f1) < 0)
        {
            printf("Failure to compute F1 Score.\n");
            return -1;
        }
    }
    else
    {
        printf("Model type unknown\n");
        return -1;
    }

    if (printMetrics(*eval_metrics, model_type) < 0)
    {
        printf("Printing all performance metrics failed.\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Function to print all metrics for a model
 *
 * @param eval_metrics EvalMetrics object
 * @param model_type RegressionType enum of the model type
 * @param predicted_labels Matrix of the predicted labels
 *
 * @return 0 if successful, -1 if failure
 */
int printMetrics(EvalMetrics eval_metrics, RegressionType model_type)
{
    if (model_type == LINEAR_REGRESSION)
    {
        printf("\n");
        printf("     MSE = %.6lf\n", eval_metrics.mse);
        printf("     RMSE = %.6lf\n", eval_metrics.rmse);
        printf("     MAE = %.6lf\n", eval_metrics.mae);
        printf("     R2 Score = %.6lf\n", eval_metrics.r2score);
        printf("\n");
    }
    else if (model_type == LOGISTIC_REGRESSION || model_type == SOFTMAX_REGRESSION)
    {
        printf("\n");
        printf("     TP = %d\n", eval_metrics.TP);
        printf("     TN = %d\n", eval_metrics.TN);
        printf("     FP = %d\n", eval_metrics.FP);
        printf("     FN = %d\n", eval_metrics.FN);
        printf("     Accuracy = %.6lf\n", eval_metrics.accuracy);
        printf("     Precision = %.6lf\n", eval_metrics.precision);
        printf("     Recall = %.6lf\n", eval_metrics.recall);
        printf("     F1 Score = %.6lf\n", eval_metrics.f1);
        printf("\n");
    }
    else
    {
        printf("Model type unknown\n");
        return -1;
    }

    return 0;
}

/**
 * @brief Free EvalMetrics object and set poitner to NULL
 *
 * @param em EmalMetrics object to free
 *
 * @return None
 */
void freeEvalMetrics(EvalMetrics *em)
{
    if (em && em->y_lables)
    {
        free(em->y_lables);
        em->y_lables = NULL;
    }
}
