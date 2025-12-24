/*
 * file: file_handling.c
 * description: used for reading and writing to files for learning or saving a model
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: only focuses on CSV and TXT file right now
 *        CSV files need to adhere to typical CSV filetype formatting
 *        TXT files need to be comma separated as well, in
 */

#include "file_handling.h"

/**
 * @brief Function to get the number of rows and columns in a file
 *
 * @param filename relative or abolsute path to the file
 * @param has_header if the file has a header or not
 * @param rows number of rows in input dataset
 * @param cols number of columns in input dataset
 *
 * @return 0 if successful, -1 if failure
 */
int getColandRowNum(const char *filename, bool has_header, int *rows, int *cols)
{
    // Open file and check for success
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        LOG_ERROR("Error opening CSV file.\n");
        return -1;
    }

    if (has_header)
    {
        --*rows;
    }

    // Buffer to hold line
    char line[2048];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (*rows == 0)
        {
            // Calculate the number of cols in header
            char *token = strtok(line, ",");
            while (token != NULL)
            {
                // Process each token
                ++*cols;
                token = strtok(NULL, ",");
            }
        }

        ++*rows;
    }

    fclose(file);

    return 0;
}

/**
 * @brief Function to put the data in a CSV file into a Matrix object
 *
 * @param filename relative or abolsute path to the file
 * @param has_header if the file has a header or not
 * @param m Matrix object
 *
 * @return 0 if successful, -1 if failure
 */
int loadCSVtoMatrix(const char *filename, bool has_header, Matrix *m)
{
    int rows = 0;
    int cols = 0;
    getColandRowNum(filename, has_header, &rows, &cols);

    m->rows = rows;
    m->cols = cols;
    m->data = calloc(rows * cols, sizeof(double));

    // Open file and check for success
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        LOG_ERROR("Error opening CSV file.\n");
        return -1;
    }

    // Buffer to hold line
    char line[4096];
    int counter = 0;

    if (has_header == 1)
    {
        fgets(line, sizeof(line), file);
    }

    rows = 0;
    cols = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            // Process each token
            char *endptr;
            m->data[rows * m->cols + cols] = strtod(token, &endptr);
            // Get next token
            token = strtok(NULL, ",");
            ++cols;
        }
        ++rows;
        cols = 0;
    }

    fclose(file);

    return 1;
}

/**
 * @brief Function to normalize the data in a Matrix object column-wise
 *
 * @param m Matrix object
 *
 * @return 0 if successful, -1 if failure
 */
