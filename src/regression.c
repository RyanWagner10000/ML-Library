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

    model->config = makeDefaultConfig();

    model->func = ACT_NONE;
    model->batch_size = -1;
    model->classes = 1;

    return 0;
}

/**
 * @brief Make a default model configuration
 *
 * @return ModelConfig object
 */
ModelConfig makeDefaultConfig()
{
    ModelConfig config;
    config.epochs = 1000;
    config.lambda = 0;
    config.regularization = REG_NONE;

    LearningRate lr;
    config.learning_rate.decay_constant = 0;
    config.learning_rate.decay_step = 1;
    config.learning_rate.decay_type = EXPONENTIAL_DECAY;
    config.learning_rate.init_learning_rate = 0.01;
    config.learning_rate.min_learning_rate = 0.01;
    return config;
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
        LOG_ERROR("X Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if y has been set
    if (model->splitdata.train_labels.data == NULL)
    {
        LOG_ERROR("y Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if weights has been set
    if (model->weights->data == NULL)
    {
        LOG_ERROR("Weights Matrix is NULL and unset.\n");
        return -1;
    }

    // Check if bias has been set
    if (model->bias->data == NULL)
    {
        LOG_ERROR("Bias Vector is NULL and unset.\n");
        return -1;
    }

    // Check if batch size has been set
    if (model->batch_size < 1)
    {
        LOG_WARN("Batch size was invalid or unset. Setting automatically based on input size.\n");
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
        LOG_ERROR("Regression type is not recognized or unset.\n");
        return -1;
    }

    // Check if model config learning rate has been set, default to 0.01
    if (model->config.learning_rate.init_learning_rate <= 0)
    {
        LOG_WARN("Learning rate is <= 0 or unset. Setting to default 0.01.\n");
        model->config.learning_rate.init_learning_rate = 0.01;
    }

    // Check if model config learning rate has been set, default to 0.01
    if (model->config.learning_rate.decay_type < 0)
    {
        LOG_WARN("Decay type is < 0 or unset. Setting to default Exponential Decay.\n");
        model->config.learning_rate.decay_type = EXPONENTIAL_DECAY;
        model->config.learning_rate.init_learning_rate = 0.01;
        model->config.learning_rate.curr_learning_rate = 0.01;
    }

    // Check if model config epochs has been set, default to 1
    if (model->config.epochs <= 0)
    {
        LOG_WARN("Epochs is <= 0 or unset. Setting to default 1\n");
        model->config.epochs = 1;
    }
    else
    {
        // Set array size of loss metrics now that epochs value is good
        model->metrics.loss_vs_epochs = calloc(model->config.epochs, sizeof(double));
    }

    // Check if model config lambda has been set, default to 0.01
    if (model->config.lambda <= 0)
    {
        LOG_WARN("Lambda is <= 0 or unset. Setting to default 0.01\n");
        model->config.lambda = 0.01;
    }

    // Check if model config regularization has been set, default to none
    if (model->config.regularization < 0 || model->config.regularization > 2)
    {
        LOG_WARN("Regularization is not recognized. Setting to default 0 or REG_NONE\n");
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
        LOG_ERROR("Could not initialize temp y matrix.\n");
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
        LOG_ERROR("Recreation of encoded matrix was unsuccessful.\n");
        return -1;
    }

    // Copy contents of one-hot encoded form to y matrix
    if (copyMatrix(temp_y, m_encoded) < 0)
    {
        LOG_ERROR("Copying of temp matrix in encoded matrix was unsuccessful.\n");
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
        LOG_ERROR("Input variables int computeLabels were not sucessfully setup.\n");
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
        LOG_ERROR("Matrix multiplication in computeLabels was unsuccessful.\n");
        return -1;
    }

    if (mat_add(*labels, biases, labels) < 0)
    {
        LOG_ERROR("Error with matrix addition.\n");
        return -1;
    }

    switch (activation)
    {
    case SIGMOID:
    {
        if (applyToMatrix(labels, SIGMOID) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case SIGMOID_DX:
    {
        if (applyToMatrix(labels, SIGMOID_DX) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case RELU:
    {
        if (applyToMatrix(labels, RELU) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case RELU_DX:
    {
        if (applyToMatrix(labels, RELU_DX) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case TANH:
    {
        if (applyToMatrix(labels, TANH) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
            return -1;
        }
        break;
    }
    case TANH_DX:
    {
        if (applyToMatrix(labels, TANH_DX) < 0)
        {
            LOG_ERROR("Applying activation function was unsuccessful.\n");
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
        LOG_ERROR("Matrix multiplication in logits computation was unsuccessful.\n");
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
            LOG_ERROR("Applying function to each member in logits matrix was unsuccessful.\n");
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
            LOG_ERROR("Making temp row variable was unsuccessful.\n");
            return -1;
        }

        // Row-wise softmax
        for (int r = 0; r < model->logits->rows; ++r)
        {
            if (getRowMatrix(*model->logits, r, &temp_row) < 0)
            {
                LOG_ERROR("Getting row %d from Matrix m for computing Logits was unsuccessful.\n", r);
                return -1;
            }

            for (int j = 0; j < model->logits->cols; ++j)
            {
                int index = r * model->logits->cols + j;
                if (softmax(model->logits->data[index], temp_row, &model->logits->data[index]) < 0)
                {
                    LOG_ERROR("Softmax function was unsuccessful when computing logits.\n");
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
        LOG_ERROR("Model type unrecognized when computing the loss.\n");
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
        LOG_ERROR("Creation of delta Z matrix from computation of gradients was unsuccessful.\n");
        return -1;
    }

    if (model->type == LINEAR_REGRESSION)
    {
        // Calculate dZ matrix
        if (mat_sub(y_real, *model->logits, &dZ) < 0)
        {
            LOG_ERROR("Matrix subtraction was unsuccessful.");
            return -1;
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
            LOG_ERROR("Initialization of transpose matrix unsuccessful.\n");
            freeMatrix(&X_T);
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            LOG_ERROR("Transpose operation was unsuccessful in compute gradients.\n");
            freeMatrix(&X_T);
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            LOG_ERROR("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
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
            LOG_ERROR("Matrix subtraction was unsuccessful.");
            return -1;
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
            LOG_ERROR("Initialization of transpose matrix unsuccessful.\n");
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            LOG_ERROR("Transpose operation was unsuccessful in compute gradients.\n");
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            LOG_ERROR("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
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
            LOG_ERROR("Matrix subtraction was unsuccessful.");
            return -1;
        }

        // Init transpose of X matrix
        Matrix X_T = {0};
        if (makeMatrixZeros(&X_T, x_inputs.cols, x_inputs.rows) < 0)
        {
            LOG_ERROR("Initialization of transpose matrix unsuccessful.\n");
            return -1;
        }

        // Calculate transpose of X matrix
        if (transpose(x_inputs, &X_T) < 0)
        {
            LOG_ERROR("Transpose operation was unsuccessful in compute gradients.\n");
            return -1;
        }

        // Matrix multiply to get gradient of weights
        if (mat_mul(X_T, dZ, grad_w) < 0)
        {
            LOG_ERROR("X^T and dZ matrix multiplication was unsuccessful in compute gradients.\n");
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
 * @brief Computes velocity matrix (velocity) for weights Matrix
 *
 * @param v_t Matrix object of velocity values
 * @param beta Double that is the velocity constant
 * @param grad_w Matrix object of model Weights
 *
 * @return 0 if successful, -1 if failure
 */
int computeVelocityWeights(Matrix *v_t, double beta, Matrix grad_w)
{
    // Check current velocity matrix
    if (!v_t || !v_t->data || v_t->rows <= 0 || v_t->cols <= 0)
    {
        LOG_ERROR("Input current Momentum matrix was invalid. Momentum calculation was unsuccessful.\n");
        return -1;
    }
    
    // Check weight matrix
    if (!grad_w.data || grad_w.rows <= 0 || grad_w.cols <= 0)
    {
        LOG_ERROR("Input Weights matrix was invalid. Momentum calculation was unsuccessful.\n");
        return -1;
    }

    // Check beta is greater than zero
    if (beta <= 0)
    {
        LOG_WARN("Input Momentum Beta constant was <= 0. Using value close to zero instead.\n");
        beta = 0.0000001;
    }

    Matrix temp_left = {0};
    if (makeMatrixZeros(&temp_left, grad_w.rows, grad_w.cols) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of temp Matrix in Momentum computation.\n");
        return -1;
    }

    if (mat_mul(*v_t, beta, &temp_left) < 0)
    {
        LOG_ERROR("Matrix multiplication of beta * m_t-1 in Momentum calculation was unsuccessful.\n");
        freeMatrix(&temp_left);
        return -1;
    }

    if (mat_add(temp_left, grad_w, v_t) < 0)
    {
        LOG_ERROR("Matrix addition of part [beta * m_t-1] and [(1 - beta) * weights] in Momentum calculation was unsuccessful.\n");
        freeMatrix(&temp_left);
        return -1;
    }

    return 0;
}

/**
 * @brief Computes velocity matrix (velocity) for bias Vector
 *
 * @param mt Vector object of velocity values
 * @param beta Double that is the velocity constant
 * @param grad_b Vector object of model bias(es)
 *
 * @return 0 if successful, -1 if failure
 */
int computeVelocityBias(Vector *mt, double beta, Vector grad_b)
{
    // Check current velocity matrix
    if (!mt || !mt->data || mt->size <= 0)
    {
        LOG_ERROR("Input current Momentum matrix was invalid. Momentum calculation was unsuccessful.\n");
        return -1;
    }
    
    // Check weight matrix
    if (!grad_b.data || grad_b.size <= 0)
    {
        LOG_ERROR("Input Weights matrix was invalid. Momentum calculation was unsuccessful.\n");
        return -1;
    }

    // Check beta is greater than zero
    if (beta <= 0)
    {
        LOG_WARN("Input Momentum Beta constant was <= 0. Using value close to zero instead.\n");
        beta = 0.0000001;
    }

    Vector temp_left = {0};
    if (makeVectorZeros(&temp_left, grad_b.size) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of temp Vector in Momentum computation.\n");
        return -1;
    }

    if (vect_mul(*mt, beta, &temp_left) < 0)
    {
        LOG_ERROR("Vector multiplication of beta * m_t-1 in Momentum calculation was unsuccessful.\n");
        freeVector(&temp_left);
        return -1;
    }

    if (vect_add(temp_left, grad_b, mt) < 0)
    {
        LOG_ERROR("Vector addition of part [beta * m_t-1] and [(1 - beta) * bias] in Momentum calculation was unsuccessful.\n");
        freeVector(&temp_left);
        return -1;
    }

    freeVector(&temp_left);

    return 0;
}

/**
 * @brief Updates the learning rate of the model given the type of learning function
 *
 * @param model Model object
 *
 * @return 0 if successful, -1 if failure
 */
int updateLearningRate(Model *model, int epoch)
{
    if (!model)
    {
        LOG_ERROR("Input model for learning rate was not accepted.\n");
        return -1;
    }
    else if (!model->config.learning_rate.init_learning_rate)
    {
        LOG_ERROR("Input learning rate information was not accepted.\n");
        return -1;
    }

    --epoch;

    switch (model->config.learning_rate.decay_type)
    {
    case CONSTANT:
    {
        model->config.learning_rate.curr_learning_rate = model->config.learning_rate.init_learning_rate;
        break;
    }
    case LINEAR_DECAY:
    {
        model->config.learning_rate.curr_learning_rate = model->config.learning_rate.init_learning_rate * (1.0 - (double)epoch / (double)model->config.epochs);
        break;
    }
    case EXPONENTIAL_DECAY:
    {
        model->config.learning_rate.curr_learning_rate = model->config.learning_rate.init_learning_rate * exp(-1.0 * model->config.learning_rate.decay_constant * epoch);
        break;
    }
    case STEP_DECAY:
    {
        model->config.learning_rate.curr_learning_rate = model->config.learning_rate.init_learning_rate * pow(model->config.learning_rate.decay_constant, ((double)epoch / model->config.learning_rate.decay_step));
        break;
    }
    case COSINE_ANNEALING:
    {
        double a = model->config.learning_rate.min_learning_rate;
        double b = 0.5 * (model->config.learning_rate.init_learning_rate - a);
        double c = 1.0 + cos(((double)epoch * 3.141592 / model->config.learning_rate.max_epoch_cycle));
        model->config.learning_rate.curr_learning_rate = a + b * c;
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
        LOG_ERROR("Problem initializing weight Matrix\n");
        return -1;
    }
    if (makeVectorZeros(model->bias, model->classes) < 0)
    {
        LOG_ERROR("Problem initializing bias Matrix\n");
        return -1;
    }

    // Check that the model has been setup correctly before trying to train
    if (checkModel(model) < 0)
    {
        LOG_ERROR("The model object submitted to train has not be setup properly.\n");
        return -1;
    }

    // Convert y matrix to one-hot encoded form if performing softmax regression
    if (model->type == SOFTMAX_REGRESSION)
    {
        computeOneHotEncodedMatrix(*model->y, model->y, model->classes);
    }

    // Init gradient weight Matrix, bias Vector, and velocity Matrix
    Matrix grad_w = {0};
    if (makeMatrixZeros(&grad_w, model->weights->rows, model->weights->cols) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of gradient weights Matrix in model training.\n");
        return -1;
    }
    Vector grad_b = {0};
    if (makeVectorZeros(&grad_b, model->bias->size) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of gradient bias Vector in model training.\n");
        return -1;
    }
    Matrix velocity_weights = {0};
    if (makeMatrixZeros(&velocity_weights, model->weights->rows, model->weights->cols) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of velocity weights Matrix in model training.\n");
        return -1;
    }
    Vector velocity_bias = {0};
    if (makeVectorZeros(&velocity_bias, model->bias->size) < 0)
    {
        LOG_ERROR("Unsuccessful initialization of velocity bias Vector in model training.\n");
        return -1;
    }

    // Init reused variables, build batch sizing
    double loss = 0;
    int *perm_arr = (int *)calloc(model->splitdata.train_features.rows, sizeof(int));
    int batches = (int)ceil(model->splitdata.train_features.rows / (double)model->batch_size);
    int mini_batch_idx = 0;
    int batch_size = 0;
    PBD progress_bar;
    initProgressBar(&progress_bar, 50, '[', ']', '#', '.', 0.1);
    drawProgressBar(&progress_bar);

    // Iterate through N-number of epochs adjusting the weights and bias
    for (int epoch = 1; epoch <= model->config.epochs; ++epoch)
    {
        // --- SHUFFLE DATASET ---

        // Create a random permutation of the number of samples in the dataset
        if (generateRandomPermutation(perm_arr, model->splitdata.train_features.rows) < 0)
        {
            LOG_ERROR("Creating random permutation for input shuffling was unsuccessful.\n");
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
                LOG_ERROR("Problem initializing logits Matrix\n");
                return -1;
            }

            // Get mini-batch of X
            Matrix mini_X = {0};
            if (makeMatrixZeros(&mini_X, batch_size, model->splitdata.train_features.cols) < 0)
            {
                LOG_ERROR("Creation of empty mini-batch X matrix was unsuccessful.\n");
                return -1;
            }
            if (makeMiniMatrix(*model->X, &mini_X, perm_arr, b, batch_size) < 0)
            {
                LOG_ERROR("Creation of mini-batch X matrix was unsuccessful.\n");
                return -1;
            }

            // Get mini-batch of y
            Matrix mini_y = {0};
            if (makeMatrixZeros(&mini_y, batch_size, model->splitdata.train_labels.cols) < 0)
            {
                LOG_ERROR("Creation of empty mini-batch y matrix was unsuccessful.\n");
                return -1;
            }
            if (makeMiniMatrix(*model->y, &mini_y, perm_arr, b, batch_size) < 0)
            {
                LOG_ERROR("Creation of mini-batch X matrix was unsuccessful.\n");
                return -1;
            }

            loss = 0;
            // --- FORWARD PASS ---

            // Compute logits and apply activation function
            if (computeLogits(mini_X, model) < 0)
            {
                LOG_ERROR("Computation of logits was unsuccessful while training model.\n");
                return -1;
            }

            // Compute loss
            if (computeLoss(mini_y, model, &loss) < 0)
            {
                LOG_ERROR("Computation of Loss was unsuccessful while training model.\n");
                return -1;
            }

            // --- BACKWARD PASS (GRADIENTS) ---

            if (clearMatrix(&grad_w) < 0)
            {
                LOG_ERROR("Clearing gradient weights matrix was unsuccessful.\n");
                return -1;
            }
            if (clearVector(&grad_b) < 0)
            {
                LOG_ERROR("Clearing gradient bias vector was unsuccessful.\n");
                return -1;
            }

            // Compute gradients
            if (computeGradients(mini_X, mini_y, model, &grad_w, &grad_b) < 0)
            {
                LOG_ERROR("Computation of Gradient was unsuccessful while training model.\n");
                return -1;
            }

            // Optional regularization
            if (computeRegularization(*model, &grad_w) < 0)
            {
                LOG_ERROR("Computation of Regularization was unsuccessful while training model.\n");
                return -1;
            }

            // Calculate weights velocity matrix
            if (computeVelocityWeights(&velocity_weights, model->beta, grad_w))
            {
                LOG_ERROR("Computation of Weights Momentum was unsuccessful while training model.\n");
                return -1;
            }
            // Calculate biases velocity vector
            if (computeVelocityBias(&velocity_bias, model->beta, grad_b))
            {
                LOG_ERROR("Computation of Biases Momentum was unsuccessful while training model.\n");
                return -1;
            }

            // Gradient descent update with momentum
            if (mat_mul(velocity_weights, model->config.learning_rate.curr_learning_rate, &velocity_weights) < 0)
            {
                LOG_ERROR("Weights gradient descent update with learning rate was not successful.\n");
                return -1;
            }

            if (mat_sub(*model->weights, velocity_weights, model->weights) < 0)
            {
                LOG_ERROR("Weights update with gradient weights was not successful.\n");
                return -1;
            }

            if (vect_mul(velocity_bias, model->config.learning_rate.curr_learning_rate, &velocity_bias) < 0)
            {
                LOG_ERROR("Bias gradient descent update with learning rate was not successful.\n");
                return -1;
            }

            if (vect_sub(*model->bias, velocity_bias, model->bias) < 0)
            {
                LOG_ERROR("Bias update with gradient bias was not successful.\n");
                return -1;
            }

            mini_batch_idx += batch_size;
            freeMatrix(&mini_X);
            freeMatrix(&mini_y);
            freeMatrix(model->logits);
        }
        mini_batch_idx = 0;

        // Update learning rate
        if (updateLearningRate(model, epoch) < 0)
        {
            LOG_ERROR("Bias update with gradient bias was not successful.\n");
            return -1;
        }

        // Progress over time/epoch
        progress_bar.n_curr_len = (epoch * progress_bar.m_max_len) / model->config.epochs;
        progress_bar.loss = loss;
        progress_bar.progress = (int)(((double)epoch / (double)model->config.epochs) * 100.0);
        drawProgressBar(&progress_bar);

        // Save Loss value to array for output
        model->metrics.loss_vs_epochs[epoch-1] = loss;
    }
    LOG_INFO("\n");

    freeMatrix(&grad_w);
    freeMatrix(&velocity_weights);
    freeVector(&grad_b);
    freeVector(&velocity_bias);
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

    // Free metrics arrays
    if (model && model->metrics.loss_vs_epochs)
    {
        free(model->metrics.loss_vs_epochs);
    }
}