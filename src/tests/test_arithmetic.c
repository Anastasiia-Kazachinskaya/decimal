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

    status = s21_big_sub(&five, &zero, &big_result);

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

    status = s21_big_sub(&five, &three, &big_result);

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

    status = s21_big_sub(&three, &five, &big_result);

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

    status = s21_big_sub(&five, &two, &big_result);

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

    status = s21_big_sub(&five, &two, &big_result);

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

    status = s21_big_sub(&five, &five2, &big_result);

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

    status = s21_big_add(&two, &five, &big_result);

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

    status = s21_big_add(&two, &five, &big_result);

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

START_TEST (s21_sub_test_result_is_null) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};


    int code = s21_sub(a, b, NULL);

    ck_assert_int_eq(code, 1); 
}
END_TEST


START_TEST(s21_sub_overflow_max) {
    // (2^96-1) - 1 = valid result
    s21_decimal v1 = {{4294967295U, 4294967295U, 4294967295U, 0}};
    s21_decimal v2 = {{1, 0, 0, 0}};
    s21_decimal result = {0};
    int code = s21_sub(v1, v2, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq((unsigned)result.bits[0], 4294967294U);
    ck_assert_uint_eq((unsigned)result.bits[1], 4294967295U);
    ck_assert_uint_eq((unsigned)result.bits[2], 4294967295U);
}
END_TEST


START_TEST(s21_sub_overflow_min) {
    // 1 - (2^96-1) = -(2^96-2), fits in decimal with negative sign
    s21_decimal v1 = {{1, 0, 0, 0}};
    s21_decimal v2 = {{4294967295U, 4294967295U, 4294967295U, 0}};
    s21_decimal result = {0};
    int code = s21_sub(v1, v2, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq((unsigned)result.bits[0], 4294967294U);
    ck_assert_uint_eq((unsigned)result.bits[1], 4294967295U);
    ck_assert_uint_eq((unsigned)result.bits[2], 4294967295U);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(s21_handle_overflow_scale_decrement) {
    s21_big_decimal test_big;
    s21_null_big_decimal(&test_big);

    test_big.scale = 15;
    test_big.bits[3] = 1;

    ck_assert_int_eq(check_overflow(&test_big), 1);

    int code = s21_handle_overflow_and_rounding(&test_big);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(test_big.scale, 14);
    ck_assert_int_eq(test_big.bits[3], 0);
}
END_TEST

// s21_add tests section
START_TEST(s21_add_two_positive) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 8);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_add_two_negative) {
    s21_decimal a = {{5, 0, 0, 0x80000000}};
    s21_decimal b = {{3, 0, 0, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 8);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(s21_add_positive_and_negative) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 2);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_add_negative_and_positive) {
    s21_decimal a = {{3, 0, 0, 0x80000000}};
    s21_decimal b = {{5, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 2);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_add_zero_and_value) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{42, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 42);
}
END_TEST

START_TEST(s21_add_with_scale) {
    // 1.5 + 2.5 = 4.0
    s21_decimal a = {{15, 0, 0, 0x00010000}};   // scale=1
    s21_decimal b = {{25, 0, 0, 0x00010000}};   // scale=1
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 40);
    ck_assert_int_eq(s21_get_scale(&result), 1);
}
END_TEST

START_TEST(s21_add_different_scales) {
    // 1.5 (scale=1) + 0.25 (scale=2) = 1.75
    s21_decimal a = {{15, 0, 0, 0x00010000}};   // 1.5
    s21_decimal b = {{25, 0, 0, 0x00020000}};   // 0.25
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 175);
    ck_assert_int_eq(s21_get_scale(&result), 2);
}
END_TEST

START_TEST(s21_add_result_null) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};

    int code = s21_add(a, b, NULL);

    ck_assert_int_ne(code, OK);
}
END_TEST

