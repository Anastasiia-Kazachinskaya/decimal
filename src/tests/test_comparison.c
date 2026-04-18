#include "../s21_decimal.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_utils.h"
#include "../headers/s21_big_decimal.h"


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

START_TEST(test_is_equal_same) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_different_scale) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{100, 0, 0, 0x00010000}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_different_values) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{11, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_equal_pos_neg_zero) {
    s21_decimal a = {{0, 0, 0, 0x00000000}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_less_basic) {
    s21_decimal a = {{5, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_negative) {
    s21_decimal a = {{10, 0, 0, (int)0x80000000}};
    s21_decimal b = {{5,  0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_is_less_pos_vs_neg) {
    s21_decimal a = {{5, 0, 0, (int)0x80000000}};
    s21_decimal b = {{5, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_with_scale) {
    s21_decimal a = {{15, 0, 0, 0x00010000}};
    s21_decimal b = {{20, 0, 0, 0x00010000}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_equal_values) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_is_greater_basic) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{5,  0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_greater(a, b), 1);
    ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_is_greater_negative) {
    s21_decimal a = {{5,  0, 0, (int)0x80000000}};
    s21_decimal b = {{10, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_is_greater_equal_values) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_is_less_or_equal_less) {
    s21_decimal a = {{5,  0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_equal) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_less_or_equal_greater) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{5,  0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_greater) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{5,  0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_equal) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_less) {
    s21_decimal a = {{5,  0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_not_equal_different) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{11, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_not_equal_same) {
    s21_decimal a = {{10, 0, 0, 0x00000000}};
    s21_decimal b = {{10, 0, 0, 0x00000000}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST


 Suite *s21_comparison_suite(void){
    Suite *s = suite_create("s21_comparison");
    TCase *tc = tcase_create("comparison");

    tcase_add_test(tc, s21_is_big_greater_five_minus_zero);
    tcase_add_test(tc, test_is_equal_same);
    tcase_add_test(tc, test_is_equal_different_scale);
    tcase_add_test(tc, test_is_equal_different_values);
    tcase_add_test(tc, test_is_equal_pos_neg_zero);
    tcase_add_test(tc, test_is_less_basic);
    tcase_add_test(tc, test_is_less_negative);
    tcase_add_test(tc, test_is_less_pos_vs_neg);
    tcase_add_test(tc, test_is_less_with_scale);
    tcase_add_test(tc, test_is_less_equal_values);
    tcase_add_test(tc, test_is_greater_basic);
    tcase_add_test(tc, test_is_greater_negative);
    tcase_add_test(tc, test_is_greater_equal_values);
    tcase_add_test(tc, test_is_less_or_equal_less);
    tcase_add_test(tc, test_is_less_or_equal_equal);
    tcase_add_test(tc, test_is_less_or_equal_greater);
    tcase_add_test(tc, test_is_greater_or_equal_greater);
    tcase_add_test(tc, test_is_greater_or_equal_equal);
    tcase_add_test(tc, test_is_greater_or_equal_less);
    tcase_add_test(tc, test_is_not_equal_different);
    tcase_add_test(tc, test_is_not_equal_same);

    suite_add_tcase(s, tc);
    return s;
}
