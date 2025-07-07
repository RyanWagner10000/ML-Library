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
 *
 * @return 0 if successful, -1 if failure
 */
int initEvalMetrics(EvalMetrics *eval_metrics, Vector *y_pred)
{
    eval_metrics->y_lables = malloc(sizeof(Vector));

    if (makeVectorZeros(eval_metrics->y_lables, y_pred->size) < 0)
    {
        printf("Could not initialize y_labels Vector for EvalMetrics object.\n");
        return -1;
    }

    for (int i = 0; i < y_pred->size; ++i)
    {
        eval_metrics->y_lables->data[i] = y_pred->data[i];
    }

    eval_metrics->threshold = 0.5;
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
 * @brief
 *
 * @param y_true Pointer to Vector with predicted values
 * @param y_pred Pointer to Vector for output of predicted classification values
 * @param threshold Threshold value for classification
 *
 * @return 0 if successful, -1 if failure
 */
int getPredictedLabels(Vector *y_pred, Vector *y_labels, double threshold)
{
    if (y_labels == NULL || y_pred == NULL || y_labels->data == NULL || y_pred->data == NULL || y_labels->size != y_pred->size)
    {
        printf("Failed to predict labels.\n");
        return -1;
    }

    for (int i = 0; i < y_pred->size; ++i)
    {
        y_labels->data[i] = (y_pred->data[i] >= threshold) ? 1 : 0;
    }

    return 0;
}

/**
 * @brief
 *
 * @param y_true Pointer to Vector with real/known values
 * @param y_pred Pointer to Vector with predicted classification values
 * @param TP Int pointer for True Positive
 * @param FP Int pointer for False Positive
 * @param TN Int pointer for True Negative
 * @param FN Int pointer for False Negative
 *
 * @return 0 if successful, -1 if failure
 */
int computeConfusionMatrix(Vector *y_true, Vector *y_pred, int *TP, int *FP, int *TN, int *FN)
{
    if (y_true == NULL || y_pred == NULL || y_true->data == NULL || y_pred->data == NULL || y_true->size != y_pred->size)
    {
        printf("Failed to compute confusion matrix.\n");
        return -1;
    }

    for (int i = 0; i < y_pred->size; ++i)
    {
        if ((int)y_true->data[i] == 0 && (int)y_pred->data[i] == 0)
        {
            ++*TN;
        }
        else if ((int)y_true->data[i] == 0 && (int)y_pred->data[i] == 1)
        {
            ++*FP;
        }
        else if ((int)y_true->data[i] == 1 && (int)y_pred->data[i] == 0)
        {
            ++*FN;
        }
        else if ((int)y_true->data[i] == 1 && (int)y_pred->data[i] == 1)
        {
            ++*TP;
        }
        else
        {
            printf("Problem with computing confusing matrix at an index.\n");
            printf(" y_true[%d] = %d", i, (int)y_true->data[i]);
            printf(" y_pred[%d] = %d", i, (int)y_pred->data[i]);
            return -1;
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
        printf("Denominator value is too close to 0; will result in error.\n");
        return -1;
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
    printf("denom = %.2lf\n", denom);
    printf("abs(0.0 - denom) = %.4lf\n", fabs(0.0 - denom));
    if (abs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; will result in error.\n");
        return -1;
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
    // printf("denom = %.2lf\n", denom);
    // printf("abs(0.0 - denom) = %.4lf\n", fabs(0.0 - denom));
    if (abs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; will result in error.\n");
        return -1;
    }

    *recall = (double)TP / denom;

    return 0;
}

/**
 * @brief Function to calculate the F1 score of a model
 *
 * @param precision Double with precision value
 * @param recall Double with recall value
 *
 * @return 0 if successful, -1 if failure
 */
int computeF1(double precision, double recall, double *f1)
{
    double denom = precision + recall;
    if (abs(0.0 - denom) < 0.00001)
    {
        printf("Denominator value is too close to 0; will result in error.\n");
        return -1;
    }

    *f1 = 2 * (precision * recall) / denom;

    return 0;
}

/**
 * @brief Function to calculate the Mean Squared Error of two Vectors
 *
 * @param y_true Pointer to Vector object with true y values for model
 * @param y_pred Pointer to Vector object with predicted y values for model
 * @param mse Pointer to double that will hold the mse value
 *
 * @return 0 if successful, -1 if failure
 */
int computeMSE(Vector *y_true, Vector *y_pred, double *mse)
{
    if (y_true == NULL || y_true->data == NULL || y_pred == NULL || y_pred->data == NULL)
    {
        printf("Problem with input paramters for computing MSE.\n");
        return -1;
    }

    double sum_square = 0.0;
    for (int i = 0; i < y_true->size; ++i)
    {
        sum_square += (y_true->data[i] - y_pred->data[i]) * (y_true->data[i] - y_pred->data[i]);
    }

    *mse = sum_square / (double)y_true->size;

    return 0;
}

/**
 * @brief Function to calculate the Root Mean Squared Error of two Vectors
 *
 * @param y_true Pointer to Vector object with true y values for model
 * @param y_pred Pointer to Vector object with predicted y values for model
 * @param rmse Pointer to double that will hold the rmse value
 *
 * @return 0 if successful, -1 if failure
 */
int computeRMSE(Vector *y_true, Vector *y_pred, double *rmse)
{
    if (y_true == NULL || y_true->data == NULL || y_pred == NULL || y_pred->data == NULL)
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
 * @brief Function to calculate the Mean Sbsolute Error of two Vectors
 *
 * @param y_true Pointer to Vector object with true y values for model
 * @param y_pred Pointer to Vector object with predicted y values for model
 * @param mae Pointer to double that will hold the mae value
 *
 * @return 0 if successful, -1 if failure
 */
int computeMAE(Vector *y_true, Vector *y_pred, double *mae)
{
    if (y_true == NULL || y_true->data == NULL || y_pred == NULL || y_pred->data == NULL)
    {
        printf("Problem with input paramters for computing MAE.\n");
        return -1;
    }

    double sum_absolute = 0.0;
    for (int i = 0; i < y_true->size; ++i)
    {
        sum_absolute += abs(y_true->data[i] - y_pred->data[i]);
    }

    *mae = sum_absolute / (double)y_true->size;

    return 0;
}

/**
 * @brief Function to calculate the R^2 Score (Coefficient of Determination)
 *
 * @param y_true Pointer to Vector object with true y values for model
 * @param y_pred Pointer to Vector object with predicted y values for model
 * @param r2score Pointer to double that will hold the r2score value
 *
 * @return 0 if successful, -1 if failure
 */
int computeR2Score(Vector *y_true, Vector *y_pred, double *r2score)
{
    if (y_true == NULL || y_true->data == NULL || y_pred == NULL || y_pred->data == NULL)
    {
        printf("Problem with input paramters for computing MAE.\n");
        return -1;
    }

    double y_true_avg = 0.0;
    for (int i = 0; i < y_true->size; ++i)
    {
        y_true_avg += y_true->data[i];
    }
    y_true_avg /= (double)y_true->size;

    double sum_numer = 0.0;
    double sum_denom = 0.0;
    for (int i = 0; i < y_true->size; ++i)
    {
        sum_numer += (y_true->data[i] - y_pred->data[i]) * (y_true->data[i] - y_pred->data[i]);
        sum_denom += (y_true->data[i] - y_true_avg) * (y_true->data[i] - y_true_avg);
    }

    *r2score = 1.0 - (sum_numer / sum_denom);
    return 0;
}

/**
 * @brief Function to calculate all the metrics given the models type. Initialize EvalMetrics object prior to calulating metrics.
 *
 * @param model Pointer to solved/trained Model object
 * @param eval_metrics Pointer to initialized, but empty, EvalMetrics object
 *
 * @return 0 if successful, -1 if failure
 */
int calculateAllMetrics(Model *model, EvalMetrics *eval_metrics)
{
    if (model == NULL || eval_metrics == NULL || model->y == NULL || model->y->data == NULL)
    {
        printf("Input model or eval metrics object is incorrect.\n");
        return -1;
    }

    if (model->type == LINEAR)
    {
        // Linear model gets MSE, RMSE, MAE, and R2 Score

        // MSE
        if (computeMSE(model->y, eval_metrics->y_lables, &eval_metrics->mse) < 0)
        {
            printf("Failure to compute MSE.\n");
            return -1;
        }

        // RMSE
        if (computeRMSE(model->y, eval_metrics->y_lables, &eval_metrics->rmse) < 0)
        {
            printf("Failure to compute RMSE.\n");
            return -1;
        }

        // MAE
        if (computeMAE(model->y, eval_metrics->y_lables, &eval_metrics->mae) < 0)
        {
            printf("Failure to compute MAE.\n");
            return -1;
        }

        // R2 Score
        if (computeR2Score(model->y, eval_metrics->y_lables, &eval_metrics->r2score) < 0)
        {
            printf("Failure to compute R2 Score.\n");
            return -1;
        }
    }
    else if (model->type == LOGISTIC)
    {
        // Logistic model gets confusion matrix, accuracy, precision, recall, and F1 score

        // Confusion Matrix
        if (computeConfusionMatrix(model->y, eval_metrics->y_lables, &eval_metrics->TP, &eval_metrics->FP, &eval_metrics->TN, &eval_metrics->FN) < 0)
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

    return 0;
}

/**
 * @brief Function to print all metrics for a model
 *
 * @param model Pointer to solved/trained Model object
 * @param eval_metrics Pointer to initialized, but empty, EvalMetrics object
 *
 * @return 0 if successful, -1 if failure
 */
int printMetrics(Model *model, EvalMetrics *eval_metrics)
{
    if (model == NULL || eval_metrics == NULL || model->y == NULL || model->y->data == NULL)
    {
        printf("Input model or eval metrics object is incorrect.\n");
        return -1;
    }

    if (model->type == LINEAR)
    {
        printf("\n");
        printf("     MSE = %.6lf\n", eval_metrics->mse);
        printf("     RMSE = %.6lf\n", eval_metrics->rmse);
        printf("     MAE = %.6lf\n", eval_metrics->mae);
        printf("     R2 Score = %.6lf\n", eval_metrics->r2score);
        printf("\n");
    }
    else if (model->type == LOGISTIC)
    {
        printf("\n");
        printf("     TP = %d\n", eval_metrics->TP);
        printf("     TN = %d\n", eval_metrics->TN);
        printf("     FP = %d\n", eval_metrics->FP);
        printf("     FN = %d\n", eval_metrics->FN);
        printf("     Accuracy = %.6lf\n", eval_metrics->accuracy);
        printf("     Precision = %.6lf\n", eval_metrics->precision);
        printf("     Recall = %.6lf\n", eval_metrics->recall);
        printf("     F1 Score = %.6lf\n", eval_metrics->f1);
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
