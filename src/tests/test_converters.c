#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/s21_helpers.h"

// int_to_decimal

Suite *int_conversion_suite(void) {
    Suite *s = suite_create("Converters");
    
    TCase *tc_core = tcase_create("Core Tests");

    tcase_add_test(tc_core, int_to_decimal_test_positive_int);
    tcase_add_test(tc_core, int_to_decimal_test_negative_int);
    tcase_add_test(tc_core, int_to_decimal_test_zero);
    tcase_add_test(tc_core, int_to_decimal_test_min_int);
    tcase_add_test(tc_core, int_to_decimal_test_max_int);
    tcase_add_test(tc_core, int_to_decimal_test_one);
    tcase_add_test(tc_core, int_to_decimal_test_minus_one);
    tcase_add_test(tc_core, int_to_decimal_test_null_pointer);
    tcase_add_test(tc_core, int_to_decimal_test_boundary_values);
    tcase_add_test(tc_core, int_to_decimal_test_random_numbers);
    tcase_add_test(tc_core, int_to_decimal_test_clean_before_write);
    tcase_add_test(tc_core, int_to_decimal_test_large_positive);
    tcase_add_test(tc_core, int_to_decimal_test_large_negative);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}