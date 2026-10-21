#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_mult_number(s21_matrix *A, double number, s21_matrix *result) {
  int status = 0;

  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0 ||
      result == NULL) {
    status = 1;
    return status;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != 0) status = 1;

  for (int i = 0; i < A->rows && status == 0; i++) {
    for (int j = 0; j < A->columns && status == 0; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
      if (!isfinite(result->matrix[i][j])) status = 2;
    }
  }

  return status;
}