#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/s21_helpers.h"

#ifndef FLOAT_EPS
#define FLOAT_EPS 1e-6
#endif

// int_to_decimal
// Тест 1: Конвертация положительных чисел
START_TEST(int_to_decimal_test_positive_int) {
    s21_decimal result;
    int num = 12345;
    
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 12345);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 0);
}
END_TEST

// Тест 2: Конвертация отрицательных чисел
START_TEST(int_to_decimal_test_negative_int) {
    s21_decimal result;
    int num = -12345;
    
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 12345);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

// Тест 3: Конвертация нуля
START_TEST(int_to_decimal_test_zero) {
    s21_decimal result;
    int num = 0;
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 0);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_uint_eq(result.bits[3], 0);
}
END_TEST

// Тест 4: Конвертация минимального int
START_TEST(int_to_decimal_test_min_int) {
    s21_decimal result;
    memset(&result, 0, sizeof(result));
    
    int num = INT_MIN;
    int status = s21_from_int_to_decimal(num, &result);
    
    ck_assert_int_eq(status, OK);

    unsigned int actual = result.bits[0];
    unsigned int expected = 0x80000000U;
    
    ck_assert_msg(actual == expected,
                  "Expected bits[0] = 0x%08x (%u), got 0x%08x (%llu)",
                  expected, expected, actual, (unsigned long long)actual);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

// Тест 5: Конвертация максимального int
START_TEST(int_to_decimal_test_max_int) {
    s21_decimal result;
    int num = INT_MAX;
    
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 2147483647);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 0);
}
END_TEST

// Тест 6: Конвертация числа 1
START_TEST(int_to_decimal_test_one) {
    s21_decimal result;
    int num = 1;
    
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 1);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 0);
}
END_TEST

// Тест 7: Конвертация числа -1
START_TEST(int_to_decimal_test_minus_one) {
    s21_decimal result;
    int num = -1;
    
    int status = s21_from_int_to_decimal(num, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 1);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

// Тест 8: Конвертация с нулевым указателем
START_TEST(int_to_decimal_test_null_pointer) {
    int num = 42;
    int status = s21_from_int_to_decimal(num, NULL);
    ck_assert_int_eq(status, CONVERTATION_ERROR);
}
END_TEST

// Тест 9: Конвертация граничных значений
START_TEST(int_to_decimal_test_boundary_values) {
    s21_decimal result;

    int status = s21_from_int_to_decimal(-2, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 2);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);

    status = s21_from_int_to_decimal(2, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_uint_eq(result.bits[0], 2);
    ck_assert_int_eq((result.bits[3] >> 31) & 1, 0);
}
END_TEST

// Тест 10: Конвертация произвольных чисел
START_TEST(int_to_decimal_test_random_numbers) {
    s21_decimal result;
    
    int tests[] = {100, -100, 999, -999, 1234567, -1234567};
    
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        int num = tests[i];
        int expected_value = (num < 0) ? -num : num;
        
        int status = s21_from_int_to_decimal(num, &result);
        ck_assert_int_eq(status, OK);
        ck_assert_uint_eq(result.bits[0], expected_value);
        ck_assert_uint_eq(result.bits[1], 0);
        ck_assert_uint_eq(result.bits[2], 0);
        
        int expected_sign = (num < 0) ? 1 : 0;
        ck_assert_int_eq((result.bits[3] >> 31) & 1, expected_sign);
    }
}
END_TEST

// Тест 11: Проверка, что функция обнуляет decimal перед записью
START_TEST(int_to_decimal_test_clean_before_write) {
    s21_decimal result;

    result.bits[0] = 0xFFFFFFFF;
    result.bits[1] = 0xFFFFFFFF;
    result.bits[2] = 0xFFFFFFFF;
    result.bits[3] = 0xFFFFFFFF;
    
    int status = s21_from_int_to_decimal(42, &result);
    ck_assert_int_eq(status, OK);

    ck_assert_uint_eq(result.bits[0], 42);
    ck_assert_uint_eq(result.bits[1], 0);
    ck_assert_uint_eq(result.bits[2], 0);
    ck_assert_uint_eq(result.bits[3] & 0x7FFFFFFF, 0);
}
END_TEST

