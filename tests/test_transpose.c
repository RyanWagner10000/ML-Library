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
    Matrix result;
    int status = -1;

    double init_A[] = {1, 1, 1};
    Matrix A = makeMatrix(init_A, 1, 3);

    double init_ans[] = {1, 1, 1};
    Matrix ans = makeMatrix(init_ans, 3, 1);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_transpose_2d(void)
{
    Matrix result;
    int status = -1;

    double init_A[] = {1, 1, 1, 2, 2, 2};
    Matrix A = makeMatrix(init_A, 2, 3);

    double init_ans[] = {1, 2, 1, 2, 1, 2};
    Matrix ans = makeMatrix(init_ans, 3, 2);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_transpose_3d(void)
{
    Matrix result;
    int status = -1;

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 3);

    double init_ans[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    Matrix ans = makeMatrix(init_ans, 3, 3);

    status = transpose(&A, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_A); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_transpose_1d);
    RUN_TEST(test_transpose_2d);
    RUN_TEST(test_transpose_3d);

    return UNITY_END();
}