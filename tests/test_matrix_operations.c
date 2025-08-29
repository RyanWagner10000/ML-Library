/*
 * file: test_matrix_math.c
 * description: script to test the matrix multiplication, addition, subtraction, and division functions
 * author: Ryan Wagner
 * date: August 22, 2025
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

void test_mat_multiply_double(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_mul(a, 2.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&result);
    freeMatrix(&a);
    freeVector(&ans);
}

void test_mat_multiply_matrix(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 3, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {12, 12, 12, 12, 12, 12, 12, 12, 12};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_mul(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
    freeVector(&ans);
}

void test_mat_multiply_matrix_bad_sizes(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 2, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_mul(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
}

void test_mat_addition_double(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_add(a, 1.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&result);
    freeMatrix(&a);
    freeVector(&ans);
}

void test_mat_addition_matrix(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 3, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {3, 4, 5, 4, 5, 3, 5, 4, 3};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_add(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
    freeVector(&ans);
}

void test_mat_addition_matrix_bad_sizes(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 2, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_add(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = makeMatrix(&b, 3, 2, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_add(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
}

void test_mat_subtraction_double(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_sub(a, 1.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&result);
    freeMatrix(&a);
    freeVector(&ans);
}

void test_mat_subtraction_matrix(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 3, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {-1, 0, 1, 0, 1, -1, 1, 0, -1};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_sub(a, b, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
    freeVector(&ans);
}

void test_mat_subtraction_matrix_bad_sizes(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 2, 2, 2, 2, 2};
    Matrix b = {0};
    status = makeMatrix(&b, 2, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_sub(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = makeMatrix(&b, 3, 2, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_sub(a, b, &result);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);
}

void test_mat_division_double(void)
{
    int status = -1;

    Matrix result = {0};
    status = makeMatrixZeros(&result, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_a[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = mat_div(a, 2.0, &result);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)result.data[idx]);
        }
    }

    freeMatrix(&result);
    freeMatrix(&a);
    freeVector(&ans);
}

void test_mat_clear(void)
{
    int status = -1;

    double init_a[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = clearMatrix(&a);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)a.data[idx]);
        }
    }

    freeMatrix(&a);
}

void test_mat_make_zero(void)
{
    int status = -1;

    Matrix a = {0};
    status = makeMatrixZeros(&a, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)a.data[idx]);
        }
    }

    freeMatrix(&a);
}

void test_mat_copy(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix b = {0};
    status = makeMatrixZeros(&b, 3, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = copyMatrix(a, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int idx = r * 3 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)a.data[idx], (float)b.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
}

void test_mat_copy_wrong(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix b = {0};
    status = makeMatrixZeros(&b, 4, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = copyMatrix(a, &b);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = makeMatrixZeros(&b, 3, 4);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = copyMatrix(a, &b);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);
    freeMatrix(&b);
}

void test_mat_delete_col(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {
        1, 3,
        2, 1,
        3, 1};
    Matrix ans = {0};
    status = makeMatrix(&ans, 3, 2, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteColMatrix(&a, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 2; ++c)
        {
            int idx = r * 2 + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)a.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&ans);
}

void test_mat_delete_col_wrong(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteColMatrix(&a, -1);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteColMatrix(&a, 4);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);

    status = deleteColMatrix(&a, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteColMatrix(NULL, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_mat_delete_row(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {
        1, 2, 3,
        3, 2, 1};
    Matrix ans = {0};
    status = makeMatrix(&ans, 2, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteRowMatrix(&a, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < ans.rows; ++r)
    {
        for (int c = 0; c < ans.cols; ++c)
        {
            int idx = r * ans.cols + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)a.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&ans);
}

void test_mat_delete_row_wrong(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteRowMatrix(&a, -1);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteRowMatrix(&a, 4);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeMatrix(&a);

    status = deleteRowMatrix(&a, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteRowMatrix(NULL, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_mat_multiply_double);
    RUN_TEST(test_mat_multiply_matrix);
    RUN_TEST(test_mat_multiply_matrix_bad_sizes);

    RUN_TEST(test_mat_addition_double);
    RUN_TEST(test_mat_addition_matrix);
    RUN_TEST(test_mat_addition_matrix_bad_sizes);

    RUN_TEST(test_mat_subtraction_double);
    RUN_TEST(test_mat_subtraction_matrix);
    RUN_TEST(test_mat_subtraction_matrix_bad_sizes);

    RUN_TEST(test_mat_division_double);

    RUN_TEST(test_mat_clear);
    RUN_TEST(test_mat_copy);
    RUN_TEST(test_mat_copy_wrong);
    RUN_TEST(test_mat_make_zero);
    RUN_TEST(test_mat_delete_col);
    RUN_TEST(test_mat_delete_col_wrong);
    RUN_TEST(test_mat_delete_row);
    RUN_TEST(test_mat_delete_row_wrong);

    return UNITY_END();
}