/*
 * file: logging.c
 * description: file for all the logging functions
 * author: Ryan Wagner
 * date: October 31, 2025
 * notes:
 */

#include "../header/logging.h"

/**
 * @brief Basic printing of a Matrix object
 *
 * @param include_date Boolean if date is wanted in string
 * @param include_time Boolean if time is wanted in string
 * @param buffer Char * buffer to hold string of date/time
 *
 * @return 0 if successful, -1 if failure
 */
int getCurrentTime(bool include_date, bool include_time, char *buffer)
{
    // Source: https://stackoverflow.com/questions/25030055/add-date-and-time-to-a-file-name-in-c

    time_t now = time(NULL);
    struct tm *timenow = gmtime(&now);

    if (include_date && !include_time)
    {
        strftime(buffer, LOG_DATE_TIME_SIZE * sizeof(char), "%Y-%m-%d", timenow);
    }
    else if (!include_date && include_time)
    {
        strftime(buffer, LOG_DATE_TIME_SIZE * sizeof(char), "%H:%M:%S", timenow);
    }
    else if (include_date && include_time)
    {
        strftime(buffer, LOG_DATE_TIME_SIZE * sizeof(char), "%Y-%m-%d  %H:%M:%S", timenow);
    }
    else
    {
        buffer[0] = '\0';
    }

    return 0;
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param level Level enum of LogLevel for logging
 *
 * @return Name of logging level
 */
const char *getLevelString(int level)
{
    switch (level)
    {
    case 0:
    {
        return "TRACE";
    }
    case 1:
    {
        return "DEBUG";
    }
    case 2:
    {
        return "INFO";
    }
    case 3:
    {
        return "WARN";
    }
    case 4:
    {
        return "ERROR";
    }
    case 5:
    {
        return "FATAL";
    }
    default:
    {
        printf("Level given to getLevelString was not valid.\n");
        return "INVALID";
    }
    }
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param level Level enum of LogLevel for logging
 *
 * @return Color format with respect to logging level
 */
const char *getColorCode(int level)
{
    switch (level)
    {
    case 0:
    {
        return "\x1b[1;37m"; // White
    }
    case 1:
    {
        return "\x1b[1;36m"; // Cyan
    }
    case 2:
    {
        return "\x1b[1;32m"; // Green
    }
    case 3:
    {
        return "\x1b[1;33m"; // Yellow
    }
    case 4:
    {
        return "\x1b[1;31m"; // Red
    }
    case 5:
    {
        return "\x1b[1;35m"; // Magenta
    }
    default:
    {
        printf("Level given to getLevelString was not valid.\n");
        return "";
    }
    }
}

/**
 * @brief Initialize logger object
 *
 * @param config LogConfig object pre-filled
 *
 * @return 0 if successful, -1 if failure
 */
int initLogger(LogConfig *config, LogLevel min_level, const char *filename, bool include_file_info, bool include_line_info, bool include_date, bool include_time, bool log_to_file, bool log_to_console)
{
    // Set all options
    config->include_file_info = include_file_info;
    config->include_line_info = include_line_info;
    config->include_date = include_date;
    config->include_time = include_time;
    config->log_to_console = log_to_console;
    config->log_to_file = log_to_file;

    // Check if input config options are valid
    if (min_level < TRACE || min_level > FATAL)
    {
        printf("Minimum level of logging configuration is invalid.\n");
        return -1;
    }
    config->min_level = min_level;

    // If the user wants to log to a file
    if (log_to_file)
    {
        // If the filename is not valid
        if (filename == NULL || strlen(filename) < 1)
        {
            printf("Minimum filename length of 1 was not met.\n");
            return -1;
        }

        // Build absolute path to log file
        size_t relative_path_size = strlen(PATH_TO_LOGS);
        size_t filename_size = strlen(filename);

        // Copy absolute_path into log_filepath
        config->log_filepath = (char *)malloc(relative_path_size + filename_size + 1);
        strcpy(config->log_filepath, PATH_TO_LOGS);
        strcat(config->log_filepath, filename);
        // Copy filename into log_filename
        config->log_filename = (char *)malloc(filename_size + 1);
        strcpy(config->log_filename, filename);

        FILE *fptr;

        fptr = fopen(config->log_filepath, "w");

        if (fptr == NULL)
        {
            printf("Error: Logging file could not be opened.\n");
            return -1;
        }

        fclose(fptr);
    }
    else
    {
        config->log_filename = NULL;
        config->log_filepath = NULL;
    }

    return 0;
}

/**
 * @brief Calculate the number of digits in integer
 *
 * @param n Integer value to count digits
 *
 * @return Number of digits as integer
 */
int countDigits(int n)
{
    // Sufficiently large buffer for 2^64 unsigned int value
    char buffer[20];
    return snprintf(buffer, sizeof(buffer), "%d", n);
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param timestamp Timestamp string
 * @param level Level enum of LogLevel
 * @param file Filename of file
 * @param line Line of info, debug, error, etc.
 * @param color Color associated to level of message
 * @param message Message to print
 *
 * @return Color format with respect to logging level
 */
char *build_formatted_message(LogConfig config, const char *timestamp, const char *level, const char *file, int line, const char *color, const char *message)
{
    // Calculate length of prefix strings with formatting, including colon, dash, and spaces
    int msg_length = strlen(timestamp) +
                     strlen(color) +
                     strlen(level) +
                     strlen(RESET_COLOR) +
                     strlen(message);

    // Format: timestamp (file, line) - color level reset: message
    if (config.include_file_info && !config.include_line_info)
    {
        msg_length += strlen(file) + 10;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            printf("Allocation of prefix to log was unsuccessful.\n");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s) - %s%s%s: %s\n", timestamp, file, color, level, RESET_COLOR, message);
        return msg;
    }
    else if (config.include_line_info && !config.include_file_info)
    {
        msg_length += countDigits(line) + 10;
        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            printf("Allocation of prefix to log was unsuccessful.\n");
            return "";
        }

        snprintf(msg, msg_length, "%s (%d) - %s%s%s: %s\n", timestamp, line, color, level, RESET_COLOR, message);
        return msg;
    }
    else if (config.include_file_info && config.include_line_info)
    {
        msg_length += strlen(file) + countDigits(line) + 12;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            printf("Allocation of prefix to log was unsuccessful.\n");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s, %d) - %s%s%s: %s\n", timestamp, file, line, color, level, RESET_COLOR, message);
        return msg;
    }
    else
    {
        if (strlen(timestamp) == 0)
        {
            msg_length += 4;
            char *msg = (char *)malloc(msg_length + 1);

            if (msg == NULL)
            {
                printf("Allocation of prefix to log was unsuccessful.\n");
                return "";
            }

            snprintf(msg, msg_length, "%s%s%s: %s\n", color, level, RESET_COLOR, message);
            return msg;
        }
        else
        {
            msg_length += 7;
            char *msg = (char *)malloc(msg_length + 1);

            if (msg == NULL)
            {
                printf("Allocation of prefix to log was unsuccessful.\n");
                return "";
            }

            snprintf(msg, msg_length, "%s - %s%s%s: %s\n", timestamp, color, level, RESET_COLOR, message);
            return msg;
        }
    }
}

