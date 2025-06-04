/*
 * file: test_mat_vect_mult,c
 * description: script to test the vector and matrix multiplication
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

void test_mat_vect_mult_zeros(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {0, 0, 0};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 3);

    double init_ans[] = {0, 0, 0};
    Vector ans = makeVector(init_ans, LEN(init_ans));

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_mat_vect_mult(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {1, 2, 3};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 3);

    double init_ans[] = {6, 12, 18};
    Vector ans = makeVector(init_ans, LEN(init_ans));

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_mat_vect_mult_neg_vector(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {-1, 2, -3};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 3);

    double init_ans[] = {-2, -4, -6};
    Vector ans = makeVector(init_ans, LEN(init_ans));

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_mat_vect_mult_neg_matrix(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {1, 2, 3};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {-1, 1, 1, 2, -2, 2, 3, 3, -3};
    Matrix A = makeMatrix(init_A, 3, 3);

    double init_ans[] = {4, 4, 0};
    Vector ans = makeVector(init_ans, LEN(init_ans));

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[i], (float)result.data[i]);
    }
}

void test_mat_vect_mult_wrong_size_vector(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {1, 2};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 3);

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_mat_vect_mult_wrong_size_matrix(void)
{
    Vector result;
    int status = -1;

    double init_a[] = {1, 2, 3};
    Vector a = makeVector(init_a, LEN(init_a));

    double init_A[] = {1, 1, 2, 2, 3, 3};
    Matrix A = makeMatrix(init_A, 3, 2);

    status = matvec_mult(&A, &a, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_mat_vect_mult_zeros);
    RUN_TEST(test_mat_vect_mult);
    RUN_TEST(test_mat_vect_mult_neg_vector);
    RUN_TEST(test_mat_vect_mult_neg_matrix);
    RUN_TEST(test_mat_vect_mult_wrong_size_vector);
    RUN_TEST(test_mat_vect_mult_wrong_size_matrix);

    return UNITY_END();
}