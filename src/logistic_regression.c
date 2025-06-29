/*
 * file: logistic_regression.c
 * description: script of all logistic regression functions
 * author: Ryan Wagner
 * date: June 23, 2025
 * notes: script encompasses prediction, loss with Cross Entropy,
 *        gradient descent, and a model trainer
 */

#include "../header/logistic_regression.h"

/**
 * @brief Prediction function for logistic regression
 *
 * @param x Vector pointer for input values
 * @param w Vector pointer for weights for each predicted value in Vector x
 * @param b Bias applied to each prediction
 * @param result Result from the prediction operation
 *
 * @return 0 if successful, -1 if failure
 */
static int predict(Vector *x, Vector *w, double b, double *result)
{
    // Apply wights and biases to input values
    if (dot_product(x, w, result) < 0)
    {
        printf("Dot product operation was unsuccessful.\n");
        return -1;
    }

    // Add bias
    *result += b;

    // Apply sigmoid to dot product and bias sum
    double temp_res = 0.0;
    if (sigmoid(result, &temp_res) < 0)
    {
        printf("Sigmoid was unsuccessful in prediction operation.\n");
        return -1;
    }
    *result = temp_res;

    return 0;
}

/**
 * @brief Compute the loss of the current epoch with Cross Entropy
 *
 * @param x Vector pointer for input values
 * @param y Vector pointer of target values
 * @param w Vector pointer for weights of features per sample
 * @param b Bias value applied in prediction step
 * @param result Resulting Vector of predicted valeus
 * @param lambda Strength of the regularization
 * @param regularize RegularizationType option for which type of norm to use
 *
 * @return 0 if successful, -1 if failure
 */
static int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, RegularizationType regularize)
{
    double y_pred = 0;
    *result = 0;

    // apply the prediction to each row in input matrix
    for (int i = 0; i < x->rows; ++i)
    {
        // printf("Predict in compute loss.\n");
        Vector xi = {x->cols, &(x->data)[i * x->cols]};
        if (predict(&xi, w, b, &y_pred) < 0)
        {
            printf("Prediction was unsuccessful in Compute Loss function.\n");
            return -1;
        }

        // Accumulate error based on prediction and target values
        // double error = (y->data)[i] - y_pred;
        double error = y_pred - (y->data)[i];
        *result += y->data[i] * log10(y_pred) + (1 - y->data[i]) * log10(1 - y_pred);
    }

    // Average the result
    *result /= x->rows;
    *result *= -1;

    if (regularize == REG_L1)
    {
        // Apply L1 Norm regularization (sum of absolute values)
        double reg_term = 0.0;
        for (int i = 0; i < w->size; ++i)
        {
            reg_term += fabs(w->data[i]);
        }
        // Apply lambda on regularization
        *result += lambda * reg_term;
    }
    else if (regularize == REG_L2)
    {
        // Apply L2 Norm regularization (sum of squares)
        double reg_term = 0.0;
        for (int i = 0; i < w->size; ++i)
        {
            reg_term += w->data[i] * w->data[i];
        }
        // Apply lambda on regularization
        *result += lambda * reg_term;
    }

    return 0;
}

/**
 * @brief Computes the gradient descent as it solves the problem
 *
 * @param x Vector pointer of input Values
 * @param y_true Vector pointer of target values
 * @param w Vector pointer of weights per feature
 * @param b Bias value applied to each predicted value
 * @param grad_w Vector pointer of the gradient of the weights
 * @param grad_b Vector pointer of the gradient of the bias(es)
 * @param lambda Lambda value used for regularization
 * @param regularize RegularizationType option for which type of norm to use
 *
 * @return 0 if successful, -1 if failure
 */
static int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, RegularizationType regularize)
{
    // Init gradient biases to 0
    *grad_b = 0.0;

    // Apply the prediction to each row in the input Matrix
    for (int i = 0; i < x->rows; ++i)
    {
        Vector xi;
        if (makeVector(&xi, x->cols, &x->data[i * x->cols], TYPE_DOUBLE) < 0)
        {
            printf("Unsucessful making of \'xi\' vector\n");
            return -1;
        }

        double y_pred = 0;
        if (predict(&xi, w, b, &y_pred) < 0)
        {
            printf("Prediction was unsuccessful when computing gradients with xi.\n");
            return -1;
        }
        // double error = y_true->data[i] - y_pred;
        double error = y_pred - y_true->data[i];

        // Calculate the weights and biases gradients
        for (int j = 0; j < w->size; ++j)
        {
            grad_w->data[j] += error * xi.data[j];
        }

        *grad_b += error;
        freeVector(&xi);
    }

    // Average the gradients
    for (int j = 0; j < grad_w->size; ++j)
    {
        grad_w->data[j] /= x->rows;

        // Add regularization gradient
        if (regularize == REG_NONE)
        {
            continue;
        }
        else if (regularize == REG_L1)
        {
            grad_w->data[j] += lambda * ((w->data[j] > 0) ? 1.0 : (w->data[j] < 0) ? -1.0
                                                                                   : 0.0);
        }
        else if (regularize == REG_L2)
        {
            grad_w->data[j] += 2 * lambda * w->data[j];
        }
    }

    *grad_b /= x->rows;

    return 0;
}

/**
 * @brief
 *
 * @param x Vector pointer of input Values
 * @param y_true Vector pointer of target values
 * @param w Vector pointer of weights per feature
 * @param config TrainConfig pointer for all training options from user
 *
 * @return 0 if successful, -1 if failure
 */
int train_logistic_model(Matrix *x, Vector *y_true, Vector *w, double *b, TrainConfig *config)
{
    // Init weights gradient Vector and bias
    Vector grad_w;
    if (makeVectorZeros(&grad_w, w->size) < 0)
    {
        printf("Unsuccessful initialization of gradient weights vector in model training\n");
        return -1;
    }
    double grad_b = 0.0;

    // Set a division factor for printing the loss over epochs
    int division_factor = 10.0;
    if (config->epochs >= 1000)
    {
        division_factor = 100;
    }
    if (config->epochs >= 5000)
    {
        division_factor = 250;
    }
    if (config->epochs >= 10000)
    {
        division_factor = 500;
    }

    for (int epoch = 0; epoch < config->epochs; ++epoch)
    {
        if (computeGradients(x, y_true, w, *b, &grad_w, &grad_b, config->lambda, config->regularization) < 0)
        {
            printf("Compute Gradients was unsuccessful.\n");
            return -1;
        }

        // Adjust the weights and biases based on the learning rate and respective gradients
        for (int j = 0; j < w->size; ++j)
        {
            w->data[j] -= config->learning_rate * grad_w.data[j];
        }

        *b -= config->learning_rate * grad_b;

        double loss = 0.0;
        if (computeLoss(x, y_true, w, *b, &loss, config->lambda, config->regularization) < 0)
        {
            printf("Compute Loss was unsuccessful.\n");
            return -1;
        }

        // Purely for user to see progress over time/epoch
        if (epoch % division_factor == 0 || epoch == config->epochs - 1)
        {
            printf("Epoch %d | Loss %.6f\n", epoch, loss);
        }
    }

    freeVector(&grad_w);
    return 0;
}