#include "../s21_decimal.h"
#include "../headers/s21_utils.h"

#include <check.h>


// s21_is_zero tests section
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


// s21_null_decimal tests section
START_TEST(s21_null_decimal_basic){
    s21_decimal val = {{1, 2, 3, 4}};
    s21_null_decimal(&val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST


START_TEST(s21_null_decimal_garbage){
    s21_decimal val;
    s21_null_decimal(&val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_null_decimal_already_zero){
    s21_decimal val = {{0, 0, 0, 0}};
    s21_null_decimal(&val);
    ck_assert_int_eq(val.bits[0], 0);
}
END_TEST

START_TEST(s21_null_decimal_test) {
    s21_decimal d;
    // Намеренно заполняем мусором
    d.bits[0] = 0xDEADBEEF;
    d.bits[1] = 0xCAFEBABE;
    d.bits[2] = 0x12345678;
    d.bits[3] = 0xFFFFFFFF;
    
    s21_null_decimal(&d);
    
    ck_assert_int_eq(d.bits[0], 0);
    ck_assert_int_eq(d.bits[1], 0);
    ck_assert_int_eq(d.bits[2], 0);
    ck_assert_int_eq(d.bits[3], 0);
}
END_TEST


// s21_get_bit tests section
START_TEST(s21_get_bit_basic){
    s21_decimal val = {{0, 0, 1 << 6, 0}};
    int index_of_bit = 70;
    int result;
    result = s21_get_bit(val, index_of_bit);
    ck_assert_int_eq(result, 1);
}
END_TEST


// s21_set_bit tests section
START_TEST(s21_set_bit_basic){
    s21_decimal val = {{0, 0, 1 << 6, 0}};
    int index_of_bit = 70;
    int bit_value = 0;

    s21_set_bit(&val, index_of_bit, bit_value);
    
    int result;
    result = s21_get_bit(val, index_of_bit);
    ck_assert_int_eq(result, 0);
}
END_TEST

// s21_get_scale tests section
START_TEST(s21_get_scale_basic){
    s21_decimal val = {{0, 0, 0, 0}};
    val.bits[3] = 5 << 16;
    int result = s21_get_scale(&val);
    ck_assert_int_eq(result, 5);

}
END_TEST

// s21_get_sign tests section
START_TEST(s21_get_sign_basic){
    s21_decimal val = {{0, 0, 0, 0}};
    val.bits[3] = 1u << 31;
    int result = s21_get_sign(&val);
    ck_assert_int_eq(result, 1);

}
END_TEST

START_TEST(s21_set_sign_case_big_decimal_convert_to_decimal){
    s21_big_decimal val1;
    s21_decimal val2;
    s21_null_big_decimal(&val1);
    s21_null_decimal(&val2);

    val1.sign = 1;
    val2.bits[3] = 0;

    s21_set_sign(&val1, &val2);
    int result = s21_get_sign(&val2);
    ck_assert_int_eq(result, 1);

}
END_TEST


Suite* s21_utils_suite(void) {
    Suite *s = suite_create("utils");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_is_zero_first_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_second_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_third_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_fourth_bit_is_digit);
    tcase_add_test(tc_core, s21_is_zero_all_zero);
    tcase_add_test(tc_core, s21_is_zero_multiple_nonzero);
    tcase_add_test(tc_core, s21_null_decimal_test);
    tcase_add_test(tc_core, s21_null_decimal_basic);
    tcase_add_test(tc_core, s21_null_decimal_garbage);
    tcase_add_test(tc_core, s21_null_decimal_already_zero);
    tcase_add_test(tc_core, s21_get_bit_basic);
    tcase_add_test(tc_core, s21_set_bit_basic);
    tcase_add_test(tc_core, s21_get_scale_basic);
    tcase_add_test(tc_core, s21_get_sign_basic);
    tcase_add_test(tc_core, s21_set_sign_case_big_decimal_convert_to_decimal);

    suite_add_tcase(s, tc_core);
    return s;
}

