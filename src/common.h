#ifndef S21_COMMON_H
#define S21_COMMON_H

#include "s21_matrix.h"

int create_subMatrix(s21_matrix *A, int skip_row, int skip_column,
                     s21_matrix *sub_A);

#endif