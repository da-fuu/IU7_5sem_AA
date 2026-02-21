#include <stdlib.h>

#include "check_operations.h"
#include "errors.h"
#include "matrix.h"
#include "operations.h"

#define EPS 0.0000001

void assert_matrix_eq(matrix_t in1, matrix_t in2) {
  ck_assert(in1.rows == in2.rows && in1.cols == in2.cols);
  for (size_t i = 0; i < in1.rows; i++)
    for (size_t j = 0; j < in1.cols; j++)
      ck_assert_double_eq_tol(in1.elements[i][j], in2.elements[i][j], EPS);
}

void free_all(double **m1, double **m2, double **m3, double **m4) {
  free(m1);
  free(m2);
  free(m3);
  free(m4);
}

// Умножение нулевых матриц
START_TEST(std_multiplication_all_zero) {
  int rc;
  matrix_t in1 = create_matrix(2, 3);
  matrix_t in2 = create_matrix(3, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < 2; i++)
    for (size_t j = 0; j < 2; j++) {
      in1.elements[i][j] = 0.0;
      in2.elements[i][j] = 0.0;
      out_res.elements[i][j] = 0.0;
    }
  in2.elements[2][0] = 0.0;
  in2.elements[2][1] = 0.0;
  in1.elements[0][2] = 0.0;
  in1.elements[1][2] = 0.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_std(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение единичных матриц
START_TEST(std_multiplication_unit) {
  int rc;
  matrix_t in1 = create_matrix(2, 2);
  matrix_t in2 = create_matrix(2, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < in1.rows; i++)
    for (size_t j = 0; j < in1.cols; j++) {
      in1.elements[i][j] = (double)((i + j + 1) % 2);
      in2.elements[i][j] = (double)((i + j + 1) % 2);
      out_res.elements[i][j] = (double)((i + j + 1) % 2);
    }

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_std(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение столбца на строку
START_TEST(std_multiplication_col_by_row) {
  int rc;
  matrix_t in1 = create_matrix(3, 1);
  matrix_t in2 = create_matrix(1, 3);
  matrix_t out;
  matrix_t out_res = create_matrix(3, 3);
  for (size_t i = 0; i < 3; i++) {
    in1.elements[i][0] = (double)i;
    in2.elements[0][i] = (double)i;
  }
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      out_res.elements[i][j] = (double)i * (double)j;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_std(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение строки на столбец
START_TEST(std_multiplication_row_by_col) {
  int rc;
  matrix_t in1 = create_matrix(1, 3);
  matrix_t in2 = create_matrix(3, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  for (size_t i = 0; i < 3; i++) {
    in2.elements[i][0] = (double)i;
    in1.elements[0][i] = (double)i;
  }
  out_res.elements[0][0] = 5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_std(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение матриц из одного элемента
START_TEST(std_multiplication_one_elem) {
  int rc;
  matrix_t in1 = create_matrix(1, 1);
  matrix_t in2 = create_matrix(1, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  in2.elements[0][0] = -100.0;
  in1.elements[0][0] = 0.05;
  out_res.elements[0][0] = -5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_std(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение нулевых матриц
START_TEST(vinograd_multiplication_all_zero) {
  int rc;
  matrix_t in1 = create_matrix(2, 3);
  matrix_t in2 = create_matrix(3, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < 2; i++)
    for (size_t j = 0; j < 2; j++) {
      in1.elements[i][j] = 0.0;
      in2.elements[i][j] = 0.0;
      out_res.elements[i][j] = 0.0;
    }

  in2.elements[2][0] = 0.0;
  in2.elements[2][1] = 0.0;
  in1.elements[0][2] = 0.0;
  in1.elements[1][2] = 0.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение единичных матриц
START_TEST(vinograd_multiplication_unit) {
  int rc;
  matrix_t in1 = create_matrix(2, 2);
  matrix_t in2 = create_matrix(2, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < in1.rows; i++)
    for (size_t j = 0; j < in1.cols; j++) {
      in1.elements[i][j] = (double)((i + j + 1) % 2);
      in2.elements[i][j] = (double)((i + j + 1) % 2);
      out_res.elements[i][j] = (double)((i + j + 1) % 2);
    }

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение столбца на строку
START_TEST(vinograd_multiplication_col_by_row) {
  int rc;
  matrix_t in1 = create_matrix(3, 1);
  matrix_t in2 = create_matrix(1, 3);
  matrix_t out;
  matrix_t out_res = create_matrix(3, 3);
  for (size_t i = 0; i < 3; i++) {
    in1.elements[i][0] = (double)i;
    in2.elements[0][i] = (double)i;
  }
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      out_res.elements[i][j] = (double)i * (double)j;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение строки на столбец
START_TEST(vinograd_multiplication_row_by_col) {
  int rc;
  matrix_t in1 = create_matrix(1, 3);
  matrix_t in2 = create_matrix(3, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  for (size_t i = 0; i < 3; i++) {
    in2.elements[i][0] = (double)i;
    in1.elements[0][i] = (double)i;
  }
  out_res.elements[0][0] = 5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение матриц из одного элемента
START_TEST(vinograd_multiplication_one_elem) {
  int rc;
  matrix_t in1 = create_matrix(1, 1);
  matrix_t in2 = create_matrix(1, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  in2.elements[0][0] = -100.0;
  in1.elements[0][0] = 0.05;
  out_res.elements[0][0] = -5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение нулевых матриц
START_TEST(vinograd_opt_multiplication_all_zero) {
  int rc;
  matrix_t in1 = create_matrix(2, 3);
  matrix_t in2 = create_matrix(3, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < 2; i++)
    for (size_t j = 0; j < 2; j++) {
      in1.elements[i][j] = 0.0;
      in2.elements[i][j] = 0.0;
      out_res.elements[i][j] = 0.0;
    }

  in2.elements[2][0] = 0.0;
  in2.elements[2][1] = 0.0;
  in1.elements[0][2] = 0.0;
  in1.elements[1][2] = 0.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd_opt(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение единичных матриц
START_TEST(vinograd_opt_multiplication_unit) {
  int rc;
  matrix_t in1 = create_matrix(2, 2);
  matrix_t in2 = create_matrix(2, 2);
  matrix_t out;
  matrix_t out_res = create_matrix(2, 2);
  for (size_t i = 0; i < in1.rows; i++)
    for (size_t j = 0; j < in1.cols; j++) {
      in1.elements[i][j] = (double)((i + j + 1) % 2);
      in2.elements[i][j] = (double)((i + j + 1) % 2);
      out_res.elements[i][j] = (double)((i + j + 1) % 2);
    }

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd_opt(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение столбца на строку
START_TEST(vinograd_opt_multiplication_col_by_row) {
  int rc;
  matrix_t in1 = create_matrix(3, 1);
  matrix_t in2 = create_matrix(1, 3);
  matrix_t out;
  matrix_t out_res = create_matrix(3, 3);
  for (size_t i = 0; i < 3; i++) {
    in1.elements[i][0] = (double)i;
    in2.elements[0][i] = (double)i;
  }
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      out_res.elements[i][j] = (double)i * (double)j;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd_opt(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение строки на столбец
START_TEST(vinograd_opt_multiplication_row_by_col) {
  int rc;
  matrix_t in1 = create_matrix(1, 3);
  matrix_t in2 = create_matrix(3, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  for (size_t i = 0; i < 3; i++) {
    in2.elements[i][0] = (double)i;
    in1.elements[0][i] = (double)i;
  }
  out_res.elements[0][0] = 5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd_opt(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

// Умножение матриц из одного элемента
START_TEST(vinograd_opt_multiplication_one_elem) {
  int rc;
  matrix_t in1 = create_matrix(1, 1);
  matrix_t in2 = create_matrix(1, 1);
  matrix_t out;
  matrix_t out_res = create_matrix(1, 1);
  in2.elements[0][0] = -100.0;
  in1.elements[0][0] = 0.05;
  out_res.elements[0][0] = -5.0;

  rc = prepare_out(in1, in2, &out);
  ck_assert_int_eq(rc, OK);

  multiply_vinograd_opt(in1, in2, out);
  assert_matrix_eq(out_res, out);
  free_all(in1.elements, in2.elements, out.elements, out_res.elements);
}
END_TEST

Suite *multiplication_suite(void) {
  Suite *s = suite_create("multiplication");

  TCase *tc_std = tcase_create("std");
  tcase_add_test(tc_std, std_multiplication_all_zero);
  tcase_add_test(tc_std, std_multiplication_unit);
  tcase_add_test(tc_std, std_multiplication_col_by_row);
  tcase_add_test(tc_std, std_multiplication_row_by_col);
  tcase_add_test(tc_std, std_multiplication_one_elem);
  suite_add_tcase(s, tc_std);

  TCase *tc_vinograd = tcase_create("vinograd");
  tcase_add_test(tc_vinograd, vinograd_multiplication_all_zero);
  tcase_add_test(tc_vinograd, vinograd_multiplication_unit);
  tcase_add_test(tc_vinograd, vinograd_multiplication_col_by_row);
  tcase_add_test(tc_vinograd, vinograd_multiplication_row_by_col);
  tcase_add_test(tc_vinograd, vinograd_multiplication_one_elem);
  suite_add_tcase(s, tc_vinograd);

  TCase *tc_vinograd_opt = tcase_create("vinograd_opt");
  tcase_add_test(tc_vinograd_opt, vinograd_opt_multiplication_all_zero);
  tcase_add_test(tc_vinograd_opt, vinograd_opt_multiplication_unit);
  tcase_add_test(tc_vinograd_opt, vinograd_opt_multiplication_col_by_row);
  tcase_add_test(tc_vinograd_opt, vinograd_opt_multiplication_row_by_col);
  tcase_add_test(tc_vinograd_opt, vinograd_opt_multiplication_one_elem);
  suite_add_tcase(s, tc_vinograd_opt);

  return s;
}
