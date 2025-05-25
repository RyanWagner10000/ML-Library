#include "unity.h"
#include <stdio.h>
#include "../header/math_funcs.h"

void setUp(void) {
    // Optional: initialize stuff before each test
}

void tearDown(void) {
    // Optional: clean up after each test
}

void test_identity_1d(void)
{
    Matrix result;
    int status = -1;

    double init_ans[] = {1};
    Matrix ans = makeMatrix(init_ans, 1, 1);

    status = identity(&result, 1);

    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_identity_2d(void)
{
    Matrix result;
    int status = -1;

    double init_ans[] = {1, 0, 0, 1};
    Matrix ans = makeMatrix(init_ans, 2, 2);

    status = identity(&result, 2);

    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        char msg[128];
        snprintf(msg, sizeof(msg), "Mismatch at [%d]", i);
        // TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.0001f, (float)ans.data[i], (float)result.data[i], msg);
    }
}

void test_identity_3d(void)
{
    Matrix result;
    int status = -1;

    double init_ans[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    Matrix ans = makeMatrix(init_ans, 3, 3);

    status = identity(&result, 3);

    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_identity_4d(void)
{
    Matrix result;
    int status = -1;

    double init_ans[] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};
    Matrix ans = makeMatrix(init_ans, 4, 4);

    status = identity(&result, 4);

    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_ans); ++i)
    {
        char msg[128];
        snprintf(msg, sizeof(msg), "Mismatch at [%d]", i);
        // TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.0001f, (float)ans.data[i], (float)result.data[i], msg);
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_identity_1d);
    RUN_TEST(test_identity_2d);
    RUN_TEST(test_identity_3d);
    RUN_TEST(test_identity_4d);

    return UNITY_END();
}