/*
 * file: math_funcs.c
 * description: script with all the self written math functions
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: All math functions are row-wise vectors/matrices in memory
 */

#include "../header/math_funcs.h"

/**
 * @brief Performs the dot product of two vectors
 *
 * @param x Pointer to x of Vector type
 * @param y Pointer to y of Vector type
 * @param result of the dot product operation
 *
 * @return 0 on success and -1 on failure
 */
int dot_product(Vector x, Vector y, double *result)
{
    // If they ain't the right size then exit on failure
    if (x.size != y.size)
    {
        printf("Size of input Vectors do not match for dot product operation.\n");
        printf("Size of Vector x = %d.\n", x.size);
        printf("Size of Vector y = %d.\n", y.size);
        return -1;
    }
    if (x.size <= 0)
    {
        printf("Size of input Vector x in <= 0.\n");
        return -1;
    }
    if (y.size <= 0)
    {
        printf("Size of input Vector y in <= 0.\n");
        return -1;
    }

    // Perform sum of products
    *result = 0.0;
    for (int i = 0; i < x.size; ++i)
    {
        *result += x.data[i] * y.data[i];
    }

    return 0;
}

/**
 * @brief Matrix and Vector multiplication operation
 *
 * @param A Matrix of size MxN of Matrix type
 * @param x Vector of size N of Vector type
 * @param result Calculated vector of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int matvec_mult(Matrix A, Vector x, Vector *result)
{
    // If sizes don't match, then exit on failure
    if (A.cols != x.size)
    {
        return -1;
    }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.rows; ++i)
    {

        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i] += A.data[i * A.cols + j] * x.data[j];
        }
    }

    return 0;
}

/**
 * @brief Matrix multiplication: A * B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Matrix of size NxP of Matrix type
 * @param result Calculated Matrix of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int mat_mul_matrix(Matrix A, Matrix B, Matrix *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix multiplication.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.cols != B.rows)
    {
        printf("Matrix shapes do not match. Cannot perform matrix multiplication.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        printf("Input result matrix was unitialized. Zeroing input results matrix.\n");
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    else if (result->cols != B.cols || result->rows != A.rows)
    {
        // Check if the input result matrix is the right shape if it's already inited
        printf("Input result matrix dimensions do match input A or B. Freeing, resizing, and zeroing input result matrix.\n");
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    else
    {
        // Clear Matrix for a clean slate
        if (clearMatrix(result) < 0)
        {
            printf("Could not set Matrix object to all 0's\n");
            return -1;
        }
    }

    // Create temp variables for math operations
    Vector curr_row = {0};
    if (makeVectorZeros(&curr_row, A.cols) < 0)
    {
        printf("Could not init temp row vector in matrix multiplication function.\n");
        return -1;
    }
    Vector curr_col = {0};
    if (makeVectorZeros(&curr_col, B.rows) < 0)
    {
        printf("Could not init temp col vector in matrix multiplication function.\n");
        return -1;
    }

    // Perform sum of products for rows in Matrix
    double temp_result = 0.0;
    for (int r = 0; r < A.rows; ++r)
    {
        if (getRowMatrix(A, r, &curr_row) < 0)
        {
            printf("Could not get row from Matrix A doing matrix multiplication.\n");
            return -1;
        }

        for (int c = 0; c < B.cols; ++c)
        {

            if (getColMatrix(B, c, &curr_col) < 0)
            {
                printf("Could not get column from Matrix B doing matrix multiplication.\n");
                return -1;
            }

            if (dot_product(curr_row, curr_col, &temp_result) < 0)
            {
                printf("Could not do dot product for row and column for matrix multiplication.\n");
                return -1;
            }

            int idx = r * result->cols + c;
            result->data[idx] = temp_result;
        }
    }

    return 0;
}

/**
 * @brief Matrix multiplication: A * B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Double to multiply element-wise to A
 * @param result Calculated Matrix of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int mat_mul_double(Matrix A, double B, Matrix *result)
{
    // Test inputs
    if (!A.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix multiplication.\n");
        return -1;
    }

    // Test inputs
    if (!result)
    {
        printf("Input variables could not pass inital tests for matrix multiplication.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != A.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise multiplication for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] * B;
        }
    }

    return 0;
}

/**
 * @brief Matrix addition: A + B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Matrix of size NxP of Matrix type
 * @param result Calculated Matrix of addition process
 *
 * @return 0 on success and -1 on failure
 */
