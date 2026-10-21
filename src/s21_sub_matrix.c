#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_sub_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
  int status = 0;

  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL || A->rows <= 0 || B->rows <= 0 || A->columns <= 0 ||
      B->columns <= 0) {
    status = 1;
    return status;
  }

  if (A->rows == B->rows && A->columns == B->columns) {
    if (s21_create_matrix(A->rows, A->columns, result) != 0) status = 1;

    for (int i = 0; i < A->rows && status == 0; i++) {
      for (int j = 0; j < A->columns && status == 0; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        if (!isfinite(result->matrix[i][j])) status = 2;
      }
    }
  } else {
    status = 2;
  }

  return status;
}