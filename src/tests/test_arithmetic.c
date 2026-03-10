#include "../s21_decimal.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/s21_big_decimal.h"
#include "../headers/s21_utils.h"



// s21_sub tests section
START_TEST(s21_sub_positive_minus_zero){
    int status;

    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal zero = s21_decimal_to_big_internal(&value_2);
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(five, zero, &big_result);

    ck_assert_int_eq(big_result.bits[0], 5);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    ck_assert_int_eq(status, OK);
}
END_TEST



// TODO: s21_big_to_decimal TEST


START_TEST(s21_sub_positive_minus_lower_positive){
    int status;

    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{3, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal three = s21_decimal_to_big_internal(&value_2);
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(five, three, &big_result);

    ck_assert_int_eq(big_result.bits[0], 2);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    ck_assert_int_eq(status, OK);
}
END_TEST


START_TEST(s21_sub_positive_minus_larger_positive){
    int status;

    s21_decimal value_1 = {{3, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal three = s21_decimal_to_big_internal(&value_2);
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(three, five, &big_result);

    ck_assert_int_eq(big_result.bits[0], 2);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    ck_assert_int_eq(status, OK);
}
END_TEST


START_TEST(s21_sub_modules_two_minus_five){
    int status;

    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    s21_big_decimal two = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal five = s21_decimal_to_big_internal(&value_2);
    two.sign = 1;
    five.sign = 1;
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(five, two, &big_result);

    ck_assert_int_eq(big_result.bits[0], 3);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    ck_assert_int_eq(big_result.sign, 0);
    ck_assert_int_eq(status, OK);
}
END_TEST

START_TEST(s21_sub_modules_five_minus_two){
    int status;

    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{2, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal two = s21_decimal_to_big_internal(&value_2);
    five.sign = 1;
    two.sign = 1;
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(five, two, &big_result);

    ck_assert_int_eq(big_result.bits[0], 3);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    // знак меняется в основной функции s21_sub
    ck_assert_int_eq(big_result.sign, 0);
    ck_assert_int_eq(status, OK);
}
END_TEST

START_TEST(s21_sub_modules_five_minus_five){
    int status;

    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    s21_big_decimal five = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal five2 = s21_decimal_to_big_internal(&value_2);
    five.sign = 1;
    five2.sign = 1;
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_sub(five, five2, &big_result);

    ck_assert_int_eq(big_result.bits[0], 0);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    // знак меняется в основной функции s21_sub
    ck_assert_int_eq(big_result.sign, 0);
    ck_assert_int_eq(status, OK);
}
END_TEST


START_TEST(s21_sub_negative_minus_positive){
    int status;

    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    s21_big_decimal two = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal five = s21_decimal_to_big_internal(&value_2);
    two.sign = 1;
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_add(two, five, &big_result);

    ck_assert_int_eq(big_result.bits[0], 7);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    // знак меняется в основной функции s21_sub
    ck_assert_int_eq(big_result.sign, 0);
    ck_assert_int_eq(status, OK);
}
END_TEST


START_TEST(s21_sub_positive_minus_negative){
    int status;

    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{5, 0, 0, 0}};
    s21_big_decimal two = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal five = s21_decimal_to_big_internal(&value_2);
    five.sign = 1;
    s21_big_decimal big_result;
    s21_null_big_decimal(&big_result);

    status = s21_big_add(two, five, &big_result);

    ck_assert_int_eq(big_result.bits[0], 7);
    ck_assert_int_eq(big_result.bits[1], 0);
    ck_assert_int_eq(big_result.bits[2], 0);
    ck_assert_int_eq(big_result.bits[6], 0);
    // знак меняется в основной функции s21_sub
    ck_assert_int_eq(big_result.sign, 0);
    ck_assert_int_eq(status, OK);
}
END_TEST



Suite *s21_arithmetic_suite(void) {
    Suite *s = suite_create("arithmetic");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_sub_positive_minus_zero);
    tcase_add_test(tc_core, s21_sub_positive_minus_lower_positive);
    tcase_add_test(tc_core, s21_sub_positive_minus_larger_positive);
    tcase_add_test(tc_core, s21_sub_modules_two_minus_five);
    tcase_add_test(tc_core, s21_sub_modules_five_minus_two);
    tcase_add_test(tc_core, s21_sub_modules_five_minus_five);
    tcase_add_test(tc_core, s21_sub_negative_minus_positive);
    tcase_add_test(tc_core, s21_sub_positive_minus_negative);


    suite_add_tcase(s, tc_core);
    return s;
}
