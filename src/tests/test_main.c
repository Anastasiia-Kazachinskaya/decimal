// tests/test_main.c
#include <check.h>
#include <stdlib.h> 
#include "../headers/s21_decimal.h"
#include "../headers/s21_helpers.h"

#include "test_arithmetic.c"
#include "test_comparison.c" 
#include "test_converters.c"
#include "test_helpers.c"
#include "test_other.c"

Suite* s21_decimal_suite(void) {
    Suite *s = suite_create("s21_decimal");
    suite_add_tcase(s, s21_arithmetic_suite());
    suite_add_tcase(s, s21_comparison_suite());
    suite_add_tcase(s, s21_converters_suite());
    suite_add_tcase(s, s21_helpers_suite());
    suite_add_tcase(s, s21_other_suite());
    return s;
}

int main(void) {
    Suite *s = s21_decimal_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
