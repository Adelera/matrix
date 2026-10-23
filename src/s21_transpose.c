#include <stdlib.h>

#include "s21_matrix.h"

int s21_transpose(s21_matrix *A, s21_matrix *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL || A->rows <= 0 ||
      A->columns <= 0) {
    status = 1;
    return status;
  }

  if (s21_create_matrix(A->columns, A->rows, result) != 0) status = 1;

  if (status == 0) {
    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  }

  return status;
}