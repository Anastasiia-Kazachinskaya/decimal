#include "../s21_decimal.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_utils.h"


START_TEST(s21_is_big_greater_five_minus_zero) {
    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal zero = s21_decimal_to_big_internal(&value_2);

    int result = s21_is_big_greater(five, zero);
    ck_assert_int_eq(result, 1); 
    
    result = s21_is_big_greater(zero, five);
    ck_assert_int_eq(result, 0);

    result = s21_is_big_greater(five, five);
    ck_assert_int_eq(result, 0);

}
END_TEST


Suite *s21_comparison_suite(void) {
    Suite *s = suite_create("comparison");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_is_big_greater_five_minus_zero);

    suite_add_tcase(s, tc_core);
    return s;
}
