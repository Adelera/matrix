#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"
#define SUCCESS 1
#define FAILURE 0
#define MAX_DIFF 0.000001

int s21_eq_matrix(s21_matrix *A, s21_matrix *B) {
  if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL)
    return FAILURE;

  int result = 1;

  if (A->rows == B->rows && A->columns == B->columns) {
    for (int i = 0; i < A->rows && result == SUCCESS; i++) {
      for (int j = 0; j < A->columns && result == SUCCESS; j++) {
        double diff = 0.0;
        diff = fabs(A->matrix[i][j] - B->matrix[i][j]);

        if (diff < MAX_DIFF) {
          result = SUCCESS;
        } else {
          result = FAILURE;
        }
      }
    }
  } else {
    result = FAILURE;
  }

  return result;
}