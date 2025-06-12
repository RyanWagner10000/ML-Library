/*
 * file: linear_regression.c
 * description: script of all linear regression functions
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: script encompasses prediction, loss with MSE & regularization w/ lambda,
 *        gradient descent, and a linear model trainer
 */

#include "../header/linear_regression.h"

/**
 * @brief Prediction function for linear regression
 *
 * @param x Vector pointer for input values
 * @param w Vector pointer for weights for each predicted value in Vector x
 * @param b Bias applied to each prediction
 * @param result Result from the prediction operation
 *
 * @return 0 if successful, -1 if failure
 */
int predict(Vector *x, Vector *w, double b, double *result)
{
    // Apply wights and biases to input values
    return dot_product(x, w, result) + b;
}

/**
 * @brief Compute the loss of the current epoch with MSE and L2 Regularization with lambda
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
int computeLoss(Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, RegularizationType regularize)
{
    double y_pred = 0;
    *result = 0;

    // apply the prediction to each row in input matrix
    for (int i = 0; i < x->rows; ++i)
    {
        switch (x->type)
        {
        case TYPE_INT:
        {
            Vector xi = {x->cols, &((int *)x->data)[i * x->cols], TYPE_INT};
            if (predict(&xi, w, b, &y_pred) < 0)
            {
                return -1;
            }
            break;
        }
        case TYPE_FLOAT:
        {
            Vector xi = {x->cols, &((float *)x->data)[i * x->cols], TYPE_FLOAT};
            if (predict(&xi, w, b, &y_pred) < 0)
            {
                return -1;
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            Vector xi = {x->cols, &((double *)x->data)[i * x->cols], TYPE_DOUBLE};
            if (predict(&xi, w, b, &y_pred) < 0)
            {
                return -1;
            }
            break;
        }
        default:
            break;
        }

        // Accumulate error based on prediction and target values
        double error = ((double *)y->data)[i] - y_pred;
        *result += error * error;
    }

    // Average the result
    *result /= x->rows;

    if (regularize == REG_L1)
    {
        // Apply L1 Norm regularization (sum of absolute values)
        double reg_term = 0.0;
        for (int i = 0; i < w->size; ++i)
        {
            reg_term += fabs(((double *)w->data)[i]);
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
            reg_term += ((double *)w->data)[i] * ((double *)w->data)[i];
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
int computeGradients(Matrix *x, Vector *y_true, Vector *w, double b, Vector *grad_w, double *grad_b, double lambda, RegularizationType regularize)
{
    // Init gradient weights and biases to 0
    switch (grad_w->type)
    {
    case TYPE_INT:
    {
        for (int j = 0; j < w->size; ++j)
        {
            ((int *)grad_w->data)[j] = 0;
        }
        break;
    }
    case TYPE_FLOAT:
    {
        for (int j = 0; j < w->size; ++j)
        {
            ((float *)grad_w->data)[j] = 0.0;
        }
        break;
    }
    case TYPE_DOUBLE:
    {
        for (int j = 0; j < w->size; ++j)
        {
            ((double *)grad_w->data)[j] = 0.0;
        }
        break;
    }
    default:
    {
        printf("Unrecognized data type for gradient weights zero initialization\n");
        return -1;
    }
    }
    *grad_b = 0.0;

    // Apply the prediction to each row in the input Matrix
    for (int i = 0; i < x->rows; ++i)
    {
        Vector xi;
        switch (x->type)
        {
        case TYPE_INT:
        {
            if (makeVector(&xi, x->cols, &((int *)x->data)[i * x->cols], TYPE_INT) < 0)
            {
                printf("Unsucessful making of \'xi\' vector\n");
                return -1;
            }
            break;
        }
        case TYPE_FLOAT:
        {
            if (makeVector(&xi, x->cols, &((float *)x->data)[i * x->cols], TYPE_FLOAT) < 0)
            {
                printf("Unsucessful making of \'xi\' vector\n");
                return -1;
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            if (makeVector(&xi, x->cols, &((double *)x->data)[i * x->cols], TYPE_DOUBLE) < 0)
            {
                printf("Unsucessful making of \'xi\' vector\n");
                return -1;
            }
            break;
        }
        default:
            break;
        }

        double y_pred = 0;
        if (predict(&xi, w, b, &y_pred) < 0)
        {
            return -1;
        }
        double error = ((double *)y_true->data)[i] - y_pred;

        // Calculate the weights and biases gradients
        switch (grad_w->type)
        {
        case TYPE_INT:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((int *)grad_w->data)[j] += -2 * error * ((int *)xi.data)[j];
            }
            break;
        }
        case TYPE_FLOAT:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((float *)grad_w->data)[j] += -2 * error * ((float *)xi.data)[j];
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((double *)grad_w->data)[j] += -2 * error * ((double *)xi.data)[j];
            }
            break;
        }
        default:
        {
            printf("Unrecognized data type for gradient weights calculation\n");
            return -1;
        }
        }

        *grad_b += -2 * error;
    }

    // Average the gradients
    switch (grad_w->type)
    {
    case TYPE_INT:
    {
        for (int j = 0; j < grad_w->size; ++j)
        {
            ((int *)grad_w->data)[j] /= x->rows;

            // Add regularization gradient
            if (regularize == REG_L1)
            {
                ((int *)grad_w->data)[j] += lambda * ((((int *)w->data)[j] > 0) ? 1.0 : (((int *)w->data)[j] < 0) ? -1.0
                                                                                                                  : 0.0);
            }
            else if (regularize == REG_L2)
            {
                ((int *)grad_w->data)[j] += 2 * lambda * ((int *)w->data)[j];
            }
        }
        break;
    }
    case TYPE_FLOAT:
    {
        for (int j = 0; j < grad_w->size; ++j)
        {
            ((float *)grad_w->data)[j] /= x->rows;

            // Add regularization gradient
            if (regularize == REG_L1)
            {
                ((float *)grad_w->data)[j] += lambda * ((((float *)w->data)[j] > 0) ? 1.0 : (((float *)w->data)[j] < 0) ? -1.0
                                                                                                                        : 0.0);
            }
            else if (regularize == REG_L2)
            {
                ((float *)grad_w->data)[j] += 2 * lambda * ((float *)w->data)[j];
            }
        }
        break;
    }
    case TYPE_DOUBLE:
    {
        for (int j = 0; j < grad_w->size; ++j)
        {
            ((double *)grad_w->data)[j] /= x->rows;

            // Add regularization gradient
            if (regularize == REG_L1)
            {
                ((double *)grad_w->data)[j] += lambda * ((((double *)w->data)[j] > 0) ? 1.0 : (((double *)w->data)[j] < 0) ? -1.0
                                                                                                                           : 0.0);
            }
            else if (regularize == REG_L2)
            {
                ((double *)grad_w->data)[j] += 2 * lambda * ((double *)w->data)[j];
            }
        }
        break;
    }
    default:
    {
        printf("Unrecognized data type for gradient weights calculation\n");
        return -1;
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
int train_linear_model(Matrix *x, Vector *y_true, Vector *w, double *b, TrainConfig *config)
{
    // Init weights gradient Vector and bias
    Vector grad_w;
    if (makeVectorZeros(&grad_w, w->size, w->type) < 0)
    {
        printf("Unsuccessful initialization of gradient weights vector in model training\n");
        return -1;
    }
    double grad_b = 0.0;

    for (int epoch = 0; epoch < config->epochs; ++epoch)
    {
        if (computeGradients(x, y_true, w, *b, &grad_w, &grad_b, config->lambda, config->regularization) < 0)
        {
            return -1;
        }

        // Adjust the weights and biases based on the learning rate and respective gradients
        switch (w->type)
        {
        case TYPE_INT:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((int *)w->data)[j] -= config->learning_rate * ((int *)grad_w.data)[j];
            }
            break;
        }
        case TYPE_FLOAT:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((float *)w->data)[j] -= config->learning_rate * ((float *)grad_w.data)[j];
            }
            break;
        }
        case TYPE_DOUBLE:
        {
            for (int j = 0; j < w->size; ++j)
            {
                ((double *)w->data)[j] -= config->learning_rate * ((double *)grad_w.data)[j];
            }
            break;
        }
        default:
        {
            printf("Unrecognized data type for weights update\n");
            return -1;
        }
        }

        *b -= config->learning_rate * grad_b;

        // Purely for user to see progress over time/epoch
        if (epoch % 10 == 0 || epoch == config->epochs - 1)
        {
            double loss = 0.0;
            if (computeLoss(x, y_true, w, *b, &loss, config->lambda, config->regularization) < 0)
            {
                return -1;
            }
            printf("Epoch %d | Loss %.6f\n", epoch, loss);
        }
    }

    freeVector(&grad_w);
    return 0;
}