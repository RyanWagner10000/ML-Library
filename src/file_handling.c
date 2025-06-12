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

int getColandRowNum(const char *filename, bool has_header, int *rows, int *cols)
{
    // Open file and check for success
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening CSV file");
        return -1;
    }

    // Buffer to hold line
    char line[2048];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (rows == 0)
        {
            // Calculate the number of cols in header
            char *token = strtok(line, ",");
            while (token != NULL)
            {
                // Process each token
                // printf("Token: %s\n", token);
                ++cols;
                // Get next token
                token = strtok(NULL, ",");
            }
        }

        ++rows;
    }

    fclose(file);

    return 0;
}

int loadCSVtoMatrix(const char *filename, bool has_header, Matrix *m)
{
    // Open file and check for success
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening CSV file");
        return -1;
    }

    // Buffer to hold line
    char line[2048];
    int counter = 0;
    
    if (has_header == 1)
    {
        fgets(line, sizeof(line), file);
        // printf("%s",line);
    }
    
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("Row: %s\n",line);
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            // Process each token
            printf("Token: %s\n", token);
            // Get next token
            token = strtok(NULL, ",");
        }
        
    }

    fclose(file);

    return 1;
}

// Vector loadCSVtoVector(const char *filename, int col_idx, int has_header)
// {
//     return;
// }