/**
 * @brief Basic printing of a Matrix object
 *
 * @param config LogConfig object that holds logging settings
 * @param level Level enum of LogLevel
 * @param file Filename of file
 * @param line Line of info, debug, error, etc.
 * @param format Format of message to print/log
 * @param ... Extra arguements
 *
 * @return Color format with respect to logging level
 */
int log_message(LogConfig config, int level, const char *file, int line, const char *format, ...)
{
    if (level < config.min_level)
    {
        return 0;
    }

    // Build the log message
    char temp_timestamp[LOG_DATE_TIME_SIZE];
    if (getCurrentTime(config.include_date, config.include_time, temp_timestamp) < 0)
    {
        printf("Getting timestamp for logging was unsuccessful.\n");
        return -1;
    }
    const char *timestamp_str = temp_timestamp;
    const char *level_str = getLevelString(level);
    const char *color_code_str = getColorCode(level);

    // Calculate the length of the format and args string
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (len < 0)
    {
        printf("Error determining formatted string length.\n");
        return -1;
    }

    // Make the formatted string with args
    char *formatted_message = (char *)malloc(len + 1);
    if (formatted_message == NULL)
    {
        printf("Memory allocation for formatted logging message failed.\n");
        return -1;
    }

    va_start(args, format);
    vsnprintf(formatted_message, len + 1, format, args);
    va_end(args);

    char *built_message = build_formatted_message(config, timestamp_str, level_str, file, line, color_code_str, formatted_message);
    free(formatted_message);

    if (config.log_to_console)
    {
        printf("%s", built_message);
    }
    if (config.log_to_file)
    {
        FILE *fptr;

        fptr = fopen(config.log_filepath, "a");

        if (fptr == NULL)
        {
            printf("Logging file could not be opened.\n");
            return -1;
        }
        else
        {
            fprintf(fptr, "%s", built_message);
        }

        fclose(fptr);
    }

    free(built_message);

    return 0;
}

/**
 * @brief Free Logging object and set poitner to NULL
 *
 * @param config LogConfig variable to free
 *
 * @return None
 */
void freeLogger(LogConfig *config)
{
    if (config && config->log_filepath != NULL)
    {
        free(config->log_filepath);
        config->log_filepath = NULL;
    }
    if (config && config->log_filename != NULL)
    {
        free(config->log_filename);
        config->log_filename = NULL;
    }

    return;
}
