/*
 * file: test_vect_vect_mult,c
 * description: script to test the vector multiplication, addition, subtraction, and division functions
 * author: Ryan Wagner
 * date: August 23, 2025
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

void test_vect_multiply_double(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_mul(a, 2.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&result);
    freeVector(&a);
    freeVector(&ans);
}

void test_vect_multiply_vector(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 9, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 4, 6, 4, 6, 2, 6, 4, 2};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_mul(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
    freeVector(&ans);
}

void test_vect_multiply_vector_bad_sizes(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 6, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_mul(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
}

void test_vect_addition_double(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_add(a, 1.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&result);
    freeVector(&a);
    freeVector(&ans);
}

void test_vect_addition_vector(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 9, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {3, 4, 5, 4, 5, 3, 5, 4, 3};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_add(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
    freeVector(&ans);
}

void test_vect_addition_vector_bad_sizes(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 6, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_add(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
}

void test_vect_subtraction_double(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_sub(a, 1.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&result);
    freeVector(&a);
    freeVector(&ans);
}

void test_vect_subtraction_vector(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 9, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {-1, 0, 1, 0, 1, -1, 1, 0, -1};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_sub(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
    freeVector(&ans);
}

void test_vect_subtraction_vector_bad_sizes(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Vector b = {0};
    status = makeVector(&b, 6, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_sub(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeVector(&a);
    freeVector(&b);
    freeVector(&result);
}

void test_vect_division_double(void)
{
    int status = -1;

    Vector result = {0};
    status = makeVectorZeros(&result, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = vect_div(a, 2.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < result.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)result.data[r]);
    }

    freeVector(&result);
    freeVector(&a);
    freeVector(&ans);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_vect_multiply_double);
    RUN_TEST(test_vect_multiply_vector);
    RUN_TEST(test_vect_multiply_vector_bad_sizes);

    RUN_TEST(test_vect_addition_double);
    RUN_TEST(test_vect_addition_vector);
    RUN_TEST(test_vect_addition_vector_bad_sizes);

    RUN_TEST(test_vect_subtraction_double);
    RUN_TEST(test_vect_subtraction_vector);
    RUN_TEST(test_vect_subtraction_vector_bad_sizes);

    RUN_TEST(test_vect_division_double);

    return UNITY_END();
}