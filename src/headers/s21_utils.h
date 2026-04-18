#ifndef S21_UTIL_H
#define S21_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../s21_decimal.h"
#include "s21_big_decimal.h"

#define MAX4BITE 0xffffffff  // исп в other.c
#define S21_DECIMAL_LIMIT 3  // исп в other.c

#define FLOAT_DIGITS 7

int s21_get_sign(s21_decimal* value);              // DO
int s21_get_scale(s21_decimal* value);             // TODO
void s21_set_sign(char sign, s21_decimal* value);  // DO

int s21_get_bit(s21_decimal value, int bit_index);                  // TODO
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value);  // TODO
int s21_is_zero(s21_decimal value);                                 // DONE

void s21_null_decimal(s21_decimal* value);                           // TODO
int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2);  // TODO

int s21_from_decimal_to_float(s21_decimal src, float* dst);  // TODO
int s21_from_decimal_to_int(s21_decimal src, int* dst);      // DONE
int s21_from_float_to_decimal(float src, s21_decimal* dst);  // TODO
int s21_from_int_to_decimal(int src, s21_decimal* dst);      // DONE

int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2);
s21_big_decimal s21_decimal_to_big_internal(s21_decimal* value);
s21_decimal s21_big_to_decimal_internal(s21_big_decimal* big_result,
                                        s21_decimal* result);
void s21_null_big_decimal(s21_big_decimal* value);
int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2);
int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2);
int s21_divide_mantissa_by_10(s21_decimal* value);
int s21_normalize_and_check_overflow(s21_big_decimal* value);
void s21_set_sign_internal(s21_decimal* result, int sign);
int s21_normalize_big_pair(s21_big_decimal* value_1, s21_big_decimal* value_2);
int s21_multiply_big_by_10(s21_big_decimal* value);

int s21_decimal_check(s21_decimal value);
int set_precision(float src);
void apply_exponent(char* e_pos, long long* mantissa, int* exponent,
                    int* digits_after_dot, char* str);
int scale_upper(int* scale, long long* mantissa, s21_decimal* dst);
int scale_lower(int* scale, long long* mantissa);
int float_sign(float* src);
int float_or_dec_error(float src, s21_decimal* dst);
int s21_str_to_int(const char* str);
long long s21_str_to_ll(const char* str);
int s21_big_perform_signed_operation(int sign_1, int sign_2,
                                     s21_big_decimal* big_value_1,
                                     s21_big_decimal* big_value_2,
                                     s21_big_decimal* res_big);

int check_overflow(s21_big_decimal value);
int check_mantissa(s21_big_decimal res_big);
int s21_handle_overflow_and_rounding(s21_big_decimal* res_big);

int big_normalize(s21_big_decimal* v);
int big_to_decimal(s21_big_decimal v, s21_decimal* result);
s21_big_decimal decimal_to_big(s21_decimal v);
void big_shift_left1(s21_big_decimal* v);
int big_is_zero(s21_big_decimal v);
void big_zero(s21_big_decimal* v);

#endif
