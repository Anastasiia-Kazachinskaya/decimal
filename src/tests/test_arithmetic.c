#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"

// s21_sub tests section
START_TEST(s21_sub_positive_minus_zero){
    int status;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{0, 0, 0, 0}};
    status = s21_sub(value_1, value_2, &result);
    ck_assert_int_eq(result.bits[0], 5);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
    ck_assert_int_eq(status, 0);
}
END_TEST


START_TEST(s21_sub_positive_minus_lower_positive){
    int status;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{3, 0, 0, 0}};
    status = s21_sub(value_1, value_2, &result);
    ck_assert_int_eq(result.bits[0], 2);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
    ck_assert_int_eq(status, 0);
}
END_TEST


START_TEST(s21_sub_positive_minus_larger_positive){
    int status;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    status = s21_sub(value_1, value_2, &result);
    ck_assert_int_eq(result.bits[0], 3);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(s21_get_sign(&result), 1);
    ck_assert_int_eq(status, 0);
}
END_TEST


START_TEST(s21_sub_negative_minus_negative){
    int status;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};

    value_1.bits[3] |= 1u << 31;
    value_2.bits[3] |= 1u << 31;

    status = s21_sub(value_1, value_2, &result);
    ck_assert_int_eq(result.bits[0], 3);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(s21_get_sign(&result), 0);
    ck_assert_int_eq(status, 0);
}
END_TEST



Suite *s21_arithmetic_suite(void) {
    Suite *s = suite_create("arithmetic");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_sub_positive_minus_zero);
    tcase_add_test(tc_core, s21_sub_positive_minus_lower_positive);
    tcase_add_test(tc_core, s21_sub_positive_minus_larger_positive);
    tcase_add_test(tc_core, s21_sub_negative_minus_negative);

    suite_add_tcase(s, tc_core);
    return s;
}

