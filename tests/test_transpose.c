/*
 * file: test_transpose.c
 * description: script to test the ability of the transpose function
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: tests the transpose of 1d, 2d, 3d matrices
 */

#include "unity.h"
#include <stdio.h>
#include "../header/math_funcs.h"

void setUp(void) {
    // Optional: initialize stuff before each test
}

void tearDown(void) {
    // Optional: clean up after each test
}

void test_transpose_1d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 3, 1, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1};
    Matrix A;
    status = makeMatrix(&A, 1, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 1, 1};
    Matrix ans;
    status = makeMatrix(&ans, 3, 1, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }

    freeMatrix(&result);
    freeMatrix(&A);
    freeMatrix(&ans);
}

void test_transpose_2d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 3, 2, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2};
    Matrix A;
    status = makeMatrix(&A, 2, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 2, 1, 2, 1, 2};
    Matrix ans;
    status = makeMatrix(&ans, 3, 2, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }

    freeMatrix(&result);
    freeMatrix(&A);
    freeMatrix(&ans);
}

void test_transpose_3d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 3, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }

    freeMatrix(&result);
    freeMatrix(&A);
    freeMatrix(&ans);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_transpose_1d);
    RUN_TEST(test_transpose_2d);
    RUN_TEST(test_transpose_3d);

    return UNITY_END();
}