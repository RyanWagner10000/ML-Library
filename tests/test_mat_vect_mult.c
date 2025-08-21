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
    int status = -1;

    Vector result;
    status = makeVector(&result, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {0, 0, 0};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0, 0, 0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }
}

void test_mat_vect_mult(void)
{
    int status = -1;

    Vector result;
    status = makeVector(&result, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {6, 12, 18};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }
}

void test_mat_vect_mult_neg_vector(void)
{
    int status = -1;

    Vector result;
    status = makeVector(&result, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {-1, 2, -3};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {-2, -4, -6};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }
}

void test_mat_vect_mult_neg_matrix(void)
{
    int status = -1;

    Vector result;
    status = makeVector(&result, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {-1, 1, 1, 2, -2, 2, 3, 3, -3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {4, 4, 0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < LEN(init_a); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)result.data)[i]);
    }
}

void test_mat_vect_mult_wrong_size_vector(void)
{
    int status = -1;

    Vector result;
    status = makeVector(&result, 2, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 3, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_mat_vect_mult_wrong_size_matrix(void)
{
    int status = -1;

    Vector result;
    status = makeVector(&result, 3, NULL, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3};
    Vector a;
    status = makeVector(&a, LEN(init_a), &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_A[] = {1, 1, 2, 2, 3, 3};
    Matrix A;
    status = makeMatrix(&A, 3, 2, &init_A, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(A, a, &result);
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