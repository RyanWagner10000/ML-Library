/*
 * file: test_identity.c
 * description: script to test the ability of the identity matric generator function
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#include "unity.h"
#include <stdio.h>
#include "../header/math_funcs.h"

void setUp(void)
{
    // Optional: initialize stuff before each test
}

void tearDown(void)
{
    // Optional: clean up after each test
}

void test_identity_wrong(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 2, 2);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 0);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = identity(&result, 3);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&result);
}

void test_identity_1d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 1, 1, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1};
    Matrix ans;
    status = makeMatrix(&ans, 1, 1, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }

    freeMatrix(&result);
    freeMatrix(&ans);
}

void test_identity_2d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 2, 2, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 0, 0, 1};
    Matrix ans;
    status = makeMatrix(&ans, 2, 2, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 2);
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

void test_identity_3d(void)
{
    int status = -1;

    Matrix result;
    status = makeMatrix(&result, 3, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 0, 0,
                         0, 1, 0,
                         0, 0, 1};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 3);
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

void test_identity_4d(void)
{
    int status = -1;
    Matrix result;
    status = makeMatrix(&result, 4, 4, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};
    Matrix ans;
    status = makeMatrix(&ans, 4, 4, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 4);
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

void test_identity_5d(void)
{
    int status = -1;
    Matrix result;
    status = makeMatrix(&result, 5, 5, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 0, 0, 0, 0,
                         0, 1, 0, 0, 0,
                         0, 0, 1, 0, 0,
                         0, 0, 0, 1, 0,
                         0, 0, 0, 0, 1};
    Matrix ans;
    status = makeMatrix(&ans, 5, 5, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = identity(&result, 5);
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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_identity_wrong);
    RUN_TEST(test_identity_1d);
    RUN_TEST(test_identity_2d);
    RUN_TEST(test_identity_3d);
    RUN_TEST(test_identity_4d);
    RUN_TEST(test_identity_5d);

    return UNITY_END();
}