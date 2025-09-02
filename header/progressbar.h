/*
 * file: progressbar.h
 * description: header file that gives access to all the progress bar functions
 * author: Ryan Wagner
 * date: Augest 30, 2025
 * notes:
 * src: https://stackoverflow.com/questions/60949001/progress-bar-in-c-for-an-arbitrary-long-execution-console
 */

struct tagProgressBarData
{
    int nMaxLen;
    int nCurLen;

    double Progress;
    double Loss;

    char FillChr;
    char EmptyChr;
    char LeftMargin;
    char RightMargin;
};
typedef struct tagProgressBarData PBD;

void initProgressBar(PBD* p, int MaxLen, char Left, char Right, char Fill, char Empty);
void drawProgressBar(PBD* p);