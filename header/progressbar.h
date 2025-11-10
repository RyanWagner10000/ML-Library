/*
 * file: progressbar.h
 * description: header file that gives access to all the progress bar functions
 * author: Ryan Wagner
 * date: Augest 30, 2025
 * notes:
 * src: https://stackoverflow.com/questions/60949001/progress-bar-in-c-for-an-arbitrary-long-execution-console
 */

#include <logging.h>

#define MAX_PROGRESS_BAR_LEN 300
#define DEFAULT_TIME_UPDATE 0.100

struct tagProgressBarData
{
    int m_max_len;
    int n_curr_len;

    double progress;
    double loss;
    double ms_update;
    struct timespec last_time;

    char fill_char;
    char empty_char;
    char left_margin;
    char right_margin;
};
typedef struct tagProgressBarData PBD;

void initProgressBar(PBD *p, int max_len, char left, char right, char fill, char empty, double update_rate);
char *buildProgressBar(PBD* p);
void drawProgressBar(PBD* p);