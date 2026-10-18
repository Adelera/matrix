#include <check.h>
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

  suite_add_tcase(suite, tcase_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed ? 1 : 0;
}