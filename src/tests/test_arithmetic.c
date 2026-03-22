#include "../s21_decimal.h"
#include "../headers/s21_big_decimal.h"
#include "../headers/s21_utils.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



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

START_TEST(s21_sub_basic) {
    s21_decimal a = {{5, 0, 0, 0}};   // 5
    s21_decimal b = {{3, 0, 0, 0}};   // 3
    s21_decimal result = {0};
    
    int code = s21_sub(a, b, &result);
    
    ck_assert_int_eq(code, OK);              // Без ошибок
    ck_assert_uint_eq(result.bits[0], 2);    // 5 - 3 = 2
    ck_assert_uint_eq(result.bits[1], 0);    // Остальные биты 0
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_uint_eq(result.bits[3], 0);    // Положительный знак
}
END_TEST

START_TEST(s21_sub_test_failing_0) {
    s21_decimal value_1 = {{879, 0, 0, 0x00050000}};   // -0.00879, scale = 5
    s21_decimal value_2 = {{123456, 0, 0, 0x00060000}}; // 0.123456, scale = 6

    // переводим value_1 в scale = 6: -0.00879 → -0.008790
    // value_2 уже scale = 6: 0.123456
    // 0.008790 + 0.123456 = 0.132246 → result = -0.132246

    value_1.bits[3] |= 0x80000000;  // sign = 1

    s21_decimal result = {0};

    // -0.132246, scale = 6, sign = 1
    s21_decimal result_exp = {{132246, 0, 0, 0x80060000}};


    int code = s21_sub(value_1, value_2, &result);

    ck_assert_int_eq(code, OK);

    ck_assert_uint_eq(result.bits[0], result_exp.bits[0]);
    ck_assert_uint_eq(result.bits[1], result_exp.bits[1]);
    ck_assert_uint_eq(result.bits[2], result_exp.bits[2]);

    ck_assert_int_eq(s21_get_sign(&result), 1);
    ck_assert_int_eq(s21_get_scale(&result), 6);
    ck_assert_int_eq(s21_get_scale(&result_exp), 6);
}
END_TEST

START_TEST(s21_big_apply_bankers_round_test) {
    s21_big_decimal big = {0};
    big.bits[0] = 0xFFFFFFFF;  // 96‑бит = 0xFFFFFFFF
    big.bits[3] = 1;            // 0.5
    big.bits[4] = 1;            // rest_dropped = 1
    
    int code = s21_big_apply_bankers_round(&big);  // >> 0.5 → round up
    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(big.bits[0], 0x00000000);  // increment
    ck_assert_int_eq(big.bits[1], 0x00000001);  // carry
    // bits[3..6] = 0
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

    tcase_add_test(tc_core, s21_sub_basic);

    tcase_add_test(tc_core, s21_sub_test_failing_0);

    tcase_add_test(tc_core, s21_big_apply_bankers_round_test);


    suite_add_tcase(s, tc_core);
    return s;
}