// Тест 12: Конвертация максимального положительного до INT_MAX
START_TEST(int_to_decimal_test_large_positive) {
    s21_decimal result;
    
    for (int num = 1000000; num <= 1000005; num++) {
        int status = s21_from_int_to_decimal(num, &result);
        ck_assert_int_eq(status, OK);
        ck_assert_uint_eq(result.bits[0], num);
        ck_assert_int_eq((result.bits[3] >> 31) & 1, 0);
    }
}
END_TEST

// Тест 13: Конвертация максимального отрицательного до INT_MIN+1
START_TEST(int_to_decimal_test_large_negative) {
    s21_decimal result;
    
    for (int num = -1000005; num >= -1000000; num++) {
        int status = s21_from_int_to_decimal(num, &result);
        ck_assert_int_eq(status, OK);
        ck_assert_uint_eq(result.bits[0], -num);
        ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
    }
}
END_TEST

//decimal_to_big_decimal
// Тест 1: Конвертация положительного decimal в big_decimal
START_TEST(test_positive_decimal_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 123456789U;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 123456789U);
    ck_assert_uint_eq(result.bits[1], 0U);
    ck_assert_uint_eq(result.bits[2], 0U);
    ck_assert_uint_eq(result.bits[3], 0U);
    ck_assert_uint_eq(result.bits[4], 0U);
    ck_assert_uint_eq(result.bits[5], 0U);
    ck_assert_int_eq(result.sign, 0);
    ck_assert_int_eq(result.scale, 0);
}
END_TEST

// Тест 2: Конвертация отрицательного decimal в big_decimal
START_TEST(test_negative_decimal_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 123456789U;
    src.bits[3] = 1u << 31;
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 123456789U);
    ck_assert_int_eq(result.sign, 1);
    ck_assert_int_eq(result.scale, 0);
}
END_TEST

// Тест 3: Конвертация decimal с масштабом (scale)
START_TEST(test_decimal_with_scale_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 123456789U;
    src.bits[3] = 3 << 16;
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 123456789U);
    ck_assert_int_eq(result.sign, 0);
    ck_assert_int_eq(result.scale, 3);
}
END_TEST

// Тест 4: Конвертация decimal с отрицательным знаком и масштабом
START_TEST(test_negative_with_scale_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 987654321U;
    src.bits[3] = (5 << 16) | (1u << 31);
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 987654321U);
    ck_assert_int_eq(result.sign, 1);
    ck_assert_int_eq(result.scale, 5);
}
END_TEST

// Тест 5: Конвертация decimal с заполненными bits[0], bits[1], bits[2]
START_TEST(test_full_decimal_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 0xFFFFFFFFU;
    src.bits[1] = 0xAAAAAAAAU;
    src.bits[2] = 0x55555555U;
    src.bits[3] = (2 << 16) | (1u << 31);
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 0xFFFFFFFFU);
    ck_assert_uint_eq(result.bits[1], 0xAAAAAAAAU);
    ck_assert_uint_eq(result.bits[2], 0x55555555U);
    ck_assert_uint_eq(result.bits[3], 0U);
    ck_assert_uint_eq(result.bits[4], 0U);
    ck_assert_uint_eq(result.bits[5], 0U);
    ck_assert_int_eq(result.sign, 1);
    ck_assert_int_eq(result.scale, 2);
}
END_TEST

// Тест 6: Конвертация нулевого decimal
START_TEST(test_zero_decimal_to_big) {
    s21_decimal src = {0};
    s21_big_decimal result = s21_decimal_to_big(&src);
    for (int i = 0; i < 6; i++) {
        ck_assert_uint_eq(result.bits[i], 0U);
    }
    ck_assert_int_eq(result.sign, 0);
    ck_assert_int_eq(result.scale, 0);
}
END_TEST

