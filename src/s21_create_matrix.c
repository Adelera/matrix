#include <stdlib.h>

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, s21_matrix *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) return 1;

  int status = 0;

  result->rows = rows;
  result->columns = columns;
  result->matrix = (double **)calloc(rows, sizeof(double *));

  if (!result->matrix) {
    status = 1;
  } else {
    for (int i = 0; i < rows && status == 0; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
    }
  }

  return status;
}