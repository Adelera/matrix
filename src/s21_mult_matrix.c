#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_mult_matrix(s21_matrix *A, s21_matrix *B, s21_matrix *result) {
  int status = 0;

  if (A == NULL || B == NULL || result == NULL || A->matrix == NULL ||
      B->matrix == NULL || A->rows <= 0 || B->rows <= 0 || A->columns <= 0 ||
      B->columns <= 0) {
    status = 1;
    return status;
  }

  if (A->columns == B->rows) {
    if (s21_create_matrix(A->rows, B->columns, result) != 0) status = 1;

    for (int i = 0; i < A->rows && status == 0; i++) {
      for (int j = 0; j < B->columns && status == 0; j++) {
        for (int k = 0; k < A->columns && status == 0; k++) {
          result->matrix[i][j] += (A->matrix[i][k] * B->matrix[k][j]);
          if (!isfinite(result->matrix[i][j])) status = 2;
        }
      }
    }
  } else {
    status = 2;
  }

  return status;
}