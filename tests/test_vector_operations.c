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

void test_vect_clear(void)
{
    int status = -1;

    double init_a[] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = clearVector(&a);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < a.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)a.data[r]);
    }

    freeVector(&a);
}

void test_vect_make_zero(void)
{
    int status = -1;

    Vector a = {0};
    status = makeVectorZeros(&a, 9);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < a.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)a.data[r]);
    }

    freeVector(&a);
}

void test_vect_copy(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector b = {0};
    status = makeVectorZeros(&b, a.size);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = copyVector(a, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < a.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)a.data[r], (float)b.data[r]);
    }

    freeVector(&a);
    freeVector(&b);
}

void test_vect_copy_wrong(void)
{
    int status = -1;

    double init_a[] = {1, 2, 3, 2, 3, 1, 3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector b = {0};
    status = makeVectorZeros(&b, a.size - 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = copyVector(a, &b);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeVector(&a);
    freeVector(&b);
}

void test_vect_delete_elem(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {
        1, 2, 3,
        2, 1,
        3, 2, 1};
    Vector ans = {0};
    status = makeVector(&ans, a.size - 1, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteElemVector(&a, 4);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < a.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)a.data[r]);
    }

    freeVector(&a);
    freeVector(&ans);
}

void test_vect_delete_elem_wrong(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Vector a = {0};
    status = makeVector(&a, 9, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = deleteElemVector(&a, -1);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteElemVector(&a, 15);
    TEST_ASSERT_EQUAL_INT(-1, status);

    freeVector(&a);

    status = deleteElemVector(&a, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = deleteElemVector(NULL, 2);
    TEST_ASSERT_EQUAL_INT(-1, status);
}

void test_mat_get_col_v(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector b = {0};
    status = makeVectorZeros(&b, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 3, 2};
    Vector ans = {0};
    status = makeVector(&ans, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = getColMatrix_v(a, 1, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < ans.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)b.data[r]);
    }

    freeMatrix(&a);
    freeVector(&b);
    freeVector(&ans);
}

void test_mat_get_col_m(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix b = {0};
    status = makeMatrixZeros(&b, 3, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 3, 2};
    Matrix ans = {0};
    status = makeMatrix(&ans, 3, 1, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = getColMatrix_m(a, 1, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < ans.rows; ++r)
    {
        for (int c = 0; c < ans.cols; ++c)
        {
            int idx = r * ans.cols + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)b.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&ans);
}

void test_mat_get_row_v(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector b = {0};
    status = makeVectorZeros(&b, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 3, 1};
    Vector ans = {0};
    status = makeVector(&ans, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = getRowMatrix_v(a, 1, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < ans.size; ++r)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[r], (float)b.data[r]);
    }

    freeMatrix(&a);
    freeVector(&b);
    freeVector(&ans);
}

void test_mat_get_row_m(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    Matrix b = {0};
    status = makeMatrixZeros(&b, 1, 3);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {2, 3, 1};
    Matrix ans = {0};
    status = makeMatrix(&ans, 1, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = getRowMatrix_m(a, 1, &b);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int r = 0; r < ans.rows; ++r)
    {
        for (int c = 0; c < ans.cols; ++c)
        {
            int idx = r * ans.cols + c;
            TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)ans.data[idx], (float)b.data[idx]);
        }
    }

    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&ans);
}

void test_mat_set_col(void)
{
    int status = -1;

    double init_a[] = {
        1, 0, 3,
        2, 0, 1,
        3, 0, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 3, 2};
    Vector b = {0};
    status = makeVector(&b, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix ans = {0};
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = setColMatrix(&a, 1, b);
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
    freeVector(&b);
    freeMatrix(&ans);
}

void test_mat_set_row(void)
{
    int status = -1;

    double init_a[] = {
        1, 2, 3,
        0, 0, 0,
        3, 2, 1};
    Matrix a = {0};
    status = makeMatrix(&a, 3, 3, &init_a, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_b[] = {2, 3, 1};
    Vector b = {0};
    status = makeVector(&b, 3, &init_b, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {
        1, 2, 3,
        2, 3, 1,
        3, 2, 1};
    Matrix ans = {0};
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = setRowMatrix(&a, 1, b);
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
    freeVector(&b);
    freeMatrix(&ans);
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

    RUN_TEST(test_vect_clear);
    RUN_TEST(test_vect_copy);
    RUN_TEST(test_vect_copy_wrong);
    RUN_TEST(test_vect_make_zero);
    RUN_TEST(test_vect_delete_elem);
    RUN_TEST(test_vect_delete_elem_wrong);

    RUN_TEST(test_mat_get_col_v);
    RUN_TEST(test_mat_get_col_m);
    RUN_TEST(test_mat_get_row_v);
    RUN_TEST(test_mat_get_row_m);
    RUN_TEST(test_mat_set_col);
    RUN_TEST(test_mat_set_row);

    return UNITY_END();
}