#include "../s21_decimal.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_utils.h"
#include "../headers/s21_big_decimal.h"

// s21_normalize_and_check_overflow tests section
START_TEST(s21_normalize_and_check_overflow_no_overflow) {
    
    s21_big_decimal value;
    s21_null_big_decimal(&value);
    value.bits[0] = 12345;
    // bits[1..6] = 0
    
    
    int result = s21_normalize_and_check_overflow(&value);
    ck_assert_int_eq(result, 0);
}
END_TEST



// s21_truncate tests section
START_TEST(s21_truncate_scale_zero) {
    s21_decimal result;
    s21_decimal value;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    value.bits[0] = 12345;
    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], value.bits[0]);
    ck_assert_int_eq(result.bits[1], value.bits[1]);
    ck_assert_int_eq(result.bits[2], value.bits[2]);
    ck_assert_int_eq(result.bits[3], value.bits[3]);
}
END_TEST

START_TEST(s21_truncate_scale_positive) {
    s21_decimal result;
    s21_decimal value;
    s21_null_decimal(&value);
    result.bits[0] = 123;
    result.bits[1] = 456;
    result.bits[2] = 789;
    result.bits[3] = MAX4BITE;


    value.bits[0] = 1;
    value.bits[3] = 1 << 16;
    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 0);
}
END_TEST


START_TEST(s21_truncate_negative_small) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);

    value.bits[0] = 9;
    value.bits[3] = 2 << 16;
    value.bits[3] |= 1u << 31;

    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result.bits[3], 0);

}
END_TEST


START_TEST(s21_truncate_normal_positive) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 1234;
    value.bits[3] = 3 << 16;

    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
    int result_scale = s21_get_scale(&result); 
    ck_assert_int_eq(result_scale, 0);
}
END_TEST


START_TEST(s21_truncate_normal_negative) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 1234;
    value.bits[3] = 3 << 16 | 1u << 31;

    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
    int result_scale = s21_get_scale(&result);
    int result_sign = s21_get_sign(&result);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 1);


}
END_TEST


START_TEST(s21_truncate_scale_28_max) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 1;
    value.bits[3] = 28 << 16;
    
    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

// s21_divide_mantissa_by_10 tests section
START_TEST(s21_divide_mantissa_by_10_zero_bit) {
    s21_decimal value;
    s21_null_decimal(&value);

    value.bits[0] = 123;

    s21_divide_mantissa_by_10(&value);
    ck_assert_int_eq(value.bits[0], 12);
}
END_TEST


START_TEST(s21_divide_mantissa_remainder) {
    s21_decimal value;
    s21_null_decimal(&value);
    value.bits[0] = 123;
    
    int result = s21_divide_mantissa_by_10(&value);
    
    ck_assert_int_eq(value.bits[0], 12);    // 123 / 10 = 12
    ck_assert_int_eq(result, 1);          // функция возвращает 1 при наличиии остатка
}
END_TEST


START_TEST(s21_divide_mantissa_drop_high_remainder) {
    s21_decimal value;
    s21_null_decimal(&value);
    value.bits[0] = 95;
    value.bits[1] = 123;
    
    int result = s21_divide_mantissa_by_10(&value);
    
    ck_assert_int_eq(value.bits[0], 1288490198);
    ck_assert_int_eq(value.bits[1], 12);
    ck_assert_int_eq(value.bits[2], 0);
    ck_assert_int_eq(result, 1);
}
END_TEST


// int s21_floor tests section


// 1.234 → 1
START_TEST(s21_floor_positive_fractional) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 1234;
    value.bits[3] = 3 << 16;
    
    s21_floor(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 1);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST

// -1.234 → -2
START_TEST(s21_floor_negative_fractional) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 1234;
    value.bits[3] = 3 << 16 | 1u << 31;
    
    s21_floor(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 2);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 1);
}
END_TEST


// -12 → -12
START_TEST(s21_floor_negative_integer) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 12;
    value.bits[3] = 0 << 16 | 1u << 31;
    
    s21_floor(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 12);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 1);
}
END_TEST

// -0/0 → 0
START_TEST(s21_floor_zero_negative) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 0;
    value.bits[3] = 0 << 16 | 1u << 31;
    
    s21_floor(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 0);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST

// -0.0001 -> -1
START_TEST(s21_floor_negative_near_zero) {
    s21_decimal value = {0}, result = {0};
    
    value.bits[0] = 1;
    value.bits[3] = (4 << 16) | (1u << 31);
    
    s21_floor(value, &result);
    
    ck_assert_int_eq(result.bits[0], 1);   
    ck_assert_int_eq(s21_get_sign(&result), 1);
    ck_assert_int_eq(s21_get_scale(&result), 0);
}
END_TEST


// int s21_round tests section

// 1,5 → 2
START_TEST(s21_round_1_5_up) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 15;
    value.bits[3] = 1 << 16;
    
    s21_round(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 2);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST


// 2,5 → 3
START_TEST(s21_round_2_5_stay) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 25;
    value.bits[3] = 1 << 16;
    
    s21_round(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 3);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST

// 1,3 → 1
START_TEST(s21_round_1_3_stay) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 13;
    value.bits[3] = 1 << 16;
    
    s21_round(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 1);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST

// 1,6 → 2
START_TEST(s21_round_1_6_up) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    value.bits[0] = 16;
    value.bits[3] = 1 << 16;
    
    s21_round(value, &result);
    
    int result_scale = s21_get_scale(&result); 
    int result_sign = s21_get_sign(&result);
    
    ck_assert_int_eq(result.bits[0], 2);
    ck_assert_int_eq(result_scale, 0);
    ck_assert_int_eq(result_sign, 0);
}
END_TEST

// 1.4449 → 1 (0.4449 < 0.5)
START_TEST(s21_round_1_4449) {
    s21_decimal value = {{14449, 0, 0, 4 << 16}}, result;  // scale=4
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
    ck_assert_int_eq(s21_get_scale(&result), 0);
}
END_TEST

// 1.445 → 1 (0.445 < 0.5, НЕ 1.45 → 1.5 → 2!)
START_TEST(s21_round_1_445) {
    s21_decimal value = {{1445, 0, 0, 3 << 16}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
}
END_TEST

// 1.45 → 1 (0.45 < 0.5)
START_TEST(s21_round_1_45) {
    s21_decimal value = {{145, 0, 0, 2 << 16}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
}
END_TEST

// 1.5000 → 2
START_TEST(s21_round_1_5000) {
    s21_decimal value = {{15000, 0, 0, 4 << 16}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 2);
}
END_TEST

// 1.4999 → 1
START_TEST(s21_round_1_4999) {
    s21_decimal value = {{14999, 0, 0, 4 << 16}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 1);
}
END_TEST

// 1.5001 → 2
START_TEST(s21_round_1_5001) {
    s21_decimal value = {{15001, 0, 0, 4 << 16}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 2);
}
END_TEST

// -1.5 → -2
START_TEST(s21_round_neg_1_5) {
    s21_decimal value = {{15, 0, 0, (1 << 16) | (1 << 31)}}, result;
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], 2);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

// Тест на перенос: -4294967295.0 → -4294967296
// Проверяет, что при добавлении 1 к 0xFFFFFFFF происходит правильный carry в bits[1]
START_TEST(s21_floor_negative_carry_bits0_to_bits1) {
    s21_decimal value, result;
    s21_null_decimal(&value);
    s21_null_decimal(&result);
    
    // Мантисса: 42949672950 = 9 * 2^32 + 4294967286
    // В шестнадцатеричном виде: 0x9FFFFFFF6
    value.bits[0] = 0xFFFFFFF6;  // 4294967286
    value.bits[1] = 9;           // 9
    value.bits[2] = 0;
    value.bits[3] = (1 << 16) | (1u << 31);
    
    int ret = s21_floor(value, &result);
    
    ck_assert_int_eq(ret, OK);
    
    // Ожидаем: -4294967296 = -0x100000000
    ck_assert_uint_eq(result.bits[0], 0);  // Перенос: было 0xFFFFFFFF, стало 0
    ck_assert_uint_eq(result.bits[1], 1);  // Перенос: было 0, стало 1
    ck_assert_uint_eq(result.bits[2], 0);
    
    ck_assert_int_eq(s21_get_scale(&result), 0);
    ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST


Suite *s21_other_suite(void) {
    Suite *s = suite_create("other");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_normalize_and_check_overflow_no_overflow);
    
    tcase_add_test(tc_core, s21_truncate_scale_zero);
    tcase_add_test(tc_core, s21_truncate_scale_positive);
    tcase_add_test(tc_core, s21_truncate_negative_small);
    tcase_add_test(tc_core, s21_truncate_normal_positive);
    tcase_add_test(tc_core, s21_truncate_normal_negative);
    tcase_add_test(tc_core, s21_truncate_scale_28_max);
    
    tcase_add_test(tc_core, s21_divide_mantissa_by_10_zero_bit);
    tcase_add_test(tc_core, s21_divide_mantissa_remainder);
    tcase_add_test(tc_core, s21_divide_mantissa_drop_high_remainder);

    tcase_add_test(tc_core, s21_floor_positive_fractional);
    tcase_add_test(tc_core, s21_floor_negative_fractional);
    tcase_add_test(tc_core, s21_floor_negative_integer);
    tcase_add_test(tc_core, s21_floor_zero_negative);
    tcase_add_test(tc_core, s21_floor_negative_near_zero);
    tcase_add_test(tc_core, s21_floor_negative_carry_bits0_to_bits1);


    tcase_add_test(tc_core, s21_round_1_5_up);
    tcase_add_test(tc_core, s21_round_2_5_stay);
    tcase_add_test(tc_core, s21_round_1_3_stay);
    tcase_add_test(tc_core, s21_round_1_6_up);
    tcase_add_test(tc_core, s21_round_1_4449);
    tcase_add_test(tc_core, s21_round_1_445);
    tcase_add_test(tc_core, s21_round_1_45);
    tcase_add_test(tc_core, s21_round_1_5000);
    tcase_add_test(tc_core, s21_round_1_4999);
    tcase_add_test(tc_core, s21_round_1_5001);
    tcase_add_test(tc_core, s21_round_neg_1_5);
    
    
    suite_add_tcase(s, tc_core);
    return s;
}
