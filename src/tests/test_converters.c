#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/s21_helpers.h"

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

//decimal_to_int
// Тест 1: Конвертация положительного числа decimal → int
START_TEST(test_positive_decimal_to_int) {
    int result;
    s21_decimal src;
    s21_from_int_to_decimal(12345, &src);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 12345);
}
END_TEST

// Тест 2: Конвертация отрицательного числа decimal → int
START_TEST(test_negative_decimal_to_int) {
    int result;
    s21_decimal src;
    s21_from_int_to_decimal(-12345, &src);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, -12345);
}
END_TEST

// Тест 3: Конвертация нуля
START_TEST(test_zero_decimal_to_int) {
    int result = 42;
    s21_decimal src;
    s21_from_int_to_decimal(0, &src);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 0);
}
END_TEST

// Тест 4: Конвертация INT_MAX
START_TEST(test_max_int) {
    int result;
    s21_decimal src;
    s21_from_int_to_decimal(INT_MAX, &src);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, INT_MAX);
}
END_TEST

// Тест 5: Конвертация INT_MIN
START_TEST(test_min_int_reverse) {

    int result = 9999; 
    s21_decimal src;
    int create_status = s21_from_int_to_decimal(INT_MIN, &src);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(create_status, OK);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, INT_MIN);
}
END_TEST

// Тест 6: Конвертация с масштабом (scale) - десятичная дробь
START_TEST(test_decimal_with_scale) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 12345;
    src.bits[3] = 2 << 16;
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 123);
}
END_TEST

// Тест 7: Конвертация отрицательного числа с масштабом
START_TEST(test_negative_decimal_with_scale) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 45678;
    src.bits[3] = (2 << 16) | (1u << 31);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, -456);
}
END_TEST

// Тест 8: Число с масштабом, которое делится нацело
START_TEST(test_decimal_with_scale_exact) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 4500;
    src.bits[3] = 1 << 16;    
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 450);
}
END_TEST

// Тест 9: Число больше INT_MAX - должна быть ошибка
START_TEST(test_overflow_positive) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 3000000000U;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    int status = s21_from_decimal_to_int(src, &result); 
    ck_assert_int_eq(status, CONVERTATION_ERROR);
}
END_TEST

// Тест 10: Число меньше INT_MIN - должна быть ошибка
START_TEST(test_overflow_negative) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 3000000000U;
    src.bits[3] = 1u << 31;    
    int status = s21_from_decimal_to_int(src, &result);    
    ck_assert_int_eq(status, CONVERTATION_ERROR);
}
END_TEST

// Тест 11: Слишком большой decimal (использует bits[2])
START_TEST(test_too_large_decimal) {
    int result = 42;
    s21_decimal src = {0};
    src.bits[2] = 1;
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, CONVERTATION_ERROR);
    ck_assert_int_eq(result, 0);
}
END_TEST

// Тест 12: Нулевой указатель dst
START_TEST(test_null_pointer_reverse) {
    s21_decimal src;
    s21_from_int_to_decimal(42, &src);
    int status = s21_from_decimal_to_int(src, NULL);
    ck_assert_int_eq(status, CONVERTATION_ERROR);
}
END_TEST

// Тест 13: Большое число с масштабом, которое становится маленьким после деления
START_TEST(test_large_with_scale_reduces) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 1234567899U;
    src.bits[3] = 1 << 16;    
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 123456789);
}
END_TEST

// Тест 14: Округление в меньшую сторону для положительных
START_TEST(test_rounding_positive) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 123999;
    src.bits[3] = 3 << 16;
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 123);
}
END_TEST

// Тест 15: Округление в меньшую сторону для отрицательных
START_TEST(test_rounding_negative) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 456999;
    src.bits[3] = (3 << 16) | (1u << 31);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, -456);
}
END_TEST

// Тест 16: Число, которое после деления на scale становится 0
START_TEST(test_becomes_zero) {
    int result = 42;
    s21_decimal src = {0};
    src.bits[0] = 999;
    src.bits[3] = 3 << 16;
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 0);
}
END_TEST

// Тест 17: Проверка, что dst обнуляется при ошибке
START_TEST(test_dst_zeroed_on_error) {
    int result = 777;
    s21_decimal src = {0};
    src.bits[2] = 1;    
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, CONVERTATION_ERROR);
    ck_assert_int_eq(result, 0);
}
END_TEST

// Тест 18: Граничное значение
START_TEST(test_exact_max_int_with_scale) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 2147483647U;
    src.bits[3] = 1 << 16;    
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, 214748364);
}
END_TEST

// Тест 19: Граничное значение
START_TEST(test_exact_min_int_with_scale) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 2147483648U;
    src.bits[3] = (1 << 16) | (1u << 31);
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, OK);
    ck_assert_int_eq(result, -214748364);
}
END_TEST

// Тест 20: Использование bits[1]
START_TEST(test_uses_bits1) {
    int result;
    s21_decimal src = {0};
    src.bits[0] = 5000000000U & 0xFFFFFFFFU;
    src.bits[1] = 5000000000U >> 32;
    src.bits[3] = 0;    
    int status = s21_from_decimal_to_int(src, &result);
    ck_assert_int_eq(status, CONVERTATION_ERROR);
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

     // Основные тесты
    tcase_add_test(tc_core, test_positive_decimal_to_int);
    tcase_add_test(tc_core, test_negative_decimal_to_int);
    tcase_add_test(tc_core, test_zero_decimal_to_int);
    tcase_add_test(tc_core, test_max_int);
    tcase_add_test(tc_core, test_min_int_reverse);
    tcase_add_test(tc_core, test_null_pointer_reverse);
    tcase_add_test(tc_core, test_dst_zeroed_on_error);
    
    // Тесты с масштабом
    tcase_add_test(tc_core, test_decimal_with_scale);
    tcase_add_test(tc_core, test_negative_decimal_with_scale);
    tcase_add_test(tc_core, test_decimal_with_scale_exact);
    tcase_add_test(tc_core, test_large_with_scale_reduces);
    tcase_add_test(tc_core, test_rounding_positive);
    tcase_add_test(tc_core, test_rounding_negative);
    tcase_add_test(tc_core, test_becomes_zero);
    tcase_add_test(tc_core, test_exact_max_int_with_scale);
    tcase_add_test(tc_core, test_exact_min_int_with_scale);
    
    // Тесты ошибок
    tcase_add_test(tc_core, test_overflow_positive);
    tcase_add_test(tc_core, test_overflow_negative);
    tcase_add_test(tc_core, test_too_large_decimal);
    tcase_add_test(tc_core, test_uses_bits1);

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
    
    suite_add_tcase(s, tc_core);
    
    return s;
}