// Тест 7: Конвертация с нулевым указателем (должна вернуть нулевой big_decimal)
START_TEST(test_null_pointer_to_big) {
    s21_big_decimal result = s21_decimal_to_big(NULL);
    for (int i = 0; i < 6; i++) {
        ck_assert_uint_eq(result.bits[i], 0U);
    }
    ck_assert_int_eq(result.sign, 0);
    ck_assert_int_eq(result.scale, 0);
}
END_TEST

// Тест 8: Конвертация максимального положительного decimal
START_TEST(test_max_positive_decimal_to_big) {
    s21_decimal src = {0};
    src.bits[0] = 0xFFFFFFFFU;
    src.bits[1] = 0xFFFFFFFFU;
    src.bits[2] = 0xFFFFFFFFU;
    src.bits[3] = 0;
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 0xFFFFFFFFU);
    ck_assert_uint_eq(result.bits[1], 0xFFFFFFFFU);
    ck_assert_uint_eq(result.bits[2], 0xFFFFFFFFU);
    ck_assert_uint_eq(result.bits[3], 0U);
    ck_assert_uint_eq(result.bits[4], 0U);
    ck_assert_uint_eq(result.bits[5], 0U);
    ck_assert_int_eq(result.sign, 0);
    ck_assert_int_eq(result.scale, 0);
}
END_TEST

// Тест 9: Проверка, что остальные биты big_decimal обнуляются
START_TEST(test_clean_high_bits_in_big) {
    s21_decimal src = {0};
    src.bits[0] = 12345U;
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 12345U);
    ck_assert_uint_eq(result.bits[1], 0U);
    ck_assert_uint_eq(result.bits[2], 0U);
    ck_assert_uint_eq(result.bits[3], 0U);
    ck_assert_uint_eq(result.bits[4], 0U);
    ck_assert_uint_eq(result.bits[5], 0U);
}
END_TEST

// Тест 10: Конвертация с разными значениями scale
START_TEST(test_various_scales_to_big) {
    for (int scale = 0; scale <= 28; scale++) {
        s21_decimal src = {0};
        src.bits[0] = 1000U;
        src.bits[3] = scale << 16;
        s21_big_decimal result = s21_decimal_to_big(&src);
        ck_assert_uint_eq(result.bits[0], 1000U);
        ck_assert_int_eq(result.scale, scale);
        ck_assert_int_eq(result.sign, 0);
    }
}
END_TEST

// Тест 11: Конвертация пограничных значений
START_TEST(test_boundary_values_to_big) {
    {
        s21_decimal src = {0};
        src.bits[0] = 1U;
        s21_big_decimal result = s21_decimal_to_big(&src);
        ck_assert_uint_eq(result.bits[0], 1U);
        ck_assert_int_eq(result.sign, 0);
    }

    {
        s21_decimal src = {0};
        src.bits[2] = 0x80000000U;
        s21_big_decimal result = s21_decimal_to_big(&src);
        ck_assert_uint_eq(result.bits[2], 0x80000000U);
        ck_assert_uint_eq(result.bits[0], 0U);
        ck_assert_uint_eq(result.bits[1], 0U);
    }
}
END_TEST

// Тест 12: Проверка структуры big_decimal после конвертации
START_TEST(test_big_decimal_structure) {
    s21_decimal src = {0};
    src.bits[0] = 0x12345678U;
    src.bits[1] = 0x9ABCDEF0U;
    src.bits[2] = 0x0FEDCBA9U;
    src.bits[3] = (7 << 16) | (1u << 31);
    s21_big_decimal result = s21_decimal_to_big(&src);
    ck_assert_uint_eq(result.bits[0], 0x12345678U);
    ck_assert_uint_eq(result.bits[1], 0x9ABCDEF0U);
    ck_assert_uint_eq(result.bits[2], 0x0FEDCBA9U);
    ck_assert_uint_eq(result.bits[3], 0U);
    ck_assert_uint_eq(result.bits[4], 0U);
    ck_assert_uint_eq(result.bits[5], 0U);
    ck_assert_int_eq(result.sign, 1);
    ck_assert_int_eq(result.scale, 7);
}
END_TEST

