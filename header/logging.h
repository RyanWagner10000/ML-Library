/*
 * file: logging.h
 * description: header file that gives access to all the logging funcitons
 * author: Ryan Wagner
 * date: October 31, 2025
 * notes:
 */

#ifndef LOGGING_H
#define LOGGING_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

// Convenience macros for easier use

// Source: https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_TRACE(fmt, ...) log_message(TRACE, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_message(DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_message(INFO, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) log_message(WARN, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_message(ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) log_message(FATAL, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DATE_TIME_SIZE 64
#define RESET_COLOR "\x1b[0m"
#define PATH_MAX 4096
#define PATH_TO_LOGS "/home/ryan/ML-Library/logs/"
#define TEST_FILENAME "test_log_file.txt"

typedef enum
{
    TEST,  // Just for testing, so no logging
    TRACE, // Very detailed, for deep debugging
    DEBUG, // Debug information
    INFO,  // General information
    WARN,  // Warnings
    ERROR, // Errors
    FATAL  // Fatal errors
} LogLevel;

typedef struct
{
    LogLevel min_level;     // Minimum level to log
    bool log_to_file;       // Whether to log to file
    bool log_to_console;    // Whether to log to console
    char *log_filename;     // Filename if logging to file
    char *log_filepath;     // Filepath if logging to file
    bool use_colors;        // Use ANSI colors in console
    bool include_file_info; // Include file information
    bool include_line_info; // Include line information
    bool include_date;      // Include date when logging
    bool include_time;      // Include time when logging
} LogConfig;

extern LogConfig GLOBAL_LOGGING;

int getCurrentTime(bool include_date, bool include_time, char *buffer);
const char *getLevelString(int level);
const char *getColorCode(int level);
int countDigits(double n, const char* fmt);

int initLogger(LogLevel min_level, const char *filename, bool include_file_info, bool include_line_info, bool include_date, bool include_time, bool log_to_file, bool log_to_console);

char *buildConsoleFormattedMessage(const char *timestamp, const char *level, const char *file, int line, const char *color, const char *message);
char *buildFileFormattedMessage(const char *timestamp, const char *level, const char *file, int line, const char *message);

int log_message(int level, const char *file, int line, const char *format, ...);

void freeLogger();

char **readLogFile(const char *filepath);

#endif // LOGGING_H