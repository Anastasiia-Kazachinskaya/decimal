#include "../s21_decimal.h"
#include "../headers/s21_big_decimal.h"
#include "../headers/s21_utils.h"

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef FLOAT_EPS
#define FLOAT_EPS 1e-6
#endif

// Round-trip: int -> decimal -> add -> decimal_to_int
START_TEST(complex_int_add_roundtrip) {
    s21_decimal a, b, result;
    s21_from_int_to_decimal(100, &a);
    s21_from_int_to_decimal(200, &b);

    int code = s21_add(a, b, &result);
    ck_assert_int_eq(code, OK);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 300);
}
END_TEST

// Round-trip: int -> decimal -> sub -> decimal_to_int
START_TEST(complex_int_sub_roundtrip) {
    s21_decimal a, b, result;
    s21_from_int_to_decimal(500, &a);
    s21_from_int_to_decimal(300, &b);

    int code = s21_sub(a, b, &result);
    ck_assert_int_eq(code, OK);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 200);
}
END_TEST

// Round-trip: int -> decimal -> mul -> decimal_to_int
START_TEST(complex_int_mul_roundtrip) {
    s21_decimal a, b, result;
    s21_from_int_to_decimal(25, &a);
    s21_from_int_to_decimal(40, &b);

    int code = s21_mul(a, b, &result);
    ck_assert_int_eq(code, OK);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 1000);
}
END_TEST

// Chain: (a + b) - b == a
START_TEST(complex_add_sub_identity) {
    s21_decimal a, b, sum, result;
    s21_from_int_to_decimal(12345, &a);
    s21_from_int_to_decimal(67890, &b);

    s21_add(a, b, &sum);
    s21_sub(sum, b, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 12345);
}
END_TEST

// Chain: a * b + a * c == a * (b + c)
START_TEST(complex_distributive_law) {
    s21_decimal a, b, c, ab, ac, left, bc, right;
    s21_from_int_to_decimal(7, &a);
    s21_from_int_to_decimal(11, &b);
    s21_from_int_to_decimal(13, &c);

    s21_mul(a, b, &ab);
    s21_mul(a, c, &ac);
    s21_add(ab, ac, &left);

    s21_add(b, c, &bc);
    s21_mul(a, bc, &right);

    ck_assert_int_eq(s21_is_equal(left, right), 1);
}
END_TEST

// Negate: -(-x) == x
START_TEST(complex_negate_double) {
    s21_decimal a, neg, result;
    s21_from_int_to_decimal(42, &a);

    s21_negate(a, &neg);
    s21_negate(neg, &result);

    ck_assert_int_eq(s21_is_equal(a, result), 1);
}
END_TEST

// Negate + add == sub
START_TEST(complex_negate_add_equals_sub) {
    s21_decimal a, b, neg_b, add_result, sub_result;
    s21_from_int_to_decimal(100, &a);
    s21_from_int_to_decimal(30, &b);

    s21_negate(b, &neg_b);
    s21_add(a, neg_b, &add_result);
    s21_sub(a, b, &sub_result);

    ck_assert_int_eq(s21_is_equal(add_result, sub_result), 1);
}
END_TEST

// Float -> decimal -> negate -> float
START_TEST(complex_float_negate_roundtrip) {
    s21_decimal d, neg;
    float in_val = 3.14f;
    float out_val = 0.0f;

    s21_from_float_to_decimal(in_val, &d);
    s21_negate(d, &neg);
    s21_from_decimal_to_float(neg, &out_val);

    ck_assert_float_eq_tol(out_val, -3.14f, FLOAT_EPS);
}
END_TEST

// a + 0 == a
START_TEST(complex_add_zero_identity) {
    s21_decimal a, zero, result;
    s21_from_int_to_decimal(-999, &a);
    s21_from_int_to_decimal(0, &zero);

    s21_add(a, zero, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, -999);
}
END_TEST

// a * 1 == a
START_TEST(complex_mul_one_identity) {
    s21_decimal a, one, result;
    s21_from_int_to_decimal(-777, &a);
    s21_from_int_to_decimal(1, &one);

    s21_mul(a, one, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, -777);
}
END_TEST

// Negative chain: (-a) + (-b) == -(a + b)
START_TEST(complex_negative_add_chain) {
    s21_decimal a, b, neg_a, neg_b, left, sum, right;
    s21_from_int_to_decimal(50, &a);
    s21_from_int_to_decimal(70, &b);

    s21_negate(a, &neg_a);
    s21_negate(b, &neg_b);
    s21_add(neg_a, neg_b, &left);

    s21_add(a, b, &sum);
    s21_negate(sum, &right);

    ck_assert_int_eq(s21_is_equal(left, right), 1);
}
END_TEST

// Mul with different scales: 1.5 * 2 = 3.0, check via float
START_TEST(complex_mul_scale_to_float) {
    s21_decimal a = {{15, 0, 0, 0x00010000}};  // 1.5
    s21_decimal b;
    s21_from_int_to_decimal(2, &b);
    s21_decimal result;

    int code = s21_mul(a, b, &result);
    ck_assert_int_eq(code, OK);

    float out = 0.0f;
    s21_from_decimal_to_float(result, &out);
    ck_assert_float_eq_tol(out, 3.0f, FLOAT_EPS);
}
END_TEST

