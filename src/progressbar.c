/*
 * file: progressbar.c
 * description: file used define the progress bar functions
 * author: Ryan Wagner
 * date: August 30, 2025
 * notes:
 */

#include "progressbar.h"
#include <stdio.h>

/**
 * @brief Initialize a ProgressBar object
 *
 * @param p Pointer to PBD progress bar object
 * @param MaxLen Maximum character length of the progress bar
 * @param Left Left character of the progress bar
 * @param Right Right character of the progress bar
 * @param Fill Character that represents completion of the progress bar
 * @param Empty Character that represents incompletion of the progress bar
 *
 * @return void
 */
void initProgressBar(PBD *p, int MaxLen, char Left, char Right, char Fill, char Empty)
{
    p->nMaxLen = MaxLen;
    p->nCurLen = 0;

    p->Progress = 0.0;
    p->Loss = 0.0;

    p->LeftMargin = Left;
    p->RightMargin = Right;
    p->FillChr = Fill;
    p->EmptyChr = Empty;
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
    /** Move to the beginning of the line. */
    printf("\r");

    /** Print the left margin char. */
    printf("%c", p->LeftMargin);

    /** Make sure that MaxLen >= CurLen */
    if (p->nMaxLen < p->nCurLen)
    {
        p->nCurLen = p->nMaxLen;
    }

    /** Print the progress with the Fill char. */
    for (int i = 0; i < p->nCurLen; i++)
    {
        printf("%c", p->FillChr);
    }

    /** Complete whats left with the Fill char. */
    for (int i = 0; i < p->nMaxLen - p->nCurLen; i++)
    {
        printf("%c", p->EmptyChr);
    }

    /** Print the right margin char. */
    printf("%c", p->RightMargin);

    // Print the percentage done
    printf(" [%%%3.2lf]", p->Progress);

    // Print the loss
    // printf(" [Loss: %.4lf]", p->Loss);
}