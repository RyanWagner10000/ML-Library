/*
 * file: logging.c
 * description: file for all the logging functions
 * author: Ryan Wagner
 * date: October 31, 2025
 * notes:
 */

#include "../header/logging.h"

LogConfig GLOBAL_LOGGING;

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
        return "TEST";
    }
    case 1:
    {
        return "TRACE";
    }
    case 2:
    {
        return "DEBUG";
    }
    case 3:
    {
        return "INFO";
    }
    case 4:
    {
        return "WARN";
    }
    case 5:
    {
        return "ERROR";
    }
    case 6:
    {
        return "FATAL";
    }
    default:
    {
        perror("Level given to getLevelString was not valid.");
        return "";
    }
    }
}

/**
 * @brief Get color code for specific level of logging
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
        return RESET_COLOR;
    }
    case 1:
    {
        return "\x1b[1;37m"; // White
    }
    case 2:
    {
        return "\x1b[1;36m"; // Cyan
    }
    case 3:
    {
        return "\x1b[1;32m"; // Green
    }
    case 4:
    {
        return "\x1b[1;33m"; // Yellow
    }
    case 5:
    {
        return "\x1b[1;31m"; // Red
    }
    case 6:
    {
        return "\x1b[1;35m"; // Magenta
    }
    default:
    {
        perror("Level given to getColorCode was not valid.");
        return "";
    }
    }
}

/**
 * @brief Calculate the number of digits for value
 *
 * @param n Double (or typecasted int) value to count digits
 *
 * @return Number of digits as integer
 */
int countDigits(double n, const char* fmt)
{
    // Source: https://stackoverflow.com/questions/1701055/what-is-the-maximum-length-in-chars-needed-to-represent-any-double-value
    // Sufficiently large buffer for signed 2^64 value
    char buffer[1079];
    return snprintf(buffer, sizeof(buffer), fmt, n);
}

/**
 * @brief Initialize logger object
 *
 * @param config LogConfig object pre-filled
 *
 * @return 0 if successful, -1 if failure
 */
int initLogger(LogLevel min_level, const char *filename, bool include_file_info, bool include_line_info, bool include_date, bool include_time, bool log_to_file, bool log_to_console)
{
    // Set all options
    GLOBAL_LOGGING.include_file_info = include_file_info;
    GLOBAL_LOGGING.include_line_info = include_line_info;
    GLOBAL_LOGGING.include_date = include_date;
    GLOBAL_LOGGING.include_time = include_time;
    GLOBAL_LOGGING.log_to_console = log_to_console;
    GLOBAL_LOGGING.log_to_file = log_to_file;

    // Check if input config options are valid
    if (min_level < TEST || min_level > FATAL)
    {
        perror("Minimum level of logging configuration is invalid.");
        return -1;
    }
    GLOBAL_LOGGING.min_level = min_level;

    char *output_filename;

    if (GLOBAL_LOGGING.min_level == 0)
    {
        GLOBAL_LOGGING.log_to_console = false;
        GLOBAL_LOGGING.log_to_file = true;

        output_filename = TEST_FILENAME;
    }
    else
    {
        // If the filename is not valid
        if (filename == NULL || strlen(filename) < 1)
        {
            perror("Minimum filename length of 1 was not met.");
            return -1;
        }
        output_filename = (char *)filename;
    }

    // If the user wants to log to a file
    if (GLOBAL_LOGGING.log_to_file)
    {

        // Build absolute path to log file
        size_t relative_path_size = strlen(PATH_TO_LOGS);
        size_t filename_size = strlen(output_filename);

        // Copy absolute_path into log_filepath
        GLOBAL_LOGGING.log_filepath = (char *)malloc(relative_path_size + filename_size + 1);
        strcpy(GLOBAL_LOGGING.log_filepath, PATH_TO_LOGS);
        strcat(GLOBAL_LOGGING.log_filepath, output_filename);
        // Copy output_filename into log_filename
        GLOBAL_LOGGING.log_filename = (char *)malloc(filename_size + 1);
        strcpy(GLOBAL_LOGGING.log_filename, output_filename);

        FILE *fptr;

        fptr = fopen(GLOBAL_LOGGING.log_filepath, "w");

        if (fptr == NULL)
        {
            perror("Error: Logging file could not be opened.");
            return -1;
        }

        fclose(fptr);
    }
    else
    {
        freeLogger();
        GLOBAL_LOGGING.log_filename = NULL;
        GLOBAL_LOGGING.log_filepath = NULL;
    }

    return 0;
}

