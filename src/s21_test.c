#include <check.h>
#include <float.h>
#include <stdlib.h>

#include "s21_matrix.h"

// ТЕСТЫ ДЛЯ s21_eq_matrix
START_TEST(eq_matrix_identical) {
  s21_matrix A, B;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &B), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[0][2] = 3.0;

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 1);  // Ожидаем SUCCESS (1)

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_different_sizes) {
  s21_matrix A, B;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);  // Ожидаем FAILURE (0)

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_precision_equal) {
  s21_matrix A, B;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 1, &B), 0);

  // Первые 6 знаков после запятой абсолютно одинаковы (123456)
  A.matrix[0][0] = 0.1234561;
  B.matrix[0][0] = 0.1234562;

  int result = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(result, 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_precision_not_equal) {
  s21_matrix A, B;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 1, &B), 0);

  A.matrix[0][0] = 0.123456;
  B.matrix[0][0] = 0.123457;  // Отличие в 6-м знаке

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_null) {
  s21_matrix A;
  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);

  int result = s21_eq_matrix(&A, NULL);
  ck_assert_int_eq(result, 0);

  s21_remove_matrix(&A);
}
END_TEST

// ТЕСТЫ ДЛЯ s21_num_matrix
START_TEST(sum_matrix) {
  s21_matrix A, B, result;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &B), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[0][2] = 3.0;

  s21_matrix expected;
  ck_assert_int_eq(s21_create_matrix(1, 3, &expected), 0);
  expected.matrix[0][0] = 2.0;
  expected.matrix[0][1] = 4.0;
  expected.matrix[0][2] = 6.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}

START_TEST(sum_matrix_not_correct_sizes) {
  s21_matrix A, B, result;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}

START_TEST(sum_matrix_not_exist) {
  s21_matrix A, result;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);

  int status = s21_sum_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, 1);

  s21_remove_matrix(&A);
}

START_TEST(sum_matrix_not_correct_rows) {
  s21_matrix A, B, result;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &B), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &result), 0);

  B.columns = -3;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

START_TEST(sum_matrix_nan) {
  s21_matrix A, B, result;
  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &B), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &result), 0);

  B.matrix[0][1] = NAN;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

START_TEST(sum_matrix_overflow) {
  s21_matrix A, B, result;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = DBL_MAX;
  B.matrix[0][0] = DBL_MAX;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

// ТЕСТЫ ДЛЯ s21_sub_matrix.c
START_TEST(sub_matrix_normal) {
  s21_matrix A, B, result, expected;

  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &B), 0);
  ck_assert_int_eq(s21_create_matrix(1, 3, &expected), 0);

  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 10.0;
  A.matrix[0][2] = 15.0;
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[0][2] = 3.0;

  expected.matrix[0][0] = 4.0;
  expected.matrix[0][1] = 8.0;
  expected.matrix[0][2] = 12.0;

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(sub_matrix_wrong_sizes) {
  s21_matrix A, B, result;

  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_null) {
  s21_matrix B, result;

  int status = s21_sub_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

// ТЕСТЫ ДЛЯ s21_mult_number.c
START_TEST(mult_number_normal) {
  s21_matrix A, result, expected;
  double number = 2.0;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &expected), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = -2.0;

  expected.matrix[0][0] = 2.0;
  expected.matrix[0][1] = 4.0;
  expected.matrix[0][2] = 6.0;
  expected.matrix[1][0] = 0.0;
  expected.matrix[1][1] = 8.0;
  expected.matrix[1][2] = -4.0;

  int status = s21_mult_number(&A, number, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}

START_TEST(mult_number_nan) {
  s21_matrix A, result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  double number = NAN;

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = -2.0;

  int status = s21_mult_number(&A, number, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}

START_TEST(mult_number_null) {
  s21_matrix result;
  double number = 2.0;

  int status = s21_mult_number(NULL, number, &result);
  ck_assert_int_eq(status, 1);
}

// ТЕСТЫ ДЛЯ s21_mult_matrix.c
START_TEST(mult_matrix_normal) {
  s21_matrix A, B, result, expected;
  ck_assert_int_eq(s21_create_matrix(3, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &expected), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 5.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = -1.0;
  B.matrix[0][2] = 1.0;
  B.matrix[1][0] = 2.0;
  B.matrix[1][1] = 3.0;
  B.matrix[1][2] = 4.0;

  expected.matrix[0][0] = 9.0;
  expected.matrix[0][1] = 11.0;
  expected.matrix[0][2] = 17.0;
  expected.matrix[1][0] = 12.0;
  expected.matrix[1][1] = 13.0;
  expected.matrix[1][2] = 22.0;
  expected.matrix[2][0] = 15.0;
  expected.matrix[2][1] = 15.0;
  expected.matrix[2][2] = 27.0;

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}

START_TEST(mult_matrix_null) {
  s21_matrix B, result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &B), 0);

  int status = s21_mult_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, 1);

  s21_remove_matrix(&B);
}