START_TEST(s21_add_opposite_equal) {
    // 5 + (-5) = 0
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{5, 0, 0, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_add(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 0);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
}
END_TEST

// s21_mul tests section
START_TEST(s21_mul_two_positive) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 15);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_mul_positive_negative) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 15);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(s21_mul_two_negative) {
    s21_decimal a = {{5, 0, 0, 0x80000000}};
    s21_decimal b = {{3, 0, 0, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 15);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_mul_by_zero) {
    s21_decimal a = {{12345, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 0);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
}
END_TEST

START_TEST(s21_mul_by_one) {
    s21_decimal a = {{12345, 0, 0, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 12345);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(s21_mul_with_scale) {
    // 1.5 * 2.0 = 3.00
    s21_decimal a = {{15, 0, 0, 0x00010000}};   // 1.5, scale=1
    s21_decimal b = {{20, 0, 0, 0x00010000}};   // 2.0, scale=1
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_uint_eq(result.bits[0], 300);      // 3.00 = 300 * 10^-2
    ck_assert_int_eq(s21_get_scale(&result), 2);
}
END_TEST

START_TEST(s21_mul_large_numbers) {
    // 100000 * 100000 = 10000000000
    s21_decimal a = {{100000, 0, 0, 0}};
    s21_decimal b = {{100000, 0, 0, 0}};
    s21_decimal result = {0};

    int code = s21_mul(a, b, &result);

    ck_assert_int_eq(code, OK);
    // 10000000000 = 0x2540BE400 -> bits[0]=0x540BE400, bits[1]=0x2
    ck_assert_uint_eq(result.bits[0], 0x540BE400);
    ck_assert_uint_eq(result.bits[1], 0x2);
}
END_TEST

START_TEST(s21_mul_result_null) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{3, 0, 0, 0}};

    int code = s21_mul(a, b, NULL);

    ck_assert_int_ne(code, OK);
}
END_TEST

// div tests
START_TEST(test_div_simple) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{2,  0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 5);
    ck_assert_int_eq(s21_get_scale(&result), 0);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_div_by_one) {
    s21_decimal a = {{123456, 0, 0, 0x00030000}};
    s21_decimal b = {{1, 0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 123456);
    ck_assert_int_eq(s21_get_scale(&result), 3);
}
END_TEST

START_TEST(test_div_one_third) {
    s21_decimal a = {{1, 0, 0, 0x00000000}};
    s21_decimal b = {{3, 0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_int_eq(s21_get_scale(&result), 28);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_div_plus_minus) {
    // 10 / -2 = -5
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{2,  0, 0, (int)0x80000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 5);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_div_minus_minus) {
    // -10 / -2 = 5
    s21_decimal a = {{10, 0, 0, (int)0x80000000}};
    s21_decimal b = {{2,  0, 0, (int)0x80000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 5);
    ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_div_zero_dividend) {
    // 0 / 5 = 0
    s21_decimal a = {{0, 0, 0, 0x00000000}};
    s21_decimal b = {{5, 0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_div_by_zero) {
    s21_decimal a = {{5, 0, 0, 0x00000000}};
    s21_decimal b = {{0, 0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), DIVISION_BY_ZERO);
}
END_TEST

START_TEST(test_div_null_result) {
    s21_decimal a = {{5, 0, 0, 0x00000000}};
    s21_decimal b = {{2, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_div(a, b, NULL), ERROR);
}
END_TEST

START_TEST(test_div_with_scale) {
    s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF,
                      (int)0x801C0000}};  // scale=28, sign=1
    s21_decimal b = {{4, 0, 0, 0x000E0000}};  // scale=14
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_int_eq(s21_get_sign(&result), 1);
    ck_assert_int_eq(s21_get_scale(&result), 14);
}
END_TEST


START_TEST(test_div_max_by_max) {
    s21_decimal a = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
    s21_decimal b = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 1);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq(s21_get_scale(&result), 0);
}
END_TEST

START_TEST(test_div_exact_quarter) {
    s21_decimal a = {{100, 0, 0, 0x00020000}};
    s21_decimal b = {{4,   0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_int_eq(s21_get_scale(&result), 2);
    ck_assert_uint_eq(result.bits[0], 25);
}
END_TEST

START_TEST(test_div_negative_scale_correction) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{1,  0, 0, 0x00010000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 100);
    ck_assert_int_eq(s21_get_scale(&result), 0);
}
END_TEST

START_TEST(test_div_large_scale_correction) {
    s21_decimal a = {{1, 0, 0, 0x00140000}};
    s21_decimal b = {{1, 0, 0, 0x00090000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_uint_eq(result.bits[0], 1);
    ck_assert_int_eq(s21_get_scale(&result), 11);
}
END_TEST

START_TEST(test_div_scale_over_28) {
    s21_decimal a = {{1, 0, 0, 0x001C0000}};
    s21_decimal b = {{3, 0, 0, 0x00000000}};
    s21_decimal result = {0};
    ck_assert_int_eq(s21_div(a, b, &result), OK);
    ck_assert_int_eq(s21_get_scale(&result), 28);
    ck_assert_int_eq(s21_get_sign(&result), 0);
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
    tcase_add_test(tc_core, s21_sub_test_result_is_null);
    tcase_add_test(tc_core, s21_sub_overflow_max);
    tcase_add_test(tc_core, s21_sub_overflow_min);
    tcase_add_test(tc_core, s21_handle_overflow_scale_decrement);

    tcase_add_test(tc_core, s21_add_two_positive);
    tcase_add_test(tc_core, s21_add_two_negative);
    tcase_add_test(tc_core, s21_add_positive_and_negative);
    tcase_add_test(tc_core, s21_add_negative_and_positive);
    tcase_add_test(tc_core, s21_add_zero_and_value);
    tcase_add_test(tc_core, s21_add_with_scale);
    tcase_add_test(tc_core, s21_add_different_scales);
    tcase_add_test(tc_core, s21_add_result_null);
    tcase_add_test(tc_core, s21_add_opposite_equal);

    tcase_add_test(tc_core, s21_mul_two_positive);
    tcase_add_test(tc_core, s21_mul_positive_negative);
    tcase_add_test(tc_core, s21_mul_two_negative);
    tcase_add_test(tc_core, s21_mul_by_zero);
    tcase_add_test(tc_core, s21_mul_by_one);
    tcase_add_test(tc_core, s21_mul_with_scale);
    tcase_add_test(tc_core, s21_mul_large_numbers);
    tcase_add_test(tc_core, s21_mul_result_null);

    tcase_add_test(tc_core, test_div_simple);
    tcase_add_test(tc_core, test_div_by_one);
    tcase_add_test(tc_core, test_div_one_third);
    tcase_add_test(tc_core, test_div_plus_minus);
    tcase_add_test(tc_core, test_div_minus_minus);
    tcase_add_test(tc_core, test_div_zero_dividend);
    tcase_add_test(tc_core, test_div_by_zero);
    tcase_add_test(tc_core, test_div_null_result);
    tcase_add_test(tc_core, test_div_with_scale);
    tcase_add_test(tc_core, test_div_max_by_max);
    tcase_add_test(tc_core, test_div_exact_quarter);
    tcase_add_test(tc_core, test_div_negative_scale_correction);
    tcase_add_test(tc_core, test_div_large_scale_correction);
    tcase_add_test(tc_core, test_div_scale_over_28);

    suite_add_tcase(s, tc_core);
    return s;
}