/**
 * @brief Assembling all parts of a log message to be printed to the console
 *
 * @param timestamp Timestamp string
 * @param level Level enum of LogLevel
 * @param file Filename of file
 * @param line Line of info, debug, error, etc.
 * @param color Color associated to level of message
 * @param message Message to print
 *
 * @return Char array of assembled and formatted log message
 */
char *buildConsoleFormattedMessage(const char *timestamp, const char *level, const char *file, int line, const char *color, const char *message)
{
    // Calculate length of prefix strings with formatting, including colon, dash, and spaces
    int msg_length = strlen(timestamp) +
                     strlen(color) +
                     strlen(level) +
                     strlen(RESET_COLOR) +
                     strlen(message);

    // Format: timestamp (file, line) - color level reset: message
    if (GLOBAL_LOGGING.include_file_info && !GLOBAL_LOGGING.include_line_info)
    {
        msg_length += strlen(file) + 10;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s) - %s%s%s: %s", timestamp, file, color, level, RESET_COLOR, message);
        return msg;
    }
    else if (GLOBAL_LOGGING.include_line_info && !GLOBAL_LOGGING.include_file_info)
    {
        msg_length += countDigits((double)line, "%.0lf") + 10;
        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%d) - %s%s%s: %s", timestamp, line, color, level, RESET_COLOR, message);
        return msg;
    }
    else if (GLOBAL_LOGGING.include_file_info && GLOBAL_LOGGING.include_line_info)
    {
        msg_length += strlen(file) + countDigits((double)line, "%.0lf") + 12;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s, %d) - %s%s%s: %s", timestamp, file, line, color, level, RESET_COLOR, message);
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
                perror("Allocation of prefix to log was unsuccessful.");
                return "";
            }

            snprintf(msg, msg_length, "%s%s%s: %s", color, level, RESET_COLOR, message);
            return msg;
        }
        else
        {
            msg_length += 7;
            char *msg = (char *)malloc(msg_length + 1);

            if (msg == NULL)
            {
                perror("Allocation of prefix to log was unsuccessful.");
                return "";
            }

            snprintf(msg, msg_length, "%s - %s%s%s: %s", timestamp, color, level, RESET_COLOR, message);
            return msg;
        }
    }
}

/**
 * @brief Assembling all parts of a log message to be printed to a file without color formatting
 *
 * @param timestamp Timestamp string
 * @param level Level enum of LogLevel
 * @param file Filename of file
 * @param line Line of info, debug, error, etc.
 * @param message Message to print
 *
 * @return Char array of assembled and formatted log message
 */
