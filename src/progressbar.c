/*
 * file: progressbar.c
 * description: file used define the progress bar functions
 * author: Ryan Wagner
 * date: August 30, 2025
 * notes:
 */

#include "progressbar.h"

/**
 * @brief Initialize a ProgressBar object
 *
 * @param p Pointer to PBD progress bar object
 * @param max_len Maximum character length of the progress bar
 * @param left left character of the progress bar
 * @param right right character of the progress bar
 * @param fill Character that represents completion of the progress bar
 * @param empty Character that represents incompletion of the progress bar
 * @param update_rate Double of how fast to update the progress bar in milliseconds
 *
 * @return void
 */
void initProgressBar(PBD *p, int max_len, char left, char right, char fill, char empty, double update_rate)
{
    // Check input max_len
    if (max_len > MAX_PROGRESS_BAR_LEN)
    {
        LOG_WARN("Input max length of progress bar of %d was considered to be too large. Value must be less than %d.\n", max_len, MAX_PROGRESS_BAR_LEN);
        LOG_WARN("Setting max length of progress bar to %d\n", MAX_PROGRESS_BAR_LEN);
        p->m_max_len = MAX_PROGRESS_BAR_LEN;
    }
    else
    {
        p->m_max_len = max_len;
    }

    // Set starting values
    p->n_curr_len = 0;
    p->progress = 0.0;
    p->loss = 0.0;
    p->last_time.tv_nsec = 0;
    p->last_time.tv_sec = 0;

    // Set formatting characters
    p->left_margin = left;
    p->right_margin = right;
    p->fill_char = fill;
    p->empty_char = empty;

    // Check update rate
    if (update_rate <= 0)
    {
        LOG_WARN("Value input for progress bar update rate was invalid. Setting to default update rate of %.3f ms.\n", DEFAULT_TIME_UPDATE);
        p->ms_update = DEFAULT_TIME_UPDATE;
    }
    else
    {
        p->ms_update = update_rate;
    }
}

/**
 * @brief Create a string (char * array) of repeated characters
 *
 * @param character Character to duplicate in the string
 * @param count Number of replications of the character in the string
 *
 * @return Char array of character
 */
char *createRepeatedCharString(char character, int count)
{
    if (count < 0)
    {
        LOG_WARN("Cannot create a char * array with a length less than 0.\n");
        return "";
    }
    else if (count == 0)
    {
        return "";
    }

    // Allocate memory for the string + null terminator
    char *str = (char *)malloc(count + 1);

    // Check if malloc was successful
    if (str == NULL)
    {
        LOG_ERROR("Memory allocation for repeated string was unsuccessful.\n");
        return "";
    }

    // Fill the allocated memory with the specified character
    memset(str, character, count);

    // Null-terminate the string
    str[count] = '\0';

    return str;
}

/**
 * @brief Build the progress bar string
 *
 * @param p Progress bar object pointer
 *
 * @return Char array of assembled and formatted progerss bar
 */
char *buildProgressBar(PBD *p)
{
    // Ex: [####..............................................] [%8.00] [Loss: 0.36]

    int msg_length = 2 + // Left and right margin
                     p->m_max_len +
                     3 +                                  // % complete with [], no number
                     countDigits(p->progress, "%3.2lf") + // progress number
                     7 +                                  // loss with [], no number
                     countDigits(p->loss, "%.4lf") +      // loss number
                     2 +                                  // spaces
                     3;                                   // carriage return

    // Create filled char array
    char *fill = createRepeatedCharString(p->fill_char, p->n_curr_len);

    // Create filled char array
    const char *empty = createRepeatedCharString(p->empty_char, p->m_max_len - p->n_curr_len);

    char *progress_str = (char *)malloc(msg_length + 1);

    if (progress_str == NULL)
    {
        perror("Allocation of progress bar string was unsuccessful.");
        return "\r";
    }

    snprintf(progress_str, msg_length, "%c%s%s%c [%%%3.2lf] [Loss: %.4lf]\r", p->left_margin, fill, empty, p->right_margin, p->progress, p->loss);

    return progress_str;
}

/**
 * @brief Draw the input progress bar object
 *
 * @param p Pointer to PBD progress bar object
 *
 * @return void
 */
void drawProgressBar(PBD *p)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    long diff_ms = (now.tv_sec - p->last_time.tv_sec) * 1000 + (now.tv_nsec - p->last_time.tv_nsec) / 1000000;

    if (diff_ms < p->ms_update) return;

    p->last_time = now;

    // Make sure that max_len >= CurLen
    if (p->m_max_len < p->n_curr_len)
    {
        p->n_curr_len = p->m_max_len;
    }

    // If sufficient time has passed
    const char *progress_bar = buildProgressBar(p);

    LOG_INFO(progress_bar);

    return;
}