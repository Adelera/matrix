#include <check.h>
#include <stdlib.h>

#include "s21_matrix.h"

START_TEST(eq_matrix_identical) {
  s21_matrix A, B;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

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
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  int result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(result, 0);  // Ожидаем FAILURE (0)

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_precision_equal) {
  s21_matrix A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

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
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

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
  s21_create_matrix(2, 2, &A);

  int result = s21_eq_matrix(&A, NULL);
  ck_assert_int_eq(result, 0);

  s21_remove_matrix(&A);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, eq_matrix_identical);
  tcase_add_test(tcase_core, eq_matrix_different_sizes);
  tcase_add_test(tcase_core, eq_matrix_precision_equal);
  tcase_add_test(tcase_core, eq_matrix_precision_not_equal);
  tcase_add_test(tcase_core, eq_matrix_null);

  suite_add_tcase(suite, tcase_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed ? 1 : 0;
}