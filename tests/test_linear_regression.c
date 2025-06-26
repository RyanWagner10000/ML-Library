/*
 * file: test_linear_regression.c
 * description: script to test the capability of the linear regression function
 * author: Ryan Wagner
 * date: June 23, 2025
 * notes:
 */

#include "unity.h"
#include "../header/math_funcs.h"
#include "../header/linear_regression.h"

void setUp(void)
{
    // Optional: initialize stuff before each test
}

void tearDown(void)
{
    // Optional: clean up after each test
}

void test_linear_regression_no_reg(void)
{
    int status = 0;

    double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    status = makeMatrix(&X, 10, 1, &x_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y;
    status = makeVector(&y, X.rows, &y_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector w;
    status = makeVectorZeros(&w, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 1000,
        .lambda = 0.01,
        .regularization = REG_NONE};

    status = train_linear_model(&X, &y, &w, &b, &config);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y_new;
    status = makeVectorZeros(&y_new, y.size);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(&X, &w, &y_new);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += b;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.1f, ((double *)y.data)[i], ((double *)y_new.data)[i]);
    }

    freeMatrix(&X);
    freeVector(&y);
    freeVector(&w);
    freeVector(&y_new);

    return;
}

void test_linear_regression_L1_reg(void)
{
    int status = 0;

    double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    status = makeMatrix(&X, 10, 1, &x_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y;
    status = makeVector(&y, X.rows, &y_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector w;
    status = makeVectorZeros(&w, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 1000,
        .lambda = 0.01,
        .regularization = REG_L1};

    status = train_linear_model(&X, &y, &w, &b, &config);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y_new;
    status = makeVectorZeros(&y_new, y.size);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(&X, &w, &y_new);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += b;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.1f, ((double *)y.data)[i], ((double *)y_new.data)[i]);
    }

    freeMatrix(&X);
    freeVector(&y);
    freeVector(&w);
    freeVector(&y_new);
}

void test_linear_regression_L2_reg(void)
{
    int status = 0;

    double x_vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double y_vals[] = {2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0, 29.0};

    DataType type = TYPE_DOUBLE;

    // 4 examples, 1 feature
    Matrix X;
    status = makeMatrix(&X, 10, 1, &x_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y;
    status = makeVector(&y, X.rows, &y_vals, type);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector w;
    status = makeVectorZeros(&w, 1);
    TEST_ASSERT_EQUAL_INT(0, status);

    double b = 0.0;

    TrainConfig config = {
        .learning_rate = 0.01,
        .epochs = 1000,
        .lambda = 0.01,
        .regularization = REG_L2};

    status = train_linear_model(&X, &y, &w, &b, &config);
    TEST_ASSERT_EQUAL_INT(0, status);

    Vector y_new;
    status = makeVectorZeros(&y_new, y.size);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = matvec_mult(&X, &w, &y_new);
    TEST_ASSERT_EQUAL_INT(0, status);

    for (int i = 0; i < y_new.size; ++i)
    {
        y_new.data[i] += b;
    }

    for (int i = 0; i < y_new.size; ++i)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.1f, ((double *)y.data)[i], ((double *)y_new.data)[i]);
    }

    freeMatrix(&X);
    freeVector(&y);
    freeVector(&w);
    freeVector(&y_new);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_linear_regression_no_reg);
    RUN_TEST(test_linear_regression_L1_reg);
    RUN_TEST(test_linear_regression_L2_reg);

    return UNITY_END();
}
