/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "./tests/test.check" instead.
 */

#include <check.h>

// #line 1 "./tests/test.check"
#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_smart_calc.h"
#include "../stack.h"

START_TEST(calc_test1) {
#line 11
  char str[256] = "-3+4*X";
  char postfix_expr[256] = {0};
  Stack_d stack_d;
  stack_d.size_d = 0;
  double X = 1;
  ck_assert_double_eq(calc(to_postfix(str, postfix_expr), stack_d, X), 1);
}
END_TEST

START_TEST(calc_test2) {
#line 19
  char str[256] = "SIN(45)+582*54/(5/(8-8)";
  char postfix_expr[256] = {0};
  Stack_d stack_d;
  stack_d.size_d = 0;
  double X = 1;
  ck_assert_double_nan(calc(to_postfix(str, postfix_expr), stack_d, X));
}
END_TEST

START_TEST(calc_test3) {
#line 27
  //  sin(-12) = 0.5365729180
  char str[256] = "SIN(-12)";
  char postfix_expr[256] = {0};
  double res = 0.5365729180;
  double x = 0;
  Stack_d stack_d;
  stack_d.size_d = 0;
  double my_res = calc(to_postfix(str, postfix_expr), stack_d, x);
  ck_assert_double_eq_tol(res, my_res, 1e-07);
}
END_TEST

START_TEST(calc_test4) {
#line 38
  char str[256] = "ATAN(SIN(45)+582*54/(5/(18-8))";
  char postfix_expr[256] = {0};
  double res = 1.570780417548;
  double x = 0;
  Stack_d stack_d;
  stack_d.size_d = 0;
  double my_res = calc(to_postfix(str, postfix_expr), stack_d, x);
  ck_assert_double_eq_tol(res, my_res, 1e-07);
}
END_TEST

START_TEST(calc_test5) {
#line 48
  char str[256] = "SQRT(4)*SIN(0.2)/COS(1)";
  tokenize_operator(str);
  char str_orig[256] = "q(4)*s(0.2)/c(1)";
  ck_assert_int_eq(strcmp(str_orig, str), 0);
}
END_TEST

START_TEST(calc_test6) {
#line 54
  char str[256] = "LN(LOG(100))";
  char postfix_expr[256] = {0};
  double res = 0.6931471805599453;
  double x = 0;
  Stack_d stack_d;
  stack_d.size_d = 0;
  double my_res = calc(to_postfix(str, postfix_expr), stack_d, x);
  ck_assert_double_eq_tol(res, my_res, 1e-07);
}
END_TEST

START_TEST(calc_test7) {
#line 64
  char str[256] = "0.5+0.05";
  char res[256] = {0};
  char postfix_expr[256] = {0};
  double x = 0;
  Stack_d stack_d;
  stack_d.size_d = 0;
  sprintf(res, "%.7f", calc(to_postfix(str, postfix_expr), stack_d, x));
  delete_null(res);
  char str_orig[256] = "0.55";
  ck_assert_int_eq(strcmp(str_orig, res), 0);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, calc_test1);
  tcase_add_test(tc1_1, calc_test2);
  tcase_add_test(tc1_1, calc_test3);
  tcase_add_test(tc1_1, calc_test4);
  tcase_add_test(tc1_1, calc_test5);
  tcase_add_test(tc1_1, calc_test6);
  tcase_add_test(tc1_1, calc_test7);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}