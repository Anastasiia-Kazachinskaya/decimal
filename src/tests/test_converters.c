#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_utils.h"
#include "../headers/s21_big_decimal.h"

// s21_big_to_decimal_internal tests section
// Валидные scale
START_TEST(s21_big_to_decimal_scale_0) {
    s21_big_decimal big = {{123, 0, 0, 0, 0, 0, 0}, 0, 0};
    s21_decimal dec;
    s21_big_to_decimal_internal(&big, &dec);
    ck_assert_int_eq(s21_get_scale(&dec), 0);
}
END_TEST

START_TEST(s21_big_to_decimal_scale_28) {
    s21_big_decimal big = {{123, 0, 0, 0, 0, 0, 0}, 0, 28};
    s21_decimal dec;
    s21_big_to_decimal_internal(&big, &dec);
    ck_assert_int_eq(s21_get_scale(&dec), 28);
}
END_TEST

// Невалидные scale должны обрезаться до 0
START_TEST(s21_big_to_decimal_scale_29) {
    s21_big_decimal big = {{123, 0, 0, 0, 0, 0, 0}, 0, 29};
    s21_decimal dec;
    s21_big_to_decimal_internal(&big, &dec);
    ck_assert_int_eq(s21_get_scale(&dec), 0);  // Защита от невалидных данных
}
END_TEST

START_TEST(s21_big_to_decimal_scale_300) {
    s21_big_decimal big = {{123, 0, 0, 0, 0, 0, 0}, 0, 300};
    s21_decimal dec;
    s21_big_to_decimal_internal(&big, &dec);
    ck_assert_int_eq(s21_get_scale(&dec), 0);  // Защита от невалидных данных
}
END_TEST

Suite *s21_converters_suite(void) {
    Suite *s = suite_create("converters");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_big_to_decimal_scale_0);
    tcase_add_test(tc_core, s21_big_to_decimal_scale_28);
    tcase_add_test(tc_core, s21_big_to_decimal_scale_29);
    tcase_add_test(tc_core, s21_big_to_decimal_scale_300);
    
    suite_add_tcase(s, tc_core);
    return s;
}
