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

    status = initLogger(TRACE, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_debug(void)
{
    int status = -1;

    status = initLogger(DEBUG, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_info(void)
{
    int status = -1;

    status = initLogger(INFO, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_warn(void)
{
    int status = -1;

    status = initLogger(WARN, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_error(void)
{
    int status = -1;

    status = initLogger(ERROR, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_fatal(void)
{
    int status = -1;

    status = initLogger(FATAL, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_init_log_wrong_level(void)
{
    int status = -1;

    status = initLogger(8, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = -1;
    status = initLogger(-1, TEST_FILENAME, true, true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT(-1, status);

    return;
}

void test_init_log_wrong_filename(void)
{
    int status = -1;

    status = initLogger(TRACE, "", true, true, true, true, true, false);
    TEST_ASSERT_EQUAL_INT(-1, status);

    status = -1;
    status = initLogger(TRACE, NULL, true, true, true, true, true, false);
    TEST_ASSERT_EQUAL_INT(-1, status);

    return;
}

void test_get_curr_time(void)
{
    int status = -1;

    char temp_timestamp[LOG_DATE_TIME_SIZE];
    status = getCurrentTime(true, true, temp_timestamp);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = getCurrentTime(true, false, temp_timestamp);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = getCurrentTime(false, true, temp_timestamp);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = getCurrentTime(false, false, temp_timestamp);
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_get_level(void)
{
    const char *test_test = getLevelString(TEST);
    TEST_ASSERT_EQUAL_STRING("TEST", test_test);

    const char *test_trace = getLevelString(TRACE);
    TEST_ASSERT_EQUAL_STRING("TRACE", test_trace);

    const char *test_debug = getLevelString(DEBUG);
    TEST_ASSERT_EQUAL_STRING("DEBUG", test_debug);

    const char *test_info = getLevelString(INFO);
    TEST_ASSERT_EQUAL_STRING("INFO", test_info);

    const char *test_warn = getLevelString(WARN);
    TEST_ASSERT_EQUAL_STRING("WARN", test_warn);

    const char *test_error = getLevelString(ERROR);
    TEST_ASSERT_EQUAL_STRING("ERROR", test_error);

    const char *test_fatal = getLevelString(FATAL);
    TEST_ASSERT_EQUAL_STRING("FATAL", test_fatal);

    return;
}

void test_get_level_wrong(void)
{
    // Test outside of bounds
    const char *test_less = getLevelString(-1);
    TEST_ASSERT_EQUAL_STRING("", test_less);

    const char *test_greater = getLevelString(10);
    TEST_ASSERT_EQUAL_STRING("", test_greater);

    return;
}

void test_get_color_code(void)
{
    const char *test_test = getColorCode(TEST);
    TEST_ASSERT_EQUAL_STRING(RESET_COLOR, test_test);

    const char *test_trace = getColorCode(TRACE);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;37m", test_trace);

    const char *test_debug = getColorCode(DEBUG);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;36m", test_debug);

    const char *test_info = getColorCode(INFO);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;32m", test_info);

    const char *test_warn = getColorCode(WARN);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;33m", test_warn);

    const char *test_error = getColorCode(ERROR);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;31m", test_error);

    const char *test_fatal = getColorCode(FATAL);
    TEST_ASSERT_EQUAL_STRING("\x1b[1;35m", test_fatal);

    return;
}

void test_get_color_code_wrong(void)
{
    // Test outside of bounds

    const char *test_less = getColorCode(-1);
    TEST_ASSERT_EQUAL_STRING("", test_less);

    const char *test_greater = getColorCode(10);
    TEST_ASSERT_EQUAL_STRING("", test_greater);

    return;
}

void test_count_digits_int(void)
{
    int count = 0;

    count = countDigits(123456, "%.0lf");
    TEST_ASSERT_EQUAL_INT(6, count);

    count = 0;
    count = countDigits(-123456, "%.0lf");
    TEST_ASSERT_EQUAL_INT(7, count);

    count = 0;
    count = countDigits(0, "%.0lf");
    TEST_ASSERT_EQUAL_INT(1, count);

    return;
}

void test_count_digits_double(void)
{
    int count = 0;

    count = countDigits(123.456, "%3.3lf");
    TEST_ASSERT_EQUAL_INT(7, count);

    count = 0;
    count = countDigits(12.34, "%3.1lf");
    TEST_ASSERT_EQUAL_INT(4, count);

    count = 0;
    count = countDigits(12.34, "%3.0lf");
    TEST_ASSERT_EQUAL_INT(3, count);

    count = countDigits(-123.456, "%3.3lf");
    TEST_ASSERT_EQUAL_INT(8, count);

    count = 0;
    count = countDigits(-12.34, "%3.1lf");
    TEST_ASSERT_EQUAL_INT(5, count);

    count = 0;
    count = countDigits(-12.34, "%3.0lf");
    TEST_ASSERT_EQUAL_INT(3, count);

    count = 0;
    count = countDigits(0, "%3.3lf");
    TEST_ASSERT_EQUAL_INT(5, count);

    return;
}

void test_log_to_console_all(void)
{
    int status = -1;
    // char *messages[] = {"TEST LOG TRACE", "TEST LOG DEBUG", "TEST LOG INFO", "TEST LOG WARN", "TEST LOG ERROR", "TEST LOG FATAL"};

    status = initLogger(TEST, NULL, true, true, true, true, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    // // Read log file and compare answers
    // char **file_lines = readLogFile(GLOBAL_LOGGING.log_filepath);

    // if (file_lines != NULL)
    // {

    //     for (int i = 0; i < 6; i++)
    //     {
    //         printf("Line %d: %s\n", i + 1, file_lines[i]);
    //         free(file_lines[i]); // Free individual line memory
    //     }
    //     free(file_lines); // Free the array of pointers
    // }

    return;
}

void test_log_to_console_file(void)
{
    int status = -1;

    status = initLogger(TEST, NULL, true, false, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_console_line(void)
{
    int status = -1;

    status = initLogger(TEST, NULL, false, true, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_console_date(void)
{
    int status = -1;

    status = initLogger(TEST, NULL, false, false, true, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_console_time(void)
{
    int status = -1;

    status = initLogger(TEST, NULL, false, false, false, true, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_console_none(void)
{
    int status = -1;

    status = initLogger(TEST, NULL, false, false, false, false, false, true);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_all(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, true, true, true, true, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_file(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, true, false, false, false, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_line(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, false, true, false, false, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_date(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, false, false, true, false, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_time(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, false, false, false, true, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    return;
}

void test_log_to_file_none(void)
{
    int status = -1;

    status = initLogger(TEST, TEST_FILENAME, false, false, false, false, true, false);
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_TRACE("TEST LOG TRACE\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_DEBUG("TEST LOG DEBUG\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_INFO("TEST LOG INFO\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_WARN("TEST LOG WARN\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_ERROR("TEST LOG ERROR\n");
    TEST_ASSERT_EQUAL_INT(0, status);

    status = -1;
    status = LOG_FATAL("TEST LOG FATAL\n");
    TEST_ASSERT_EQUAL_INT(0, status);

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

    // Test getting time
    RUN_TEST(test_get_curr_time);

    // Test getting level
    RUN_TEST(test_get_level);
    RUN_TEST(test_get_level_wrong);

    // Test getting color code
    RUN_TEST(test_get_color_code);
    RUN_TEST(test_get_color_code_wrong);

    // Test count digits
    RUN_TEST(test_count_digits_int);
    RUN_TEST(test_count_digits_double);

    // Test message logging to console and not file
    RUN_TEST(test_log_to_console_all);
    RUN_TEST(test_log_to_console_file);
    RUN_TEST(test_log_to_console_line);
    RUN_TEST(test_log_to_console_date);
    RUN_TEST(test_log_to_console_time);
    RUN_TEST(test_log_to_console_none);

    // Test message logging to console and not file
    RUN_TEST(test_log_to_file_all);
    RUN_TEST(test_log_to_file_file);
    RUN_TEST(test_log_to_file_line);
    RUN_TEST(test_log_to_file_date);
    RUN_TEST(test_log_to_file_time);
    RUN_TEST(test_log_to_file_none);

    return UNITY_END();
}