int normalizeMatrix(Matrix *m)
{
    if (!m || !m->data)
    {
        LOG_ERROR("Input matrix for normalization was not compatible.\n");
        return -1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double sum_squares = 0.0;
    double std_dev = 0.0;
    double element = 0.0;
    int loop_idx = 0;

    int idx = 0;
    // Calculate mean
    for (int c = 0; c < m->cols; ++c)
    {

        for (int r = 0; r < m->rows; ++r)
        {
            idx = r * m->cols + c;
            element = m->data[idx];

            if (loop_idx == 0)
            {
                // Update Mean values
                sum += element;
            }
            else if (loop_idx == 1)
            {
                // Calculate standard deviation
                sum_squares += ((element - mean) * (element - mean));
            }
            else
            {
                // Apply normalization
                m->data[idx] = (m->data[idx] - mean) / std_dev;
            }
        }

        loop_idx++;

        if (loop_idx == 1)
        {
            // Calculate Mean
            mean = sum / m->rows;
            --c;
        }
        else if (loop_idx == 2)
        {
            // Calculate standard deviation
            std_dev = sqrt(sum_squares / m->rows);
            --c;
        }
        else
        {
            loop_idx = 0;
            sum = 0.0;
            mean = 0.0;
            sum_squares = 0.0;
            std_dev = 0.0;
        }
    }

    return 0;
}

/**
 * @brief Function to get Regression Type string from enum
 *
 * @param type Type of regression
 *
 * @return Const char * of the regression name
 */
const char *regressionTypeEnumToString(RegressionType type)
{
    switch (type)
    {
    case 0:
    {
        return "Linear";
    }
    case 1:
    {
        return "Logistic";
    }
    case 2:
    {
        return "Softmax";
    }
    default:
    {
        return "Null";
    }
    }
}

/**
 * @brief Function to get Regularization Type string from enum
 *
 * @param type Type of regularization
 *
 * @return Const char * of the regularization name
 */
const char *regularizationTypeEnumToString(RegularizationType type)
{
    switch (type)
    {
    case 0:
    {
        return "None";
    }
    case 1:
    {
        return "L1";
    }
    case 2:
    {
        return "L2";
    }
    default:
    {
        return "Null";
    }
    }
}

/**
 * @brief Function to get Decay Type string from enum
 *
 * @param type Type of decay
 *
 * @return Const char * of the decay name
 */
const char *decayTypeEnumToString(DecayType type)
{
    switch (type)
    {
    case 0:
    {
        return "Constant";
    }
    case 1:
    {
        return "Linear";
    }
    case 2:
    {
        return "Exponential";
    }
    case 3:
    {
        return "Step";
    }
    case 4:
    {
        return "Cosine Annealing";
    }
    default:
    {
        return "Null";
    }
    }
}

/**
 * @brief Function to add metrics arrays into JSON format
 *
 * @param json_file cJSON object
 * @param matrix Matrix object
 *
 * @return None
 */
void addMetricsToJSON(cJSON *json_file, ModelMetrics model_metrics)
{
    // Create outer container for all metrics
    cJSON *metrics  = cJSON_CreateArray();

    // Add Loss vs. Epochs arary
    cJSON_AddItemToObject(json_file, "loss", metrics);
    for (int i = 0; i < model_metrics.loss_vs_epochs->size; ++i)
    {
        cJSON *item = cJSON_CreateNumber(model_metrics.loss_vs_epochs->data[i]);

        // Add data to the array
        cJSON_AddItemToArray(metrics, item);
    }
    return;
}

/**
 * @brief Function to translate Matrix object into JSON format
 *
 * @param json_file cJSON object
 * @param name Name of key for JSON file
 * @param matrix Matrix object
 *
 * @return None
 */
void translateMatrixToJSON(cJSON *json_file, const char *name, Matrix matrix)
{
    cJSON *outer_array = cJSON_CreateArray();

    // Add the outer array to the root object with a key, e.g., "matrix"
    cJSON_AddItemToObject(json_file, name, outer_array);

    int idx = 0;
    for (int r = 0; r < matrix.rows; ++r)
    {
        cJSON *inner_array = cJSON_CreateArray();

        // Add the newly created inner array (row) to the outer array
        cJSON_AddItemToArray(outer_array, inner_array);

        for (int c = 0; c < matrix.cols; ++c)
        {
            // Create a JSON number item for the data
            idx = r * matrix.cols + c;
            cJSON *item = cJSON_CreateNumber(matrix.data[idx]);

            // Add the data item to the current inner array
            cJSON_AddItemToArray(inner_array, item);
        }
    }
    return;
}

/**
 * @brief Function to translate Vector object into JSON format
 *
 * @param json_file cJSON object
 * @param name Name of key for JSON file
 * @param vector Vector object
 *
 * @return None
 */
void translateVectorToJSON(cJSON *json_file, const char *name, Vector vector)
{
    cJSON *outer_array = cJSON_CreateArray();

    // Add the outer array to the root object with a key, e.g., "matrix"
    cJSON_AddItemToObject(json_file, name, outer_array);

    int idx = 0;
    for (int i = 0; i < vector.size; ++i)
    {
        cJSON *item = cJSON_CreateNumber(vector.data[idx]);

        // Add the data item to the current inner array
        cJSON_AddItemToArray(outer_array, item);
    }
    return;
}

/**
 * @brief Function to get Activation Function string from enum
 *
 * @param func Activation function
 *
 * @return Const char * of the function name
 */
const char *activationFuncitonEnumToString(Activation func)
{
    switch (func)
    {
    case 0:
    {
        return "None";
    }
    case 1:
    {
        return "Sigmoid";
    }
    case 2:
    {
        return "Sigmoid Derivative";
    }
    case 3:
    {
        return "ReLu";
    }
    case 4:
    {
        return "ReLu Derivative";
    }
    case 5:
    {
        return "Tanh";
    }
    case 6:
    {
        return "Tanh Derivative";
    }
    case 7:
    {
        return "Softmax";
    }
    default:
    {
        return "Null";
    }
    }
}

/**
 * @brief Function to save training model setup and evaluation metrics to JSON file
 *
 * @param filename Name of file output
 * @param model Trained Model type model
 *
 * @return 0 if successful, -1 if failure
 */
int outputData(const char *filename, Model model)
{
    const char *output_folder = "output/";

    // Create a cJSON object
    cJSON *json = cJSON_CreateObject();

    // Add Model params to JSON object
    //   RegressionType Enum
    cJSON_AddStringToObject(json, "regression_type", regressionTypeEnumToString(model.type));
    //   ModelConfig Enum
    cJSON *config;
    cJSON_AddItemToObject(json, "config", config = cJSON_CreateObject());
    cJSON_AddNumberToObject(config, "epochs", model.config.epochs);
    cJSON_AddNumberToObject(config, "lambda", model.config.lambda);
    cJSON_AddStringToObject(config, "regularization", regularizationTypeEnumToString(model.config.regularization));
    cJSON *learning_rate;
    cJSON_AddItemToObject(config, "learning_rate", learning_rate = cJSON_CreateObject());
    cJSON_AddNumberToObject(learning_rate, "init_learning_rate", model.config.learning_rate.init_learning_rate);
    cJSON_AddNumberToObject(learning_rate, "min_learning_rate", model.config.learning_rate.min_learning_rate);
    cJSON_AddNumberToObject(learning_rate, "curr_learning_rate", model.config.learning_rate.curr_learning_rate);
    cJSON_AddNumberToObject(learning_rate, "max_epoch_cycle", model.config.learning_rate.max_epoch_cycle);
    cJSON_AddStringToObject(learning_rate, "decay_type", decayTypeEnumToString(model.config.learning_rate.decay_type));
    cJSON_AddNumberToObject(learning_rate, "decay_step", model.config.learning_rate.init_learning_rate);
    cJSON_AddNumberToObject(learning_rate, "decay_constant", model.config.learning_rate.init_learning_rate);
    //   ModelMetrics Object
    addMetricsToJSON(json, model.metrics);
    //   Weights Matrix
    translateMatrixToJSON(json, "weights", *model.weights);
    //   Bias Vector
    translateVectorToJSON(json, "bias", *model.bias);
    //   Activation Function
    cJSON_AddStringToObject(json, "activation_function", activationFuncitonEnumToString(model.func));

    cJSON_AddNumberToObject(json, "batch_size", model.batch_size);
    cJSON_AddNumberToObject(json, "classes", model.classes);
    cJSON_AddNumberToObject(json, "beta", model.beta);

    // Convert the cJSON object to a JSON string
    char *json_str = cJSON_Print(json);

    // Write the JSON string to a file
    int path_length = strlen(output_folder) + strlen(filename) + 1;
    char *file_path = (char *)malloc(path_length + 1);
    snprintf(file_path, path_length, "%s%s", output_folder, filename);

    // Open JSON file and put data
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL)
    {
        printf("Error: Unable to open the file.\n");
        return -1;
    }
    fputs(json_str, fp);
    fclose;

    // Free filepath variable
    free(file_path);
    // free the JSON string and cJSON object
    cJSON_free(json_str);
    cJSON_Delete(json);
    return 0;
}