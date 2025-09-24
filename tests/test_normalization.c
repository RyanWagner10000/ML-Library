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
    ;

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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_normalization);

    return UNITY_END();
}