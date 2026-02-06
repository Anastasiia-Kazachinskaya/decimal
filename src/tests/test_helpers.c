#include <check.h>
#include "../headers/s21_helpers.h"



START_TEST(s21_is_zero_first_bit_is_digit){
    s21_decimal val = {{21, 0, 0, 0}};
    ck_assert_int_eq(s21_is_zero(val), 0);
}
END_TEST

START_TEST(s21_is_zero_second_bit_is_digit){
    s21_decimal val = {{0, 40, 0, 0}};
    ck_assert_int_eq(s21_is_zero(val), 0);
}
END_TEST

START_TEST(s21_is_zero_third_bit_is_digit){
    s21_decimal val = {{0, 0, 30, 0}};
    ck_assert_int_eq(s21_is_zero(val), 0);
}
END_TEST


START_TEST(s21_is_zero_fourth_bit_is_digit){
    s21_decimal val = {{0, 0, 0, 40}};
    ck_assert_int_eq(s21_is_zero(val), 1);
}
END_TEST

START_TEST(s21_is_zero_all_zero) {
    s21_decimal val = {{0, 0, 0, 0}};
    ck_assert_int_eq(s21_is_zero(val), 1);
}
END_TEST

START_TEST(s21_is_zero_multiple_nonzero) {
    s21_decimal val = {{1, 2, 0, 0}};
    ck_assert_int_eq(s21_is_zero(val), 0);
}
END_TEST




Suite* s21_helpers_suite(void) {
    Suite *s = suite_create("helpers");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_is_zero_first_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_second_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_third_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_fourth_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_all_zero);
    tcase_add_test(tc_core, s21_is_zero_multiple_nonzero);

    suite_add_tcase(s, tc_core);
    return s;
}

