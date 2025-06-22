/*
 * file: test_activations.c
 * description: script to test the ability of the activation functions
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

void test_sigmoid_neg(void)
{
    double x_in = -10.0;
    double x_out = 0.0;
    int status = sigmoid(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_sigmoid_zero(void)
{
    double x_in = 0.0;
    double x_out = 0.0;
    int status = sigmoid(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5, (float)x_out);
}

void test_sigmoid_pos(void)
{
    double x_in = 10.0;
    double x_out = 0.0;
    int status = sigmoid(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0, (float)x_out);
}

void test_sigmoid_dx_neg(void)
{
    double x_in = -10.0;
    double x_out = 0.0;
    int status = sigmoid_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_sigmoid_dx_zero(void)
{
    double x_in = 0.0;
    double x_out = 0.0;
    int status = sigmoid_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.25, (float)x_out);
}

void test_sigmoid_dx_pos(void)
{
    double x_in = 10.0;
    double x_out = 0.0;
    int status = sigmoid_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_relu_neg(void)
{
    double x_in = -10.0;
    double x_out = 0.0;
    int status = relu(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_relu_zero(void)
{
    double x_in = 0.0;
    double x_out = 0.0;
    int status = relu(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_relu_pos(void)
{
    double x_in = 10.0;
    double x_out = 0.0;
    int status = relu(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 10.0, (float)x_out);
}

void test_relu_dx_neg(void)
{
    double x_in = -10.0;
    double x_out = 0.0;
    int status = relu_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_relu_dx_zero(void)
{
    double x_in = 0.0;
    double x_out = 0.0;
    int status = relu_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_relu_dx_pos(void)
{
    double x_in = 10.0;
    double x_out = 0.0;
    int status = relu_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0, (float)x_out);
}

void test_tanh_neg(void)
{
    double x = tanh(-10.0);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -1.0, (float)x);
}

void test_tanh_zero(void)
{
    double x = tanh(0.0);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x);
}

void test_tanh_pos(void)
{
    double x = tanh(10.0);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0, (float)x);
}

void test_tanh_dx_neg(void)
{
    double x_in = -10.0;
    double x_out = 0.0;
    int status = tanh_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_tanh_dx_zero(void)
{
    double x_in = 0.0;
    double x_out = 0.0;
    int status = tanh_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0, (float)x_out);
}

void test_tanh_dx_pos(void)
{
    double x_in = 10.0;
    double x_out = 0.0;
    int status = tanh_dx(&x_in, &x_out);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0, (float)x_out);
}

void test_apply_to_vector_sigmoid(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0};
    Vector a;
    status = makeVector(&a, LEN(init), &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.5, 1.0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToVector(&a, sigmoid);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_vector_sigmoid_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0};
    Vector a;
    status = makeVector(&a, LEN(init), init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.25, 0.0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToVector(&a, sigmoid_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_vector_relu(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0};
    Vector a;
    status = makeVector(&a, LEN(init), init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.0, 10.0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToVector(&a, relu);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_vector_relu_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0};
    Vector a;
    status = makeVector(&a, LEN(init), init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.0, 1.0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToVector(&a, relu_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

// void test_apply_to_vector_tanh(void)
// {
//     int status = 0;

//     double init[] = {-10.0, 0.0, 10.0};
//     Vector a;
//     status = makeVector(&a, LEN(init), init, TYPE_DOUBLE);
//     TEST_ASSERT_EQUAL_INT(0, status);

//     double init_ans[] = {-1.0, 0.0, 1.0};
//     Vector ans;
//     status = makeVector(&ans, LEN(init_ans), init_ans, TYPE_DOUBLE);
//     TEST_ASSERT_EQUAL_INT(0, status);

//     applyToVector(&a, tanh);

//     for (int i = 0; i < LEN(init); ++i)
//     {
//         TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((float *)ans.data)[i], ((float *)a.data)[i]);
//     }
// }

void test_apply_to_vector_tanh_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0};
    Vector a;
    status = makeVector(&a, LEN(init), init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 1.0, 0.0};
    Vector ans;
    status = makeVector(&ans, LEN(init_ans), init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToVector(&a, tanh_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_matrix_sigmoid(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
    Matrix a;
    status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToMatrix(&a, sigmoid);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_matrix_sigmoid_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
    Matrix a;
    status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.25, 0.0, 0.0, 0.25, 0.0, 0.0, 0.25, 0.0};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToMatrix(&a, sigmoid_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_matrix_relu(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
    Matrix a;
    status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.0, 10.0, 0.0, 0.0, 10.0, 0.0, 0.0, 10.0};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToMatrix(&a, relu);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

void test_apply_to_matrix_relu_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
    Matrix a;
    status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToMatrix(&a, relu_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

// void test_apply_to_matrix_tanh(void)
// {
//     int status = 0;

//     double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
//     Matrix a;
//     status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
//     TEST_ASSERT_EQUAL_INT(0, status);

//     double init_ans[] = {-1.0, 0.0, 1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0};
//     Matrix ans;
//     status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
//     TEST_ASSERT_EQUAL_INT(0, status);

//     applyToMatrix(&a, tanh);

//     for (int i = 0; i < LEN(init); ++i)
//     {
//         TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((float *)ans.data)[i], ((float *)a.data)[i]);
//     }
// }

void test_apply_to_matrix_tanh_dx(void)
{
    int status = 0;

    double init[] = {-10.0, 0.0, 10.0, -10.0, 0.0, 10.0, -10.0, 0.0, 10.0};
    Matrix a;
    status = makeMatrix(&a, 3, 3, &init, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    double init_ans[] = {0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0};
    Matrix ans;
    status = makeMatrix(&ans, 3, 3, &init_ans, TYPE_DOUBLE);
    TEST_ASSERT_EQUAL_INT(0, status);

    applyToMatrix(&a, tanh_dx);

    for (int i = 0; i < LEN(init); ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, ((double *)ans.data)[i], ((double *)a.data)[i]);
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_sigmoid_neg);
    RUN_TEST(test_sigmoid_zero);
    RUN_TEST(test_sigmoid_pos);
    RUN_TEST(test_sigmoid_dx_neg);
    RUN_TEST(test_sigmoid_dx_zero);
    RUN_TEST(test_sigmoid_dx_pos);

    RUN_TEST(test_relu_neg);
    RUN_TEST(test_relu_zero);
    RUN_TEST(test_relu_pos);
    RUN_TEST(test_relu_dx_neg);
    RUN_TEST(test_relu_dx_zero);
    RUN_TEST(test_relu_dx_pos);

    RUN_TEST(test_tanh_neg);
    RUN_TEST(test_tanh_zero);
    RUN_TEST(test_tanh_pos);
    RUN_TEST(test_tanh_dx_neg);
    RUN_TEST(test_tanh_dx_zero);
    RUN_TEST(test_tanh_dx_pos);

    RUN_TEST(test_apply_to_vector_sigmoid);
    RUN_TEST(test_apply_to_vector_sigmoid_dx);
    RUN_TEST(test_apply_to_vector_relu);
    RUN_TEST(test_apply_to_vector_relu_dx);
    // RUN_TEST(test_apply_to_vector_tanh);
    RUN_TEST(test_apply_to_vector_tanh_dx);

    RUN_TEST(test_apply_to_matrix_sigmoid);
    RUN_TEST(test_apply_to_matrix_sigmoid_dx);
    RUN_TEST(test_apply_to_matrix_relu);
    RUN_TEST(test_apply_to_matrix_relu_dx);
    // RUN_TEST(test_apply_to_matrix_tanh);
    RUN_TEST(test_apply_to_matrix_tanh_dx);

    return UNITY_END();
}