char *buildFileFormattedMessage(const char *timestamp, const char *level, const char *file, int line, const char *message)
{
    // Calculate length of prefix strings with formatting, including colon, dash, and spaces
    int msg_length = strlen(timestamp) +
                     strlen(level) +
                     strlen(message);

    // Format: timestamp (file, line) - color level reset: message
    if (GLOBAL_LOGGING.include_file_info && !GLOBAL_LOGGING.include_line_info)
    {
        msg_length += strlen(file) + 10;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s) - %s: %s", timestamp, file, level, message);
        return msg;
    }
    else if (GLOBAL_LOGGING.include_line_info && !GLOBAL_LOGGING.include_file_info)
    {
        msg_length += countDigits((double)line, "%.0lf") + 10;
        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%d) - %s: %s", timestamp, line, level, message);
        return msg;
    }
    else if (GLOBAL_LOGGING.include_file_info && GLOBAL_LOGGING.include_line_info)
    {
        msg_length += strlen(file) + countDigits((double)line, "%.0lf") + 12;

        char *msg = (char *)malloc(msg_length + 1);

        if (msg == NULL)
        {
            perror("Allocation of prefix to log was unsuccessful.");
            return "";
        }

        snprintf(msg, msg_length, "%s (%s, %d) - %s: %s", timestamp, file, line, level, message);
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
                perror("Allocation of prefix to log was unsuccessful.");
                return "";
            }

            snprintf(msg, msg_length, "%s: %s", level, message);
            return msg;
        }
        else
        {
            msg_length += 7;
            char *msg = (char *)malloc(msg_length + 1);

            if (msg == NULL)
            {
                perror("Allocation of prefix to log was unsuccessful.");
                return "";
            }

            snprintf(msg, msg_length, "%s - %s: %s", timestamp, level, message);
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
int log_message(int level, const char *file, int line, const char *format, ...)
{
    if (level < GLOBAL_LOGGING.min_level)
    {
        return 0;
    }

    // Build the log message
    char temp_timestamp[LOG_DATE_TIME_SIZE];
    if (getCurrentTime(GLOBAL_LOGGING.include_date, GLOBAL_LOGGING.include_time, temp_timestamp) < 0)
    {
        perror("Getting timestamp for logging was unsuccessful.");
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
        perror("Error determining formatted string length.");
        return -1;
    }

    // Make the formatted string with args
    char *formatted_message = (char *)malloc(len + 1);
    if (formatted_message == NULL)
    {
        perror("Memory allocation for formatted logging message failed.");
        return -1;
    }

    va_start(args, format);
    vsnprintf(formatted_message, len + 1, format, args);
    va_end(args);

    // Log a different message to the console or file if applicable
    // Don't ever print to the terminal if testing
    if (GLOBAL_LOGGING.log_to_console && GLOBAL_LOGGING.min_level > TEST)
    {
        char *built_message = buildConsoleFormattedMessage(timestamp_str, level_str, file, line, color_code_str, formatted_message);
        fprintf(stdout, "%s", built_message);
        free(built_message);
    }
    if (GLOBAL_LOGGING.log_to_file)
    {
        char *built_message = buildFileFormattedMessage(timestamp_str, level_str, file, line, formatted_message);

        FILE *fptr;

        fptr = fopen(GLOBAL_LOGGING.log_filepath, "a");

        if (fptr == NULL)
        {
            perror("Logging file could not be opened.");
            return -1;
        }
        else
        {
            fprintf(fptr, "%s", built_message);
        }

        fclose(fptr);
    }

    free(formatted_message);

    return 0;
}

/**
 * @brief Free Logging object and set poitner to NULL
 *
 * @param config LogConfig variable to free
 *
 * @return None
 */
void freeLogger()
{
    if (GLOBAL_LOGGING.log_filepath != NULL)
    {
        free(GLOBAL_LOGGING.log_filepath);
        GLOBAL_LOGGING.log_filepath = NULL;
    }
    if (GLOBAL_LOGGING.log_filename != NULL)
    {
        free(GLOBAL_LOGGING.log_filename);
        GLOBAL_LOGGING.log_filename = NULL;
    }

    return;
}

/**
 * @brief Read the test_log_file.txt file and relay the contents
 *
 * @param config LogConfig variable to free
 *
 * @return Array of strings (char *) of lines from file
 */
char **readLogFile(const char *filepath)
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        perror("Error opening test_log_file.txt file");
        return NULL;
    }

    char **lines = NULL;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        // Remove trailing newline character if present
        if (read > 0 && line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }

        // Reallocate memory for the array of strings
        lines = (char **)realloc(lines, (count + 1) * sizeof(char *));
        if (lines == NULL)
        {
            perror("Memory reallocation failed");
            // Free previously allocated lines before exiting
            for (int i = 0; i < count; i++)
            {
                free(lines[i]);
            }
            free(line);
            fclose(fp);
            return NULL;
        }

        // Duplicate the line and store it in the array
        lines[count] = strdup(line);
        if (lines[count] == NULL)
        {
            perror("Memory allocation for line failed");
            // Free previously allocated lines and the current line buffer
            for (int i = 0; i < count; i++)
            {
                free(lines[i]);
            }
            free(lines);
            free(line);
            fclose(fp);
            return NULL;
        }
        count++;
    }

    free(line); // Free the buffer used by getline
    fclose(fp);
    return lines;
}