//decimal_to_float
START_TEST(test_from_decimal_to_float_null_pointer) {
    float dst;
    s21_decimal zero = {{0, 0, 0, 0}};
    
    int res = s21_from_decimal_to_float(zero, NULL);
    ck_assert_int_eq(res, CONVERTATION_ERROR);
    
    res = s21_from_decimal_to_float(zero, &dst);
    ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(test_from_decimal_to_float_invalid_decimal) {
    s21_decimal invalid = {{0, 0, 0, 0x00FF0000}};
    float dst;
    int res = s21_from_decimal_to_float(invalid, &dst);
    ck_assert_int_eq(res, CONVERTATION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_zero) {
    s21_decimal zero = {{0, 0, 0, 0}};
    float dst = 123.456f;
    int res = s21_from_decimal_to_float(zero, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(fabs(dst - 0.0f) < FLOAT_EPS);
}
END_TEST

START_TEST(test_from_decimal_to_float_simple_integers) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 1;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 1.0f, 1e-6);

    value.bits[0] = 42;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 42.0f, 1e-6);

    value.bits[0] = 123;
    value.bits[3] |= 1u << 31;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, -123.0f, 1e-6);
}
END_TEST

START_TEST(test_from_decimal_to_float_with_scale) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;
    value.bits[0] = 123;
    value.bits[3] |= (2 << 16);
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 1.23f, FLOAT_EPS);
    value.bits[0] = 314159;
    value.bits[3] = 0;
    value.bits[3] |= (5 << 16);
    value.bits[3] |= 1u << 31;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, -3.14159f, FLOAT_EPS);
}
END_TEST

// Тест 6: Числа, использующие bits[1]
START_TEST(test_from_decimal_to_float_using_bits1) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;
    value.bits[1] = 1;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 4294967296.0f, FLOAT_EPS * 1e9);
}
END_TEST

// Тест 7: Числа, использующие bits[2]
START_TEST(test_from_decimal_to_float_using_bits2) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;
    value.bits[2] = 1;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(fabs(dst - 1.8446744e19f) < FLOAT_EPS * 1e19);
}
END_TEST

// Тест 8: Максимальное представимое значение
START_TEST(test_from_decimal_to_float_max) {
    s21_decimal value = {{
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0
    }};
    float dst;
    int res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(dst < FLT_MAX);
}
END_TEST

// Тест 9: Проверка на переполнение
START_TEST(test_from_decimal_to_float_max_decimal) {
    s21_decimal value;
    value.bits[0] = 0xFFFFFFFF;
    value.bits[1] = 0xFFFFFFFF;
    value.bits[2] = 0xFFFFFFFF;
    value.bits[3] = 0;
    
    float dst = -999.999f;
    int res = s21_from_decimal_to_float(value, &dst);
    
    ck_assert_int_eq(res, OK);
    ck_assert(dst > 0);
}
END_TEST

// Тест 10: Проверка минимального положительного значения
START_TEST(test_from_decimal_to_float_min_positive) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 1;
    value.bits[3] |= (28 << 16);
    
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(dst > 0);
    ck_assert(dst < 1e-27f);
}
END_TEST

// Тест 11: Проверка очень больших чисел 
START_TEST(test_from_decimal_to_float_very_large) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;
    
    value.bits[0] = 0xFFFFFFFF;
    value.bits[1] = 0xFFFFFFFF;
    value.bits[2] = 0xFFFFFFF0;
    
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(isfinite(dst));
}
END_TEST

