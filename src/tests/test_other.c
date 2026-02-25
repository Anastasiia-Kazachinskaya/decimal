#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"


START_TEST(s21_get_overflow_no_overflow) {
    
    s21_big_decimal value;
    s21_null_big_decimal(&value);
    value.bits[0] = 12345;
    // bits[1..6] = 0
    
    
    int result = s21_get_overflow(&value);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_big_add_overflow_integration) {
    s21_big_decimal a, b, res;
    s21_null_big_decimal(&a);
    s21_null_big_decimal(&b);
    s21_null_big_decimal(&res);
    
    for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
        a.bits[i] = 0xFFFFFFFF;
        b.bits[i] = 0xFFFFFFFF;
    }
    
    int add_result = s21_big_add(a, b, &res);
    
    // === ОТЛАДОЧНЫЙ ВЫВОД ===
    printf("\n[DEBUG] s21_big_add overflow test:\n");
    printf("  add_result = %d (0=OK, 1=ERROR)\n", add_result);
    printf("  res.bits[4] = 0x%X (старшее слово)\n", res.bits[4]);
    printf("  res.bits[0] = 0x%X (младшее слово)\n", res.bits[0]);
    // =======================
    
    ck_assert_int_eq(add_result, 1);
}
END_TEST


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


int s21_truncate(s21_decimal value, s21_decimal* result);
Suite *s21_other_suite(void) {
    Suite *s = suite_create("other");
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_get_overflow_no_overflow);
    
    tcase_add_test(tc_core, s21_big_add_overflow_integration);

    tcase_add_test(tc_core, s21_truncate_scale_zero);
    tcase_add_test(tc_core, s21_truncate_scale_positive);
    tcase_add_test(tc_core, s21_truncate_negative_small);
    tcase_add_test(tc_core, s21_truncate_normal_positive);
    tcase_add_test(tc_core, s21_truncate_normal_negative);
    tcase_add_test(tc_core, s21_truncate_scale_28_max);


    suite_add_tcase(s, tc_core);
    return s;
}