// Comparison after arithmetic: a + 1 > a
START_TEST(complex_comparison_after_add) {
    s21_decimal a, one, result;
    s21_from_int_to_decimal(500, &a);
    s21_from_int_to_decimal(1, &one);

    s21_add(a, one, &result);

    ck_assert_int_eq(s21_is_greater(result, a), 1);
    ck_assert_int_eq(s21_is_less(a, result), 1);
    ck_assert_int_eq(s21_is_not_equal(result, a), 1);
}
END_TEST

// Sub to zero: a - a == 0
START_TEST(complex_sub_self_is_zero) {
    s21_decimal a, result, zero;
    s21_from_int_to_decimal(999999, &a);
    s21_from_int_to_decimal(0, &zero);

    s21_sub(a, a, &result);

    ck_assert_int_eq(s21_is_equal(result, zero), 1);
}
END_TEST

// Negative mul: (-a) * b == -(a * b)
START_TEST(complex_neg_mul_equivalence) {
    s21_decimal a, b, neg_a, left, product, right;
    s21_from_int_to_decimal(13, &a);
    s21_from_int_to_decimal(7, &b);

    s21_negate(a, &neg_a);
    s21_mul(neg_a, b, &left);

    s21_mul(a, b, &product);
    s21_negate(product, &right);

    ck_assert_int_eq(s21_is_equal(left, right), 1);
}
END_TEST

// Floor after add: floor(1.7 + 2.8) == floor(4.5) == 4
START_TEST(complex_floor_after_add) {
    s21_decimal a = {{17, 0, 0, 0x00010000}};  // 1.7
    s21_decimal b = {{28, 0, 0, 0x00010000}};  // 2.8
    s21_decimal sum, result;

    s21_add(a, b, &sum);
    s21_floor(sum, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 4);
}
END_TEST

// Round after mul: round(2.5 * 3) = round(7.5) = 8
START_TEST(complex_round_after_mul) {
    s21_decimal a = {{25, 0, 0, 0x00010000}};  // 2.5
    s21_decimal b;
    s21_from_int_to_decimal(3, &b);
    s21_decimal product, result;

    s21_mul(a, b, &product);
    s21_round(product, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 8);
}
END_TEST

// Truncate after sub: truncate(10.9 - 0.5) = truncate(10.4) = 10
START_TEST(complex_truncate_after_sub) {
    s21_decimal a = {{109, 0, 0, 0x00010000}};  // 10.9
    s21_decimal b = {{5, 0, 0, 0x00010000}};    // 0.5
    s21_decimal diff, result;

    s21_sub(a, b, &diff);
    s21_truncate(diff, &result);

    int out = 0;
    s21_from_decimal_to_int(result, &out);
    ck_assert_int_eq(out, 10);
}
END_TEST

// Large values: MAX_INT + MAX_INT via decimal
START_TEST(complex_add_large_ints) {
    s21_decimal a, b, result;
    s21_from_int_to_decimal(INT_MAX, &a);
    s21_from_int_to_decimal(INT_MAX, &b);

    int code = s21_add(a, b, &result);
    ck_assert_int_eq(code, OK);

    // 2147483647 + 2147483647 = 4294967294 — doesn't fit in int
    float out = 0.0f;
    s21_from_decimal_to_float(result, &out);
    ck_assert(out > 4.29e9f);
}
END_TEST

// Negate zero stays zero (comparison-wise)
START_TEST(complex_negate_zero) {
    s21_decimal zero, neg_zero;
    s21_from_int_to_decimal(0, &zero);

    s21_negate(zero, &neg_zero);

    ck_assert_int_eq(s21_is_equal(zero, neg_zero), 1);
}
END_TEST

Suite *s21_complex_suite(void) {
    Suite *s = suite_create("complex");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, complex_int_add_roundtrip);
    tcase_add_test(tc_core, complex_int_sub_roundtrip);
    tcase_add_test(tc_core, complex_int_mul_roundtrip);
    tcase_add_test(tc_core, complex_add_sub_identity);
    tcase_add_test(tc_core, complex_distributive_law);
    tcase_add_test(tc_core, complex_negate_double);
    tcase_add_test(tc_core, complex_negate_add_equals_sub);
    tcase_add_test(tc_core, complex_float_negate_roundtrip);
    tcase_add_test(tc_core, complex_add_zero_identity);
    tcase_add_test(tc_core, complex_mul_one_identity);
    tcase_add_test(tc_core, complex_negative_add_chain);
    tcase_add_test(tc_core, complex_mul_scale_to_float);
    tcase_add_test(tc_core, complex_comparison_after_add);
    tcase_add_test(tc_core, complex_sub_self_is_zero);
    tcase_add_test(tc_core, complex_neg_mul_equivalence);
    tcase_add_test(tc_core, complex_floor_after_add);
    tcase_add_test(tc_core, complex_round_after_mul);
    tcase_add_test(tc_core, complex_truncate_after_sub);
    tcase_add_test(tc_core, complex_add_large_ints);
    tcase_add_test(tc_core, complex_negate_zero);

    suite_add_tcase(s, tc_core);
    return s;
}
