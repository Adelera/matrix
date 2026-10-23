#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int create_subMatrix(s21_matrix *A, int skip_row, int skip_column,
                     s21_matrix *sub_A);

int s21_determinant(s21_matrix *A, double *result) {
  int status = 0;
  double determinant = 0;
  int sign = 1;

  if (A == NULL || result == NULL || A->matrix == NULL || A->rows <= 0 ||
      A->columns <= 0) {
    status = 1;
    return status;
  }

  if (A->rows == A->columns) {
    if (A->rows == 1) {
      determinant = A->matrix[0][0];
    } else if (A->rows == 2) {
      determinant = (A->matrix[0][0] * A->matrix[1][1]) -
                    (A->matrix[0][1] * A->matrix[1][0]);

      if (!isfinite(determinant)) status = 2;
    } else {
      for (int j = 0; j < A->columns && status == 0; j++) {
        s21_matrix sub_A;
        status = create_subMatrix(A, 0, j, &sub_A);

        if (status == 0) {
          double sub_determinant = 0;
          status = s21_determinant(&sub_A, &sub_determinant);

          if (status == 0) {
            determinant += sign * A->matrix[0][j] * sub_determinant;
          }

          s21_remove_matrix(&sub_A);
          sign = -sign;
        }
      }
    }
  } else {
    status = 2;
  }

  if (status == 0) *result = determinant;
  return status;
}

int create_subMatrix(s21_matrix *A, int skip_row, int skip_column,
                     s21_matrix *sub_A) {
  int status = s21_create_matrix(A->rows - 1, A->columns - 1, sub_A);

  if (status == 0) {
    int sub_A_i = 0;

    for (int i = 0; i < A->rows; i++) {
      int sub_A_j = 0;

      if (i != skip_row) {
        for (int j = 0; j < A->columns; j++) {
          if (j != skip_column) {
            sub_A->matrix[sub_A_i][sub_A_j] = A->matrix[i][j];
            sub_A_j++;
          }
        }
        sub_A_i++;
      }
    }
  } else {
    status = 1;
  }

  return status;
}