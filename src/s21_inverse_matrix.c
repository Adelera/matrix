#include <stdlib.h>

#include "common.h"
#include "s21_matrix.h"

int s21_inverse_matrix(s21_matrix *A, s21_matrix *result) {
  int status = 0;

  if (A == NULL || result == NULL || A->matrix == NULL || A->rows <= 0 ||
      A->columns <= 0) {
    status = 1;
    return status;
  }

  double determinant = 0;
  status = s21_determinant(A, &determinant);

  if (status == 0 && determinant != 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != 0) status = 1;

    s21_matrix calc_complements;
    s21_matrix transposed_matrix;
    status = s21_create_matrix(A->rows, A->columns, &calc_complements);
    status = s21_create_matrix(A->rows, A->columns, &transposed_matrix);

    if (status == 0) {
      status = s21_calc_complements(A, &calc_complements);
    }

    if (status == 0) {
      s21_transpose(&calc_complements, &transposed_matrix);

      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = transposed_matrix.matrix[i][j] * (-1);
        }
      }
    }
  }

  return status;
}