/*
 * file: file_handling.h
 * description: header file that gives access to all the linear regression funcitons
 * author: Ryan Wagner
 * date: June 6, 2025
 * notes:
 */

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "../header/math_funcs.h"

int loadCSVtoMatrix(const char *filename, bool has_header, Matrix *m);

// Vector loadCSVtoVector(const char *filename, int  col_idx, int has_header);

#endif // FILE_HANDLING_H