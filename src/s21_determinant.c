#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"

int s21_determinant(s21_matrix *A, double *result) {
  int status = 0;
  double determinant = 0;
  int sign = 1;

  if (A == NULL || result == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    status = 1;
    return status;
  }

  if (A->rows == A->columns) {
    if (A->rows == 1) {
      determinant = A->matrix[0][0];
    } else if (A->rows == 2) {
      determinant = (A->matrix[0][0] * A->matrix[1][1]) - (A->matrix[0][1] * A->matrix[1][0]);
    } else {
      for (int j = 0; j < A->columns && status == 0; j++) {
        s21_matrix sub_A;
        status = s21_create_matrix(A->rows - 1, A->rows - 1, &sub_A);
        
        if (status == 0) {
          for (int i = 1; i < A->rows; i++) {
            int sub_A_i = i - 1;
            int sub_A_j = 0;

            for (int k = 0; k < A->columns; k++) {
              if (k != j) {
                sub_A.matrix[sub_A_i][sub_A_j] = A->matrix[i][k];
                sub_A_j++;
              }
            }
          }

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