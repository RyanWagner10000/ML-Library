/*
 * file: test_identity.c
 * description: script to test the ability of the identity matric generator function
 * author: Ryan Wagner
 * date: June 6, 2025
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

void test_init_log_trace(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_debug(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, DEBUG, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_info(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, INFO, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_warn(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, WARN, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_error(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, ERROR, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_fatal(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, FATAL, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);
    freeLogger(&test_logger);

    return;
}

void test_init_log_wrong_level(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, 8, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(-1, status);
    
    status = initLogger(&test_logger, -1, "test_log_file.txt", true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(-1, status);

    return;
}

void test_init_log_wrong_filename(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, "", true, true, true, true, true, false);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = initLogger(&test_logger, TRACE, NULL, true, true, true, true, true, false);
    TEST_ASSERT_EQUAL_INT(-1, status);

    return;
}

void test_log_to_console_all(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, true, true, true, true, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    // TEST_ASSERT_EQUAL_STRING();

    freeLogger(&test_logger);

    return;
}

void test_log_to_console_file(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, true, false, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    freeLogger(&test_logger);

    return;
}

void test_log_to_console_line(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, false, true, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    freeLogger(&test_logger);

    return;
}

void test_log_to_console_date(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, false, false, true, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    freeLogger(&test_logger);

    return;
}

void test_log_to_console_time(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, false, false, false, true, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    freeLogger(&test_logger);

    return;
}

void test_log_to_console_none(void)
{
    int status = -1;

    LogConfig test_logger;

    status = initLogger(&test_logger, TRACE, NULL, false, false, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = LOG_TRACE(test_logger, "TEST LOG TRACE");
    TEST_ASSERT_EQUAL_INT(0, status);

    freeLogger(&test_logger);

    return;
}

int main(void)
{
    UNITY_BEGIN();

    // Test making a logging object
    RUN_TEST(test_init_log_trace);
    RUN_TEST(test_init_log_debug);
    RUN_TEST(test_init_log_info);
    RUN_TEST(test_init_log_warn);
    RUN_TEST(test_init_log_error);
    RUN_TEST(test_init_log_fatal);

    // Test incorrect making logging object
    RUN_TEST(test_init_log_wrong_level);
    RUN_TEST(test_init_log_wrong_filename);

    // Test message logging to console and not file
    RUN_TEST(test_log_to_console_all);
    RUN_TEST(test_log_to_console_file);
    RUN_TEST(test_log_to_console_line);
    RUN_TEST(test_log_to_console_date);
    RUN_TEST(test_log_to_console_time);
    RUN_TEST(test_log_to_console_none);

    return UNITY_END();
}