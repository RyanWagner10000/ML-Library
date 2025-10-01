/*
 * file: test_normalization.c
 * description: script to test the normalization of a matrix
 * author: Ryan Wagner
 * date: September 23, 2025
 * notes:
 */

#include "unity.h"
#include <stdio.h>
#include "../header/file_handling.h"

void setUp(void)
{
    // Optional: initialize stuff before each test
}

void tearDown(void)
{
    // Optional: clean up after each test
}

void test_normalization(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 4, 5);
    TEST_ASSERT_EQUAL_INT(0, status);

    int idx = 0;
    for (int r = 0; r < result.rows; ++r)
    {
        for (int c = 0; c < result.cols; ++c)
        {
            idx = r * result.cols + c;
            result.data[idx] = (double)idx;
        }
    }

    float init_ans[] = {-1.34164079, -1.34164079, -1.34164079, -1.34164079, -1.34164079,
                        -0.4472136, -0.4472136, -0.4472136, -0.4472136, -0.4472136,
                        0.4472136, 0.4472136, 0.4472136, 0.4472136, 0.4472136,
                        1.34164079, 1.34164079, 1.34164079, 1.34164079, 1.34164079};
    Matrix ans;
    status = makeMatrix(&ans, 4, 5, &init_ans, TYPE_FLOAT);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = normalizeMatrix(&result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        char msg[128];
        snprintf(msg, sizeof(msg), "Mismatch at [%d]", i);
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }

    freeMatrix(&result);
    freeMatrix(&ans);
}

void test_test_train_valid_split(void)
{
    int status = -1;

    Matrix input = {0};
    double init_input[] = {
        0, 0, 0,
        1, 1, 1,
        2, 2, 2,
        3, 3, 3,
        4, 4, 4,
        5, 5, 5,
        6, 6, 6,
        7, 7, 7,
        8, 8, 8,
        9, 9, 9,
        10, 10, 10};
    status = makeMatrix(&input, 11, 3, &init_input, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix labels = {0};
    double init_labels[] = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10};
    status = makeMatrix(&labels, 11, 1, &init_labels, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    SplitData splitdata = makeDefaultSplitData();

    status = splitData(input, labels, 50, 30, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    // printf("train_features = \n");
    // printMatrix(splitdata.train_features);
    // printf("train_labels = \n");
    // printMatrix(splitdata.train_labels);
    // printf("test_features = \n");
    // printMatrix(splitdata.test_features);
    // printf("test_labels = \n");
    // printMatrix(splitdata.test_labels);
    // printf("valid_features = \n");
    // printMatrix(splitdata.valid_features);
    // printf("valid_labels = \n");
    // printMatrix(splitdata.valid_labels);
    
    freeMatrix(&input);
    freeMatrix(&labels);
    freeSplitData(&splitdata);
}

void test_test_train_valid_split_wrong(void)
{
    int status = -1;

    Matrix input = {0};
    double init_input[] = {
        0, 0, 0,
        1, 1, 1,
        2, 2, 2,
        3, 3, 3,
        4, 4, 4,
        5, 5, 5,
        6, 6, 6,
        7, 7, 7,
        8, 8, 8,
        9, 9, 9,
        10, 10, 10};
    status = makeMatrix(&input, 11, 3, &init_input, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix labels = {0};
    double init_labels[] = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10};
    status = makeMatrix(&labels, 11, 1, &init_labels, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    SplitData splitdata = makeDefaultSplitData();

    // Invalid first percentage
    status = splitData(input, labels, 0, 30, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid first percentage
    status = splitData(input, labels, -10, 30, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid second percentage
    status = splitData(input, labels, 50, 0, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid second percentage
    status = splitData(input, labels, 50, -10, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid third percentage
    status = splitData(input, labels, 50, 50, -10, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid total percentage, over
    status = splitData(input, labels, 50, 50, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    // Invalid total percentage, under
    status = splitData(input, labels, 10, 10, 20, &splitdata);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&input);
    freeMatrix(&labels);
    freeSplitData(&splitdata);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_normalization);
    RUN_TEST(test_test_train_valid_split);
    RUN_TEST(test_test_train_valid_split_wrong);

    return UNITY_END();
}