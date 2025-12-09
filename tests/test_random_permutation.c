/*
 * file: test_identity.c
 * description: script to test the ability of the identity matric generator function
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

void test_create_perm_wrong(void)
{
    int status = -1;
    int n = 25;
    int *arr = (int *)calloc(n, sizeof(int));

    status = generateRandomPermutation(arr, 0);
    TEST_ASSERT_EQUAL_INT(-1, status);

    free(arr);
}

void test_create_perm(void)
{
    int status = -1;
    int n = 25;
    int *arr = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; ++i)
    {
        arr[i] = i;
    }

    status = generateRandomPermutation(arr, n);
    TEST_ASSERT_EQUAL_INT(0, status);

    free(arr);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_perm_wrong);
    RUN_TEST(test_create_perm);

    return UNITY_END();
}