int mat_add_matrix(Matrix A, Matrix B, Matrix *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix addition.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.cols != B.cols || A.rows != B.rows)
    {
        printf("Matrix shapes do not match. Cannot perform matrix addition.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != B.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise addition for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] + B.data[i * A.cols + j];
        }
    }

    return 0;
}

/**
 * @brief Matrix addition: A + B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Double to add element-wise to A
 * @param result Calculated Matrix of addition process
 *
 * @return 0 on success and -1 on failure
 */
int mat_add_double(Matrix A, double B, Matrix *result)
{
    // Test inputs
    if (!A.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix addition.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != A.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise addition for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] + B;
        }
    }

    return 0;
}

/**
 * @brief Matrix subtraction: A - B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Matrix of size NxP of Matrix type
 * @param result Calculated Matrix of subtraction process
 *
 * @return 0 on success and -1 on failure
 */
int mat_sub_matrix(Matrix A, Matrix B, Matrix *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix subtraction.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.cols != B.cols || A.rows != B.rows)
    {
        printf("Matrix shapes do not match. Cannot perform matrix subtraction.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != B.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, B.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise addition for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] - B.data[i * A.cols + j];
        }
    }

    return 0;
}

/**
 * @brief Matrix subtraction: A - B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Double to subtract element-wise to A
 * @param result Calculated Matrix of subtraction process
 *
 * @return 0 on success and -1 on failure
 */
int mat_sub_double(Matrix A, double B, Matrix *result)
{
    // Test inputs
    if (!A.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix subtraction.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != A.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise subtraction for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] - B;
        }
    }

    return 0;
}

/**
 * @brief Matrix division: A / B
 *
 * @param A Matrix of size MxN of Matrix type
 * @param B Double to divide element-wise to A
 * @param result Calculated Matrix of division process
 *
 * @return 0 on success and -1 on failure
 */
int mat_div_double(Matrix A, double B, Matrix *result)
{
    // Test inputs
    if (!A.data || !result)
    {
        printf("Input variables could not pass inital tests for matrix division.\n");
        return -1;
    }

    // Test inputs
    if (!result)
    {
        printf("Input variables could not pass inital tests for matrix division.\n");
        return -1;
    }

    // Check if the resulting matrix is initialized or has been inited to zero or less
    if (!initialized_matrix(result) || result->cols <= 0 || result->rows <= 0)
    {
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Check if the input result matrix is the right shape if it's already inited
    else if (result->cols != A.cols || result->rows != A.rows)
    {
        // Free and remake matrix properly
        freeMatrix(result);
        if (makeMatrixZeros(result, A.rows, A.cols) < 0)
        {
            printf("Error initializing zero output matrix.\n");
            return -1;
        }
    }
    // Clear Matrix for a clean slate
    // else
    // {
    //     if (clearMatrix(result) < 0)
    //     {
    //         printf("Could not set Matrix object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform element-wise division for Matrix
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < A.cols; ++j)
        {
            result->data[i * A.cols + j] = A.data[i * A.cols + j] / B;
        }
    }

    return 0;
}

/**
 * @brief Vector * Vector multiplication: A * B
 *
 * @param A Vector object to be multiplied element-wise
 * @param B Vector object to be multiplied element-wise
 * @param result Calculated Vector of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int vect_mul_vector(Vector A, Vector B, Vector *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for vector multiplication.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.size != B.size)
    {
        printf("Vector shapes do not match. Cannot perform vector multiplication.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != B.size || result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] * B.data[i];
    }

    return 0;
}

/**
 * @brief Vector * Double multiplication: A * B
 *
 * @param A Vector object to be multiplied element-wise
 * @param B Double to be multiplied element-wise
 * @param result Calculated Vector of multiplication process
 *
 * @return 0 on success and -1 on failure
 */
