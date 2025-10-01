/*
 * file: regression.c
 * description: script of all regression functions
 * author: Ryan Wagner
 * date: June 28, 2025
 * notes: script encompasses prediction, loss with regularization & lambda,
 *        gradient descent, and a linear/logistic model trainer
 */

#include "../header/regression.h"
#include "../header/progressbar.h"

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
    model->y = malloc(sizeof(Matrix));
    model->weights = malloc(sizeof(Matrix));
    model->bias = malloc(sizeof(Vector));
    model->logits = malloc(sizeof(Matrix));

    model->splitdata = makeDefaultSplitData();

    model->func = ACT_NONE;
    model->batch_size = -1;

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
    if (model->splitdata.train_features.data == NULL)
    {
        printf("X Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if y has been set
    if (model->splitdata.train_labels.data == NULL)
    {
        printf("y Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if weights has been set
    if (model->weights->data == NULL)
    {
        printf("Weights Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if bias has been set
    if (model->bias->data == NULL)
    {
        printf("Bias Vector is NULL and unset.\n");
        return -1;
    }

    // // Check if logits has been set
    // if (model->logits->data == NULL)
    // {
    //     printf("Logits Matrix is NULL and unset.\n");
    //     return -1;
    // }

    // Check if batch size has been set
    if (model->batch_size < 1)
    {
        printf("Batch size was invalid or unset. Setting automatically based on input size.\n");
        int batch_size = 1;
        while (batch_size < (int)model->splitdata.train_features.rows / 2)
        {
            batch_size *= 2;
        }
        model->batch_size = batch_size / 2;
    }

    // Check if regression type has been set
    if (model->type < 0 || model->type > 2)
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

    // Check if model config epochs has been set, default to 1
    if (model->config.epochs <= 0)
    {
        printf("Epochs is <= 0 or unset. Setting to default 1\n");
        model->config.epochs = 1;
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
 * @brief Computes the one-hot encoded form of a Matrix
 *
 * @param m Original matrix values
 * @param m_encoded Matrix to get one-hot encoded values
 * @param classes Number of classes for encoding
 *
 * @return 0 if successful, -1 if failure
 */
int computeOneHotEncodedMatrix(Matrix m, Matrix *m_encoded, int classes)
{
    // Temp Matrix
    Matrix temp_y = {0};
    if (makeMatrixZeros(&temp_y, m.rows, classes) < 0)
    {
        printf("Could not initialize temp y matrix.\n");
        return -1;
    }

    // Create one-hot encoded row in the matrix
    for (int i = 0; i < temp_y.rows; ++i)
    {
        temp_y.data[i * temp_y.cols + (int)m.data[i]] = 1.0;
    }

    freeMatrix(m_encoded);

    // Reallocate memory for bigger y matrix in one-hot encoded form
    if (makeMatrixZeros(m_encoded, temp_y.rows, classes) < 0)
    {
        printf("Recreation of encoded matrix was unsuccessful.\n");
        return -1;
    }

    // Copy contents of one-hot encoded form to y matrix
    if (copyMatrix(temp_y, m_encoded) < 0)
    {
        printf("Copying of temp matrix in encoded matrix was unsuccessful.\n");
        return -1;
    }

    freeMatrix(&temp_y);
    return 0;
}

/**
 * @brief Computes the labels with the following formula: activation(weights * features + biases)
 *
 * @param X Input Matrix of features
 * @param weights Input Matrix of trained weights
 * @param biases Input Vector of trained biases
 * @param labels Output Matrix of predicted labels
 * @param activation Activation function to apply to predicted labels
 *
 * @return 0 if successful, -1 if failure
 */
int comptueLabels(Matrix X, Matrix weights, Vector biases, Matrix *labels, Activation activation)
{
    if (!X.data || !weights.data || !biases.data || !labels->data)
    {
        printf("Input variables int computeLabels were not sucessfully setup.\n");
        return -1;
    }

    if (labels->rows != X.rows)
    {
        labels->rows = X.rows;
    }
    if (labels->cols != weights.cols)
    {
        labels->cols = weights.cols;
    }

    freeMatrix(labels);
    labels->data = calloc(labels->rows * labels->cols, sizeof(double));

    if (mat_mul(X, weights, labels) < 0)
    {
        printf("Matrix multiplication in computeLabels was unsuccessful.\n");
        return -1;
    }
    printf("multiplied here\n");

    if (mat_add(*labels, biases, labels) < 0)
    {
        printf("Error with matrix addition.\n");
        return -1;
    }

    switch (activation)
    {
    case SIGMOID:
    {
        if (applyToMatrix(labels, SIGMOID) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case SIGMOID_DX:
    {
        if (applyToMatrix(labels, SIGMOID_DX) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case RELU:
    {
        if (applyToMatrix(labels, RELU) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case RELU_DX:
    {
        if (applyToMatrix(labels, RELU_DX) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case TANH:
    {
        if (applyToMatrix(labels, TANH) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case TANH_DX:
    {
        if (applyToMatrix(labels, TANH_DX) < 0)
        {
            printf("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }

    default:
        break;
    }

    return 0;
}

/**
 * @brief Computes logits and applies activation function based on regression type
 *
 * @param x_inputs Matrix of inputs
 * @param model Model object
 *
 * @return 0 if successful, -1 if failure
 */
static int computeLogits(Matrix x_inputs, Model *model)
{
    // Calculate X * weights
    if (mat_mul(x_inputs, *model->weights, model->logits) < 0)
    {
        printf("Matrix multiplication in logits computation was unsuccessful.\n");
        return -1;
    }

    // Add bias row-wise
    for (int r = 0; r < model->logits->rows; ++r)
    {
        for (int c = 0; c < model->logits->cols; ++c)
        {
            model->logits->data[r * model->logits->cols + c] += model->bias->data[c];
        }
    }

    switch (model->type)
    {
    case LOGISTIC_REGRESSION:
    {
        // Element-wise sigmoid
        if (applyToMatrix(model->logits, model->func) < 0)
        {
            printf("Applying function to each member in logits matrix was unsuccessful.\n");
            return -1;
        }

        break;
    }
    case SOFTMAX_REGRESSION:
    {
        // Make Vector to hold one row of matrix
        Vector temp_row = {0};
        if (makeVectorZeros(&temp_row, model->logits->cols) < 0)
        {
            printf("Making temp row variable was unsuccessful.\n");
            return -1;
        }

        // Row-wise softmax
        for (int r = 0; r < model->logits->rows; ++r)
        {
            if (getRowMatrix(*model->logits, r, &temp_row) < 0)
            {
                printf("Getting row %d from Matrix m for computing Logits was unsuccessful.\n", r);
                return -1;
            }

            for (int j = 0; j < model->logits->cols; ++j)
            {
                int index = r * model->logits->cols + j;
                if (softmax(model->logits->data[index], temp_row, &model->logits->data[index]) < 0)
                {
                    printf("Softmax function was unsuccessful when computing logits.\n");
                    freeVector(&temp_row);
                    return -1;
                }
            }
        }

        freeVector(&temp_row);

        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}

/**
 * @brief Compute the loss of the current epoch with MSE and L2 Regularization with lambda
 *
 * @param y_real Matrix object holding real values
 * @param model Model object
 * @param loss Resulting Vector of losses
 *
 * @return 0 if successful, -1 if failure
 */
static int computeLoss(Matrix y_real, Model *model, double *loss)
{
    double y_pred = 0;
    *loss = 0;

    // Accumulate error based on prediction and target values
    double error = 0.0;
    int index = 0;
    switch (model->type)
    {
    case LINEAR_REGRESSION:
    {
        // Performing MSE
        for (int r = 0; r < model->logits->rows; ++r)
        {
            for (int c = 0; c < model->logits->cols; ++c)
            {
                index = r * model->logits->cols + c;

                double y_pred = model->logits->data[index];
                double diff = y_real.data[index] - y_pred;

                error += diff * diff;
            }
        }
        *loss = error / model->logits->rows;
        break;
    }
    case LOGISTIC_REGRESSION:
    {
        // Performing BCE (Binary Cross Entropy)
        for (int r = 0; r < model->logits->rows; ++r)
        {
            for (int c = 0; c < model->logits->cols; ++c)
            {
                index = r * model->logits->cols + c;

                double y_pred = model->logits->data[index];

                error += y_real.data[index] * log(y_pred) + (1 - y_real.data[index]) * log(1 - y_pred);
            }
        }
        *loss = -1 * error / model->logits->rows;
        break;
    }
    case SOFTMAX_REGRESSION:
    {
        // Performing CCE (Categorical Cross Entropy)
        for (int r = 0; r < model->logits->rows; ++r)
        {
            // reset error to zero so loss sums row-wise
            // error = 0.0;
            for (int c = 0; c < model->logits->cols; ++c)
            {
                index = r * model->logits->cols + c;

                double y_pred = model->logits->data[index];

                error += y_real.data[index] * log(y_pred);
            }
        }
        *loss = -1 * error / model->logits->rows;
        break;
    }
    default:
    {
        printf("Model type unrecognized when computing the loss.\n");
        return -1;
    }
    }

    // Apply regularization if defined by user
    if (model->config.regularization == REG_NONE)
    {
        return 0;
    }

    double reg_penalty = 0.0;
    double sum = 0.0;
    for (int r = 0; r < model->weights->rows; ++r)
    {
        for (int c = 0; c < model->weights->cols; ++c)
        {
            if (model->config.regularization == REG_L1)
            {
                sum += fabs(model->weights->data[r * model->weights->cols + c]);
            }
            else if (model->config.regularization == REG_L2)
            {
                double value = model->weights->data[r * model->weights->cols + c];
                sum += value * value;
            }
        }
    }
    reg_penalty = model->config.lambda * sum;

    *loss += reg_penalty;

    return 0;
}

/**
 * @brief Computes the gradient descent as it solves the problem
 *
 * @param x_inputs Matrix of inputs
 * @param y_real Matrix object holding real values
 * @param model Model object
 * @param grad_w Vector pointer of the gradient of the weights
 * @param grad_b Vector pointer of the gradient of the bias(es)
 *
 * @return 0 if successful, -1 if failure
 */
static int computeGradients(Matrix x_inputs, Matrix y_real, Model *model, Matrix *grad_w, Vector *grad_b)
{
    Matrix dZ = {0};
    if (makeMatrixZeros(&dZ, x_inputs.rows, model->classes) < 0)
    {
        printf("Creation of delta Z matrix from computation of gradients was unsuccessful.\n");
        return -1;
    }

    if (model->type == LINEAR_REGRESSION)
    {
        // Calculate dZ matrix
        if (mat_sub(y_real, *model->logits, &dZ) < 0)
        {
            printf("Matrix subtraction was unsuccessful.");
        }

        // Calculate gradient of bias
        double sum = 0.0;
        for (int i = 0; i < dZ.rows; ++i)
        {
            grad_b->data[0] += dZ.data[i];
        }
        grad_b->data[0] *= (-2.0 / dZ.rows);

        // Init transpose of X matrix
        Matrix X_T = {0};
        if (makeMatrixZeros(&X_T, x_inputs.cols, x_inputs.rows) < 0)
        {
            printf("Initialization of transpose matrix unsuccessful.\n");
            freeMatrix(&X_T);
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            printf("Transpose operation was unsuccessful in compute gradients.\n");
            freeMatrix(&X_T);
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            printf("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
            freeMatrix(&X_T);
            return -1;
        }

        for (int i = 0; i < grad_w->rows; ++i)
        {
            grad_w->data[i] *= (-2.0 / dZ.rows);
        }

        freeMatrix(&X_T);
    }
    else if (model->type == LOGISTIC_REGRESSION)
    {
        // Calculate dZ matrix
        if (mat_sub(*model->logits, y_real, &dZ) < 0)
        {
            printf("Matrix subtraction was undsuccessful.");
        }

        // Calculate gradient of bias
        double sum = 0.0;
        for (int i = 0; i < dZ.rows; ++i)
        {
            grad_b->data[0] += dZ.data[i];
        }
        grad_b->data[0] *= (1.0 / dZ.rows);

        // Init transpose of X matrix
        Matrix X_T = {0};
        if (makeMatrixZeros(&X_T, x_inputs.cols, x_inputs.rows) < 0)
        {
            printf("Initialization of transpose matrix unsuccessful.\n");
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            printf("Transpose operation was unsuccessful in compute gradients.\n");
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            printf("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
            return -1;
        }

        for (int i = 0; i < grad_w->rows; ++i)
        {
            grad_w->data[i] *= (1.0 / dZ.rows);
        }

        freeMatrix(&X_T);
    }
    else if (model->type == SOFTMAX_REGRESSION)
    {
        // Calculate dZ matrix
        if (mat_sub(*model->logits, y_real, &dZ) < 0)
        {
            printf("Matrix subtraction was undsuccessful.");
        }

        // Init transpose of X matrix
        Matrix X_T = {0};
        if (makeMatrixZeros(&X_T, x_inputs.cols, x_inputs.rows) < 0)
        {
            printf("Initialization of transpose matrix unsuccessful.\n");
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            printf("Transpose operation was unsuccessful in compute gradients.\n");
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            printf("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
            return -1;
        }

        for (int r = 0; r < grad_w->rows; ++r)
        {
            for (int c = 0; c < grad_w->cols; ++c)
            {
                int index = r * grad_w->cols + c;
                grad_w->data[index] *= (1.0 / (double)dZ.rows);
            }
        }

        // Compute gradient of bias
        for (int r = 0; r < dZ.rows; ++r)
        {
            for (int c = 0; c < dZ.cols; ++c)
            {
                int index = r * dZ.cols + c;
                grad_b->data[c] += dZ.data[index];
            }
        }
        for (int i = 0; i < grad_b->size; ++i)
        {
            grad_b->data[i] *= (1.0 / (double)dZ.rows);
        }

        freeMatrix(&X_T);
    }

    freeMatrix(&dZ);
    return 0;
}

/**
 * @brief Computes regularization based on regularization type
 *
 * @param model Model object
 * @param grad_w Pointer to gradient of weights matrix
 *
 * @return 0 if successful, -1 if failure
 */
static int computeRegularization(Model model, Matrix *grad_w)
{
    for (int r = 0; r < grad_w->rows; ++r)
    {
        for (int c = 0; c < grad_w->cols; ++c)
        {
            int idx = r * grad_w->cols + c;
            // Add regularization gradient
            if (model.config.regularization == REG_NONE)
            {
                return 0;
            }
            else if (model.config.regularization == REG_L1)
            {
                grad_w->data[idx] += model.config.lambda * ((model.weights->data[idx] > 0) ? 1.0 : (model.weights->data[idx] < 0) ? -1.0
                                                                                                                                  : 0.0);
            }
            else if (model.config.regularization == REG_L2)
            {
                grad_w->data[idx] += 2 * model.config.lambda * model.weights->data[idx];
            }
        }
    }
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
    // Init weights matrix and bias vector
    if (makeMatrixZeros(model->weights, model->splitdata.train_features.cols, model->classes) < 0)
    {
        printf("Problem initializing weight Matrix\n");
        return -1;
    }
    if (makeVectorZeros(model->bias, model->classes) < 0)
    {
        printf("Problem initializing bias Matrix\n");
        return -1;
    }

    // Check that the model has been setup correctly before trying to train
    if (checkModel(model) < 0)
    {
        printf("The model object submitted to train has not be setup properly.\n");
        return -1;
    }

    // Convert y matrix to one-hot encoded form if performing softmax regression
    if (model->type == SOFTMAX_REGRESSION)
    {
        computeOneHotEncodedMatrix(*model->y, model->y, model->classes);
    }

    // Init weights gradient Vector and bias
    Matrix grad_w = {0};
    if (makeMatrixZeros(&grad_w, model->weights->rows, model->weights->cols) < 0)
    {
        printf("Unsuccessful initialization of gradient weights Matrix in model training\n");
        return -1;
    }
    Vector grad_b = {0};
    if (makeVectorZeros(&grad_b, model->bias->size) < 0)
    {
        printf("Unsuccessful initialization of gradient bias Vector in model training\n");
        return -1;
    }

    // Init reused variables, build batch sizing
    double loss = 0;
    int *perm_arr = (int *)calloc(model->splitdata.train_features.rows, sizeof(int));
    int batches = (int)ceil(model->splitdata.train_features.rows / (double)model->batch_size);
    int mini_batch_idx = 0;
    int batch_size = 0;
    PBD progress_bar;
    initProgressBar(&progress_bar, 75, '[', ']', '#', '.');
    drawProgressBar(&progress_bar);

    // Iterate through N-number of epochs adjusting the weights and bias
    for (int epoch = 1; epoch <= model->config.epochs; ++epoch)
    {
        // --- SHUFFLE DATASET ---

        // Create a random permutation of the number of samples in the dataset
        if (generateRandomPermutation(perm_arr, model->splitdata.train_features.rows) < 0)
        {
            printf("Creating random permutation for input shuffling was unsuccessful.\n");
            return -1;
        }

        // Iterate through forward and backward pass for each mini-batch matrix
        for (int b = 0; b < batches; ++b)
        {
            if (model->splitdata.train_features.rows - mini_batch_idx < model->batch_size)
            {
                batch_size = model->splitdata.train_features.rows - mini_batch_idx;
            }
            else
            {
                batch_size = model->batch_size;
            }

            // Make Logits matrix
            if (makeMatrixZeros(model->logits, batch_size, model->classes) < 0)
            {
                printf("Problem initializing logits Matrix\n");
                return -1;
            }

            // Get mini-batch of X
            Matrix mini_X = {0};
            if (makeMatrixZeros(&mini_X, batch_size, model->splitdata.train_features.cols) < 0)
            {
                printf("Creation of empty mini-batch X matrix was unsuccessful.\n");
                return -1;
            }
            if (makeMiniMatrix(*model->X, &mini_X, perm_arr, b, batch_size) < 0)
            {
                printf("Creation of mini-batch X matrix was unsuccessful.\n");
                return -1;
            }

            // Get mini-batch of y
            Matrix mini_y = {0};
            if (makeMatrixZeros(&mini_y, batch_size, model->splitdata.train_labels.cols) < 0)
            {
                printf("Creation of empty mini-batch y matrix was unsuccessful.\n");
                return -1;
            }
            if (makeMiniMatrix(*model->y, &mini_y, perm_arr, b, batch_size) < 0)
            {
                printf("Creation of mini-batch X matrix was unsuccessful.\n");
                return -1;
            }

            loss = 0;
            // --- FORWARD PASS ---

            // Compute logits and apply activation function
            if (computeLogits(mini_X, model) < 0)
            {
                printf("Computation of logits was unsuccessful while training model.\n");
                return -1;
            }

            // Compute loss
            if (computeLoss(mini_y, model, &loss) < 0)
            {
                printf("Computation of Loss was unsuccessful while training model.\n");
                return -1;
            }

            // --- BACKWARD PASS (GRADIENTS) ---

            if (clearMatrix(&grad_w) < 0)
            {
                printf("Clearing gradient weights matrix was unsuccessful.\n");
                return -1;
            }
            if (clearVector(&grad_b) < 0)
            {
                printf("Clearing gradient bias vector was unsuccessful.\n");
                return -1;
            }

            // Compute gradients
            if (computeGradients(mini_X, mini_y, model, &grad_w, &grad_b) < 0)
            {
                printf("Computation of Gradient was unsuccessful while training model.\n");
                return -1;
            }

            // Optional regularization
            if (computeRegularization(*model, &grad_w) < 0)
            {
                printf("Computation of Regularization was unsuccessful while training model.\n");
                return -1;
            }

            // Gradient descent update
            if (mat_mul(grad_w, model->config.learning_rate, &grad_w) < 0)
            {
                printf("Weights gradient descent update with learning rate was not successful.\n");
                return -1;
            }

            if (mat_sub(*model->weights, grad_w, model->weights) < 0)
            {
                printf("Weights update with gradient weights was not successful.\n");
                return -1;
            }

            if (vect_mul(grad_b, model->config.learning_rate, &grad_b) < 0)
            {
                printf("Bias gradient descent update with learning rate was not successful.\n");
                return -1;
            }

            if (vect_sub(*model->bias, grad_b, model->bias) < 0)
            {
                printf("Bias update with gradient bias was not successful.\n");
                return -1;
            }

            mini_batch_idx += batch_size;
            freeMatrix(&mini_X);
            freeMatrix(&mini_y);
            freeMatrix(model->logits);
        }
        mini_batch_idx = 0;

        // Progress over time/epoch
        progress_bar.nCurLen = (epoch * progress_bar.nMaxLen) / model->config.epochs;
        progress_bar.Loss = loss;
        progress_bar.Progress = (int)(((double)epoch / (double)model->config.epochs) * 100.0);
        drawProgressBar(&progress_bar);
    }
    printf("\n");

    freeMatrix(&grad_w);
    freeVector(&grad_b);
    free(perm_arr);
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
    // Free SplitData object
    if (model && model->splitdata.test_features.data && model->splitdata.test_labels.data && model->splitdata.train_features.data && model->splitdata.train_labels.data && model->splitdata.valid_features.data && model->splitdata.valid_labels.data)
    {
        freeSplitData(&model->splitdata);
    }

    // Free X input matrix
    if (model && model->X->data)
    {
        free(model->X->data);
        model->X->data = NULL;
    }

    // Free y output matrix
    if (model && model->y->data)
    {
        free(model->y->data);
        model->y->data = NULL;
    }

    // Free weights matrix
    if (model && model->weights->data)
    {
        free(model->weights->data);
        model->weights->data = NULL;
    }

    // Free biases vector
    if (model && model->bias->data)
    {
        free(model->bias->data);
        model->bias->data = NULL;
    }

    // Free logits matrix
    if (model && model->logits->data)
    {
        free(model->logits->data);
        model->logits->data = NULL;
    }
}