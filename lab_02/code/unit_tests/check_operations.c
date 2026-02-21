#include <stdlib.h>

#include "check_operations.h"
#include "errors.h"
#include "operations.h"

#define EPS 0.0000001

START_TEST(std_iter_one_elem) {
  int array[] = {0};

  double avg = calc_avg_iter(array);
  ck_assert_double_eq_tol(avg, 0.0, EPS);
}
END_TEST

START_TEST(std_iter_many_elem) {
  int array[] = {1, 2, 3, 3, 4, 5, 4, 5, 6, 2, 1, 7, 5, 6, 0};

  double avg = calc_avg_iter(array);
  ck_assert_double_eq_tol(avg, 3.6, EPS);
}
END_TEST

START_TEST(std_iter_zero_avg) {
  int array[] = {1, -1, -1, 1, 2, -3, 4, -3, 0};

  double avg = calc_avg_iter(array);
  ck_assert_double_eq_tol(avg, 0.0, EPS);
}
END_TEST

START_TEST(std_rec_one_elem) {
  int array[] = {0};

  long long sum = 0;
  size_t len = 0;
  double avg = calc_avg_rec(array, &sum, &len);
  ck_assert_double_eq_tol(avg, 0.0, EPS);
}
END_TEST

START_TEST(std_rec_many_elem) {
  int array[] = {1, 2, 3, 3, 4, 5, 4, 5, 6, 2, 1, 7, 5, 6, 0};

  long long sum = 0;
  size_t len = 0;
  double avg = calc_avg_rec(array, &sum, &len);
  ck_assert_double_eq_tol(avg, 3.6, EPS);
}
END_TEST

START_TEST(std_rec_zero_avg) {
  int array[] = {1, -1, -1, 1, 2, -3, 4, -3, 0};

  long long sum = 0;
  size_t len = 0;
  double avg = calc_avg_rec(array, &sum, &len);
  ck_assert_double_eq_tol(avg, 0.0, EPS);
}
END_TEST

Suite *avg_suite(void) {
  Suite *s = suite_create("avg");

  TCase *tc_iter = tcase_create("iter");
  tcase_add_test(tc_iter, std_iter_one_elem);
  tcase_add_test(tc_iter, std_iter_many_elem);
  tcase_add_test(tc_iter, std_iter_zero_avg);
  suite_add_tcase(s, tc_iter);

  TCase *tc_rec = tcase_create("rec");
  tcase_add_test(tc_rec, std_rec_one_elem);
  tcase_add_test(tc_rec, std_rec_many_elem);
  tcase_add_test(tc_rec, std_rec_zero_avg);
  suite_add_tcase(s, tc_rec);

  return s;
}