int vect_mul_double(Vector A, double B, Vector *result)
{
    // Test inputs
    if (!A.data || !B || !result)
    {
        printf("Input variables could not pass inital tests for vector addition.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] * B;
    }

    return 0;
}

/**
 * @brief Vector addition: A + B
 *
 * @param A Vector to be added element-wise
 * @param B Vector to add element-wise to A
 * @param result Calculated Vector of addition process
 *
 * @return 0 on success and -1 on failure
 */
int vect_add_vector(Vector A, Vector B, Vector *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for vector addition.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.size != B.size)
    {
        printf("Vector shapes do not match. Cannot perform vector addition.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != B.size || result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output Vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] + B.data[i];
    }

    return 0;
}

/**
 * @brief vector addition: A + B
 *
 * @param A Vector to be added element-wise
 * @param B Vector to add element-wise to A
 * @param result Calculated vector of addition process
 *
 * @return 0 on success and -1 on failure
 */
int vect_add_double(Vector A, double B, Vector *result)
{
    // Test inputs
    if (!A.data || !B || !result)
    {
        printf("Input variables could not pass inital tests for vector addition.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] + B;
    }

    return 0;
}

/**
 * @brief vector subtraction: A - B
 *
 * @param A Vector to be subtracted element-wise
 * @param B Vector to subtract element-wise to A
 * @param result Calculated Vector of subtraction process
 *
 * @return 0 on success and -1 on failure
 */
int vect_sub_vector(Vector A, Vector B, Vector *result)
{
    // Test inputs
    if (!A.data || !B.data || !result)
    {
        printf("Input variables could not pass inital tests for vector subtraction.\n");
        return -1;
    }

    // If sizes don't match, then exit on failure
    if (A.size != B.size)
    {
        printf("Vector shapes do not match. Cannot perform vector subtraction.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != B.size || result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output Vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] - B.data[i];
    }

    return 0;
}

/**
 * @brief Vector subtraction: A - B
 *
 * @param A Vector to be subtracted element-wise
 * @param B Double to subtract element-wise to A
 * @param result Calculated Vector of subtraction process
 *
 * @return 0 on success and -1 on failure
 */
int vect_sub_double(Vector A, double B, Vector *result)
{
    // Test inputs
    if (!A.data || !B || !result)
    {
        printf("Input variables could not pass inital tests for vector subtraction.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Clear Vector for a clean slate
    // else
    // {
    //     if (clearVector(result) < 0)
    //     {
    //         printf("Could not set Vector object to all 0's\n");
    //         return -1;
    //     }
    // }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] - B;
    }

    return 0;
}

/**
 * @brief Vector division: A/B
 *
 * @param A Vector to be divided element-wise
 * @param B Double to divide element-wise to A
 * @param result Calculated Vector of division process
 *
 * @return 0 on success and -1 on failure
 */
int vect_div_double(Vector A, double B, Vector *result)
{
    // Test inputs
    if (!A.data || !B || !result)
    {
        printf("Input variables could not pass inital tests for vector division.\n");
        return -1;
    }

    // Check if the resulting vector is initialized or has been inited to zero or less
    if (!initialized_vector(result) || result->size <= 0)
    {
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }
    // Check if the input result vector is the right shape and if it's already inited
    else if (result->size != A.size)
    {
        // Free and remake vector properly
        freeVector(result);
        if (makeVectorZeros(result, A.size) < 0)
        {
            printf("Error initializing zero output vector.\n");
            return -1;
        }
    }

    // Perform sum of products for rows in Matrix
    for (int i = 0; i < A.size; ++i)
    {
        result->data[i] = A.data[i] / B;
    }

    return 0;
}

/**
 * @brief Performs the transpose of the input matrix
 *
 * @param A Matrix to be transposed
 * @param A_t Transposed matrix
 *
 * @return 0 on success and -1 on failure
 */
int transpose(Matrix A, Matrix *A_t)
{
    if (!A.data)
    {
        return -1;
    }

    A_t->rows = A.cols;
    A_t->cols = A.rows;

    // Iterate COLUMN-wise through row-matrix to transpose it
    for (int r = 0; r < A.rows; ++r)
    {
        for (int c = 0; c < A.cols; ++c)
        {
            A_t->data[c * A_t->cols + r] = A.data[r * A.cols + c];
        }
    }

    return 0;
}

/**
 * @brief Makes an identity matrix of given size
 *
 * @param A Matrix pointer to fill with identity values
 * @param size of the MxM identity matrix desired
 *
 * @return 0 on success and -1 on failure
 */
int identity(Matrix *A, int size)
{
    if (size < 1)
    {
        return -1;
    }

    A->cols = size;
    A->rows = size;

    A->data = calloc(size * size, sizeof(double));
    for (int i = 0; i < size; ++i)
    {
        A->data[i * size + i] = 1.0;
    }

    return 0;
}

/**
 * @brief Sigmoid function: (1/(1 + e^(-x)))
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid(double x, double *x_out)
{
    double temp = 1.0 / (1.0 + exp(-1.0 * x));
    *x_out = temp;

    return 0;
}

/**
 * @brief Derivative of sigmoid function: (1/(1 + e^(-x))) * (1 - (1/(1 + e^(-x))))
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int sigmoid_dx(double x, double *x_out)
{
    double temp_out = 0.0;
    if (sigmoid(x, &temp_out) < 0)
    {
        x_out = NULL;
        return -1;
    }
    *x_out = temp_out * (1 - temp_out);

    return 0;
}

/**
 * @brief ReLu function: maximum of 0 or x
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu(double x, double *x_out)
{
    *x_out = x > 0 ? x : 0;

    return 0;
}

/**
 * @brief ReLu derivative function
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int relu_dx(double x, double *x_out)
{
    *x_out = x > 0 ? 1.0 : 0;

    return 0;
}

/**
 * @brief Tanh function: tanh(x)
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int tanh_(double x, double *x_out)
{
    *x_out = tanh(x);

    return 0;
}

/**
 * @brief Tanh derivative function: 1-(tanh(x))^2
 *
 * @param x Input variable
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int tanh_dx(double x, double *x_out)
{
    double x_ = tanh(x);
    *x_out = (1 - x_ * x_);

    return 0;
}

/**
 * @brief Softmax function: (e^(z_i)) / (SUM(e^(z_j)))
 *
 * @param x_i Input variable
 * @param x_j Input Vector
 * @param x_out Pointer to output varaible from function
 *
 * @return 0 if successful, -1 if failure
 */
int softmax(double x_i, Vector x_j, double *x_out)
{
    if (!x_j.data)
    {
        printf("Problem with input variables put into softmax activation funciton.\n");
        return -1;
    }

    double sum_x_j = 0.0;
    double maxx = x_j.data[0];

    // Get max value in vector
    for (int j = 0; j < x_j.size; ++j)
    {
        if (x_j.data[j] > maxx)
        {
            maxx = x_j.data[j];
        }
    }

    // Calculate the denominator
    for (int j = 0; j < x_j.size; ++j)
    {
        sum_x_j += exp(x_j.data[j] - maxx);
    }

    *x_out = exp(x_i - maxx) / sum_x_j;

    return 0;
}

/**
 * @brief Function to apply an activation function to a Vector
 *
 * @param v Vector pointer to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
int applyToVector(Vector *v, Activation func)
{
    double temp_out = 0;
    for (int i = 0; i < v->size; ++i)
    {
        switch (func)
        {
        case SIGMOID:
        {
            if (sigmoid(v->data[i], &temp_out) < 0)
            {
                printf("Error applying sigmoid function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        case SIGMOID_DX:
        {
            if (sigmoid_dx(v->data[i], &temp_out) < 0)
            {
                printf("Error applying sigmoid dx function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        case RELU:
        {
            if (relu(v->data[i], &temp_out) < 0)
            {
                printf("Error applying relu function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        case RELU_DX:
        {
            if (relu_dx(v->data[i], &temp_out) < 0)
            {
                printf("Error applying relu dx function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        case TANH:
        {
            if (tanh_(v->data[i], &temp_out) < 0)
            {
                printf("Error applying tanh function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        case TANH_DX:
        {
            if (tanh_dx(v->data[i], &temp_out) < 0)
            {
                printf("Error applying tanh dx function to each element in vector.\n");
                v->data = NULL;
                return -1;
            }
            v->data[i] = temp_out;
            break;
        }
        default:
        {
            printf("Input Activation type was not recognized.\n");
            return -1;
        }
        }
    }

    return 0;
}

/**
 * @brief Function to apply an activation function to a Matrix
 *
 * @param m Matrix to apply activation function to
 * @param func Activation functino to apply
 *
 * @return None
 */
int applyToMatrix(Matrix *m, Activation func)
{

    double temp_out = 0;
    for (int r = 0; r < m->rows; ++r)
    {
        for (int c = 0; c < m->cols; ++c)
        {
            int idx = r * m->cols + c;
            switch (func)
            {
            case SIGMOID:
            {
                if (sigmoid(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying sigmoid function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            case SIGMOID_DX:
            {
                if (sigmoid_dx(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying sigmoid dx function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            case RELU:
            {
                if (relu(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying relu function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            case RELU_DX:
            {
                if (relu_dx(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying relu dx function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            case TANH:
            {
                if (tanh_(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying tanh function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            case TANH_DX:
            {
                if (tanh_dx(m->data[idx], &temp_out) < 0)
                {
                    printf("Error applying tanh dx function to each element in matrix.\n");
                    m->data = NULL;
                    return -1;
                }
                m->data[idx] = temp_out;
                break;
            }
            default:
            {
                printf("Input Activation type was not recognized.\n");
                return -1;
            }
            }
        }
    }

    return 0;
}