START_TEST(mult_matrix_wrong_sizes) {
  s21_matrix A, B, result;
  ck_assert_int_eq(s21_create_matrix(3, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}

// ТЕСТЫ ДЛЯ s21_transpose.c
START_TEST(transpose_normal) {
  s21_matrix A, result, expected;
  ck_assert_int_eq(s21_create_matrix(3, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 3, &expected), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 5.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;

  expected.matrix[0][0] = 1.0;
  expected.matrix[0][1] = 2.0;
  expected.matrix[0][2] = 3.0;
  expected.matrix[1][0] = 4.0;
  expected.matrix[1][1] = 5.0;
  expected.matrix[1][2] = 6.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}

START_TEST(transpose_null) {
  s21_matrix result;

  int status = s21_transpose(NULL, &result);
  ck_assert_int_eq(status, 1);
}

// ТЕСТЫ ДЛЯ s21_determinant
START_TEST(determinant_normal) {
  s21_matrix A;
  double result;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 7.0;
  A.matrix[2][1] = 8.0;
  A.matrix[2][2] = 9.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result, 0.0, 1e-7);

  s21_remove_matrix(&A);
}

START_TEST(determinant_null) {
  double result;

  int status = s21_determinant(NULL, &result);
  ck_assert_int_eq(status, 1);
}

START_TEST(determinant_2x2) {
  s21_matrix A;
  double result;
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);

  A.matrix[0][0] = 1.0;

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result, 1.0, 1e-7);

  s21_remove_matrix(&A);
}

START_TEST(determinant_wrong_size) {
  s21_matrix A;
  double result;
  ck_assert_int_eq(s21_create_matrix(3, 1, &A), 0);

  int status = s21_determinant(&A, &result);
  ck_assert_int_eq(status, 2);

  s21_remove_matrix(&A);
}

// ТЕСТЫ ДЛЯ s21_calc_complements
START_TEST(complements_normal) {
  s21_matrix A;
  s21_matrix result;
  s21_matrix expected;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &expected), 0);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  expected.matrix[0][0] = 0.0;
  expected.matrix[0][1] = 10.0;
  expected.matrix[0][2] = -20.0;
  expected.matrix[1][0] = 4.0;
  expected.matrix[1][1] = -14.0;
  expected.matrix[1][2] = 8.0;
  expected.matrix[2][0] = -8.0;
  expected.matrix[2][1] = -2.0;
  expected.matrix[2][2] = 4.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}

START_TEST(complements_null) {
  s21_matrix result;
  int status = s21_calc_complements(NULL, &result);
  ck_assert_int_eq(status, 1);
}

START_TEST(complements_wrong_size) {
  s21_matrix A;
  s21_matrix result;
  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, 2);
}

// ТЕСТЫ ДЛЯ s21_inverse_matrix.c
START_TEST(inverse_normal) {
  s21_matrix A;
  s21_matrix result;
  s21_matrix expected;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &expected), 0);

  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;

  expected.matrix[0][0] = 1.0;
  expected.matrix[0][1] = -1.0;
  expected.matrix[0][2] = 1.0;
  expected.matrix[1][0] = -38.0;
  expected.matrix[1][1] = 41.0;
  expected.matrix[1][2] = -34.0;
  expected.matrix[2][0] = 27.0;
  expected.matrix[2][1] = -29.0;
  expected.matrix[2][2] = 24.0;

  int status = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}

START_TEST(inverse_null) {
  s21_matrix result;

  int status = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(status, 1);
}

int main(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, eq_matrix_identical);
  tcase_add_test(tcase_core, eq_matrix_different_sizes);
  tcase_add_test(tcase_core, eq_matrix_precision_equal);
  tcase_add_test(tcase_core, eq_matrix_precision_not_equal);
  tcase_add_test(tcase_core, eq_matrix_null);

  tcase_add_test(tcase_core, sum_matrix);
  tcase_add_test(tcase_core, sum_matrix_not_correct_sizes);
  tcase_add_test(tcase_core, sum_matrix_not_exist);
  tcase_add_test(tcase_core, sum_matrix_not_correct_rows);
  tcase_add_test(tcase_core, sum_matrix_nan);
  tcase_add_test(tcase_core, sum_matrix_overflow);

  tcase_add_test(tcase_core, sub_matrix_normal);
  tcase_add_test(tcase_core, sub_matrix_wrong_sizes);
  tcase_add_test(tcase_core, sub_matrix_null);

  tcase_add_test(tcase_core, mult_number_normal);
  tcase_add_test(tcase_core, mult_number_nan);
  tcase_add_test(tcase_core, mult_number_null);

  tcase_add_test(tcase_core, mult_matrix_normal);
  tcase_add_test(tcase_core, mult_matrix_null);
  tcase_add_test(tcase_core, mult_matrix_wrong_sizes);

  tcase_add_test(tcase_core, transpose_normal);
  tcase_add_test(tcase_core, transpose_null);

  tcase_add_test(tcase_core, determinant_normal);
  tcase_add_test(tcase_core, determinant_null);
  tcase_add_test(tcase_core, determinant_2x2);
  tcase_add_test(tcase_core, determinant_wrong_size);

  tcase_add_test(tcase_core, complements_normal);
  tcase_add_test(tcase_core, complements_null);
  tcase_add_test(tcase_core, complements_wrong_size);

  tcase_add_test(tcase_core, inverse_normal);
  tcase_add_test(tcase_core, inverse_null);

  suite_add_tcase(suite, tcase_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed ? 1 : 0;
}