// Тест 10: Проверка очень маленьких чисел
START_TEST(test_from_decimal_to_float_very_small) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 1;
    value.bits[3] |= (28 << 16);
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert(dst > 0.0f);
    ck_assert(dst < 1e-27f);
}
END_TEST

// Тест 11: Проверка знака и масштаба одновременно
START_TEST(test_from_decimal_to_float_sign_and_scale) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 123;
    value.bits[3] = 0;
    value.bits[3] |= (5 << 16);
    value.bits[3] |= 1u << 31;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, -0.00123f, FLOAT_EPS);
}
END_TEST

// Тест 12: Проверка граничных значений масштаба
START_TEST(test_from_decimal_to_float_scale_boundaries) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 12345;
    value.bits[3] = 0;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 12345.0f, FLOAT_EPS);

    value.bits[0] = 12345;
    value.bits[3] = 0;
    value.bits[3] |= (28 << 16);
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 1.2345e-24f, FLOAT_EPS * 1e-24);
}
END_TEST

// Тест 13: Комбинированный тест с разными масштабами и знаками
START_TEST(test_from_decimal_to_float_combined) {
    s21_decimal value = {{0, 0, 0, 0}};
    float dst;
    int res;

    value.bits[0] = 123456;
    value.bits[3] = 0;
    value.bits[3] |= (3 << 16);
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, 123.456f, FLOAT_EPS);

    value.bits[0] = 987654321;
    value.bits[3] = 0;
    value.bits[3] |= (4 << 16);
    value.bits[3] |= 1u << 31;
    res = s21_from_decimal_to_float(value, &dst);
    ck_assert_int_eq(res, OK);
    ck_assert_float_eq_tol(dst, -98765.4321f, FLOAT_EPS * 1000);
}
END_TEST

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


    tcase_add_test(tc_core, test_positive_decimal_to_big);
    tcase_add_test(tc_core, test_negative_decimal_to_big);
    tcase_add_test(tc_core, test_decimal_with_scale_to_big);
    tcase_add_test(tc_core, test_negative_with_scale_to_big);
    tcase_add_test(tc_core, test_full_decimal_to_big);
    tcase_add_test(tc_core, test_zero_decimal_to_big);
    tcase_add_test(tc_core, test_null_pointer_to_big);
    tcase_add_test(tc_core, test_max_positive_decimal_to_big);
    tcase_add_test(tc_core, test_clean_high_bits_in_big);
    tcase_add_test(tc_core, test_various_scales_to_big);
    tcase_add_test(tc_core, test_boundary_values_to_big);
    tcase_add_test(tc_core, test_big_decimal_structure);
    
    //decimal_to_float
    tcase_add_test(tc_core, test_from_decimal_to_float_null_pointer);
    tcase_add_test(tc_core, test_from_decimal_to_float_invalid_decimal);
    tcase_add_test(tc_core, test_from_decimal_to_float_zero);
    tcase_add_test(tc_core, test_from_decimal_to_float_simple_integers);
    tcase_add_test(tc_core, test_from_decimal_to_float_with_scale);
    tcase_add_test(tc_core, test_from_decimal_to_float_using_bits1);
    tcase_add_test(tc_core, test_from_decimal_to_float_using_bits2);
    tcase_add_test(tc_core, test_from_decimal_to_float_max);
    tcase_add_test(tc_core, test_from_decimal_to_float_max_decimal);
    tcase_add_test(tc_core, test_from_decimal_to_float_min_positive);
    tcase_add_test(tc_core, test_from_decimal_to_float_very_large);
    tcase_add_test(tc_core, test_from_decimal_to_float_very_small);
    tcase_add_test(tc_core, test_from_decimal_to_float_sign_and_scale);
    tcase_add_test(tc_core, test_from_decimal_to_float_scale_boundaries);
    tcase_add_test(tc_core, test_from_decimal_to_float_combined);
    
    
    

    suite_add_tcase(s, tc_core);
    
    return s;
}