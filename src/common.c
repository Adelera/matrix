#include "s21_matrix.h"

int create_subMatrix(s21_matrix *A, int skip_row, int skip_column,
                     s21_matrix *sub_A) {
  int status = s21_create_matrix(A->rows - 1, A->columns - 1, sub_A);

  if (status == 0) {
    int sub_A_i = 0;

    for (int i = 0; i < A->rows; i++) {
      if (i != skip_row) {
        int sub_A_j = 0;

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