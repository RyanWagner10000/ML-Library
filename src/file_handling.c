/*
 * file: file_handling.c
 * description: used for reading and writing to files for learning or saving a model
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: only focuses on CSV and TXT file right now
 *        CSV files need to adhere to typical CSV filetype formatting
 *        TXT files need to be comma separated as well, in
 */

#include "../header/file_handling.h"

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
        perror("Error opening CSV file");
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
                // printf("Token: %s\n", token);
                ++*cols;
                // Get next token
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

    // printf("Rows = %d\n", rows);
    // printf("Cols = %d\n", cols);

    m->rows = rows;
    m->cols = cols;
    m->data = calloc(rows * cols, sizeof(double));

    // Open file and check for success
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening CSV file");
        return -1;
    }

    // Buffer to hold line
    char line[4096];
    int counter = 0;

    if (has_header == 1)
    {
        fgets(line, sizeof(line), file);
        // printf("%s",line);
    }

    rows = 0;
    cols = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // printf("Row: %s\n",line);
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            // Process each token
            // printf("Token: %s\n", token);
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
        printf("Input matrix for normalization was not compatible.\n");
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
