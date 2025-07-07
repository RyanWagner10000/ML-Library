/*
 * file: regression.c
 * description: script of all regression functions
 * author: Ryan Wagner
 * date: June 28, 2025
 * notes: script encompasses prediction, loss with regularization & lambda,
 *        gradient descent, and a linear/logistic model trainer
 */

#include "../header/regression.h"

/**
 * @brief Initialize a Model object by malloc-ing the Matrix and Vector members
 *
 * @param model Model object to init
 *
 * @return 0 if successful, -1 if failure
 */
int initModel(Model *model)
{
    model->X = malloc(sizeof(Matrix));
    model->y = malloc(sizeof(Vector));
    model->weights = malloc(sizeof(Vector));

    model->func = NULL;

    return 0;
}

/**
 * @brief Check to see if the model is setup correctly; if enough information is given
 *
 * @param model Model to check
 *
 * @return 0 if successful, -1 if failure
 */
int checkModel(Model *model)
{
    // Check if X has been set
    if (model->X->data == NULL)
    {
        printf("X Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if y has been set
    if (model->y->data == NULL)
    {
        printf("y Vector is NULL and unset.\n");
        return -1;
    }

    // Check if weights has been set
    if (model->weights->data == NULL)
    {
        printf("weights Vector is NULL and unset.\n");
        return -1;
    }

    // Check if input dimensions has been set
    // if (model->input_dim <= 0)
    // {
    //     printf("Model input dimensions is <= 0. Setting to number of columns in X matrix.\n");
    //     model->input_dim = model->X.cols;
    // }

    // Check if output dimensions has been set
    // if (model->output_dim <= 0)
    // {
    //     printf("Model output dimensions is <= 0. Setting to number of columns in X matrix.\n");
    //     model->output_dim = model->X.cols;
    // }

    // Check if regression type has been set
    if (model->type < 0 || model->type > 1)
    {
        printf("Regression type is not recognized or unset.\n");
        return -1;
    }

    // Check if model config has been set
    // Check if model config learning rate has been set, default to 0.01
    if (model->config.learning_rate <= 0)
    {
        printf("Learning rate is <= 0 or unset. Setting to default 0.01\n");
        model->config.learning_rate = 0.01;
    }

    // Check if model config epochs has been set, default to 1000
    if (model->config.epochs <= 0)
    {
        printf("Epochs is <= 0 or unset. Setting to default 1000\n");
        model->config.epochs = 1000;
    }

    // Check if model config lambda has been set, default to 0.01
    if (model->config.lambda <= 0)
    {
        printf("Lambda is <= 0 or unset. Setting to default 0.01\n");
        model->config.lambda = 0.01;
    }

    // Check if model config regularization has been set, default to none
    if (model->config.regularization < 0 || model->config.regularization > 2)
    {
        printf("Regularization is not recognized. Setting to default 0 or REG_NONE\n");
        model->config.regularization = REG_NONE;
    }

    return 0;
}

/**
 * @brief Prediction function for linear regression
 *
 * @param x Vector pointer for input values
 * @param w Vector pointer for weights for each predicted value in Vector x
 * @param b Bias applied to each prediction
 * @param func Activation function being applied if desired
 * @param result Result from the prediction operation
 *
 * @return 0 if successful, -1 if failure
 */
static int predict(Vector *x, Vector *w, double b, int (*func)(double *, double *), double *result)
{
    // Apply wights and biases to input values
    if (dot_product(x, w, result) < 0)
    {
        printf("Dot product operation was unsuccessful.\n");
        return -1;
    }
    *result += b;

    if (func == NULL)
    {
        return 0;
    }

    // Apply activation funciton to dot product and bias sum
    double temp_res = 0.0;
    if (func(result, &temp_res) < 0)
    {
        printf("Activation function was unsuccessful in prediction operation.\n");
        return -1;
    }
    *result = temp_res;

    return 0;
}

/**
 * @brief Compute the loss of the current epoch with MSE and L2 Regularization with lambda
 *
 * @param model Model object that holds all the Matrices and Vectors
 * @param loss Resulting Vector of losses
 *
 * @return 0 if successful, -1 if failure
 */
static int computeLoss(Model *model, double *loss)
{
    // Matrix *x, Vector *y, Vector *w, double b, double *result, double lambda, RegularizationType regularize
    double y_pred = 0;
    *loss = 0;

    // apply the prediction to each row in input matrix
    for (int i = 0; i < model->X->rows; ++i)
    {
        // printf("Predict in compute loss.\n");
        Vector xi = {model->X->cols, &(model->X->data)[i * model->X->cols]};
        if (predict(&xi, model->weights, model->bias, model->func, &y_pred) < 0)
        {
            printf("Prediction was unsuccessful in Compute Loss function.\n");
            return -1;
        }

        // Accumulate error based on prediction and target values
        double error = 0.0;
        if (model->type == LINEAR)
        {
            error = (model->y->data)[i] - y_pred;
            *loss += error * error;
        }
        else if (model->type == LOGISTIC)
        {
            error = y_pred - (model->y->data)[i];
            *loss += model->y->data[i] * log10(y_pred) + (1 - model->y->data[i]) * log10(1 - y_pred);
        }
        else
        {
            printf("Model type unrecognized when computing the loss.\n");
            return -1;
        }
    }

    // Average the result
    *loss /= model->X->rows;
    if (model->type == LOGISTIC)
    {
        *loss *= -1;
    }

    if (model->config.regularization == REG_L1)
    {
        // Apply L1 Norm regularization (sum of absolute values)
        double reg_term = 0.0;
        for (int i = 0; i < model->weights->size; ++i)
        {
            reg_term += fabs(model->weights->data[i]);
        }
        // Apply lambda on regularization
        *loss += model->config.lambda * reg_term;
    }
    else if (model->config.regularization == REG_L2)
    {
        // Apply L2 Norm regularization (sum of squares)
        double reg_term = 0.0;
        for (int i = 0; i < model->weights->size; ++i)
        {
            reg_term += model->weights->data[i] * model->weights->data[i];
        }
        // Apply lambda on regularization
        *loss += model->config.lambda * reg_term;
    }

    return 0;
}

/**
 * @brief Computes the gradient descent as it solves the problem
 *
 * @param model Model object that holds all the Matrices and Vectors
 * @param grad_w Vector pointer of the gradient of the weights
 * @param grad_b Vector pointer of the gradient of the bias(es)
 *
 * @return 0 if successful, -1 if failure
 */
static int computeGradients(Model *model, Vector *grad_w, double *grad_b)
{
    // Init gradient biases to 0
    *grad_b = 0.0;

    // Apply the prediction to each row in the input Matrix
    for (int i = 0; i < model->X->rows; ++i)
    {
        Vector xi;
        if (makeVector(&xi, model->X->cols, &model->X->data[i * model->X->cols], TYPE_DOUBLE) < 0)
        {
            printf("Unsucessful making of \'xi\' vector\n");
            return -1;
        }

        double y_pred = 0;
        if (predict(&xi, model->weights, model->bias, model->func, &y_pred) < 0)
        {
            printf("Prediction was unsuccessful when computing gradients with xi.\n");
            return -1;
        }

        // Calculate the weights and biases gradients
        double error = 0.0;
        if (model->type == LINEAR)
        {
            error = model->y->data[i] - y_pred;

            for (int j = 0; j < model->weights->size; ++j)
            {
                grad_w->data[j] += -2 * error * xi.data[j];
            }

            *grad_b += -2 * error;
        }
        else if (model->type == LOGISTIC)
        {
            error = y_pred - model->y->data[i];

            for (int j = 0; j < model->weights->size; ++j)
            {
                grad_w->data[j] += error * xi.data[j];
            }

            *grad_b += error;
        }
        else
        {
            printf("Model regression type unknown.\n");
            return -1;
        }

        freeVector(&xi);
    }

    // Average the gradients
    for (int j = 0; j < grad_w->size; ++j)
    {
        grad_w->data[j] /= model->X->rows;

        // Add regularization gradient
        if (model->config.regularization == REG_NONE)
        {
            continue;
        }
        else if (model->config.regularization == REG_L1)
        {
            grad_w->data[j] += model->config.lambda * ((model->weights->data[j] > 0) ? 1.0 : (model->weights->data[j] < 0) ? -1.0
                                                                                                                           : 0.0);
        }
        else if (model->config.regularization == REG_L2)
        {
            grad_w->data[j] += 2 * model->config.lambda * model->weights->data[j];
        }
    }

    *grad_b /= model->X->rows;

    return 0;
}

/**
 * @brief
 *
 * @param model Model object that holds the configuration, matrices, and vectors to run
 *
 * @return 0 if successful, -1 if failure
 */
int trainModel(Model *model)
{
    // Check that the model has been setup correctly before trying to train
    if (checkModel(model) < 0)
    {
        printf("The model object submitted to train has not be setup properly.\n");
        return -1;
    }

    // Init weights gradient Vector and bias
    Vector grad_w;
    if (makeVectorZeros(&grad_w, model->weights->size) < 0)
    {
        printf("Unsuccessful initialization of gradient weights vector in model training\n");
        return -1;
    }

    double grad_b = 0.0;

    // Set a division factor for printing the loss over epochs
    int division_factor = 10.0;
    if (model->config.epochs >= 1000)
    {
        division_factor = 100;
    }
    if (model->config.epochs >= 5000)
    {
        division_factor = 250;
    }
    if (model->config.epochs >= 10000)
    {
        division_factor = 500;
    }

    // Iterate through N-number of epochs adjusting the weights and bias
    for (int epoch = 0; epoch < model->config.epochs; ++epoch)
    {
        if (computeGradients(model, &grad_w, &grad_b) < 0)
        {
            printf("Compute Gradients was unsuccessful.\n");
            return -1;
        }

        // Adjust the weights and biases based on the learning rate and respective gradients
        for (int j = 0; j < model->weights->size; ++j)
        {
            model->weights->data[j] -= model->config.learning_rate * grad_w.data[j];
        }

        model->bias -= model->config.learning_rate * grad_b;

        double loss = 0.0;
        if (computeLoss(model, &loss) < 0)
        {
            printf("Compute Loss was unsuccessful.\n");
            return -1;
        }

        // Purely for user to see progress over time/epoch
        if (epoch % division_factor == 0 || epoch == model->config.epochs - 1)
        {
            printf("Epoch %d | Loss %.6f\n", epoch, loss);
        }
    }
    printf("\n");

    freeVector(&grad_w);
    return 0;
}

/**
 * @brief Free Model X, y, and weights and set poitner to NULL
 *
 * @param model Model to free
 *
 * @return None
 */
void freeModel(Model *model)
{
    if (model && model->X->data)
    {
        free(model->X->data);
        model->X->data = NULL;
    }

    if (model && model->y->data)
    {
        free(model->y->data);
        model->y->data = NULL;
    }

    if (model && model->weights->data)
    {
        free(model->weights->data);
        model->weights->data = NULL;
    }
}