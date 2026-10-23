#include <stdlib.h>

#include "common.h"
#include "s21_matrix.h"

int s21_calc_complements(s21_matrix *A, s21_matrix *result) {
  int status = 0;
  if (A == NULL || result == NULL || A->matrix == NULL || A->rows <= 0 ||
      A->columns <= 0) {
    status = 1;
    return status;
  }

  if (A->rows == A->columns) {
    if (s21_create_matrix(A->rows, A->columns, result) != 0) status = 1;

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns && status == 0; j++) {
        double minor = 0;
        int curr_sign = ((i + j) % 2 == 0) ? 1 : -1;
        s21_matrix sub_A;
        status = create_subMatrix(A, i, j, &sub_A);

        if (status == 0) {
          status = s21_determinant(&sub_A, &minor);

          if (status == 0) {
            result->matrix[i][j] = curr_sign * minor;
          }
        }

        s21_remove_matrix(&sub_A);
      }
    }
  } else {
    status = 2;
  }

  return status;
}