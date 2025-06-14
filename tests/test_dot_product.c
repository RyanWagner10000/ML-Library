/*
 * file: test_dot_pruduct.c
 * description: script to test the ability of the dot product funciton
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes: 
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

void test_dot_product_zeros(void)
{
    double result = 0.0;
    int status = -1;

    double init[] = {0};

    Vector a;
    status = makeVector(&a, LEN(init), &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init), &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(0, status);
}

void test_dot_product_different_length_arrays1(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {0, 1};
    double init_b[] = {0};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_dot_product_different_length_arrays2(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {0};
    double init_b[] = {0, 1};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_dot_product_pos_arrays(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double init_b[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 220.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(0, status);
}

void test_dot_product_pos_neg_arrays1(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
    double init_b[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -220.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(0, status);
}

void test_dot_product_pos_neg_arrays2(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double init_b[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -220.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(0, status);
}

void test_dot_product_neg_arrays(void)
{
    double result = 0.0;
    int status = -1;

    double init_a[] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
    double init_b[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1};

    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    Vector b;
    status = makeVector(&b, LEN(init_b), &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);
    
    status = dot_product(&a, &b, &result);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 220.0f, (float)result);
    TEST_ASSERT_EQUAL_INT(0, status);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dot_product_zeros);
    RUN_TEST(test_dot_product_different_length_arrays1);
    RUN_TEST(test_dot_product_different_length_arrays2);
    RUN_TEST(test_dot_product_pos_arrays);
    RUN_TEST(test_dot_product_pos_neg_arrays1);
    RUN_TEST(test_dot_product_pos_neg_arrays2);
    RUN_TEST(test_dot_product_neg_arrays);

    return UNITY_END();
}