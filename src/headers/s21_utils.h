#ifndef S21_UTIL_H
#define S21_UTIL_H

#include "../s21_decimal.h"
#include "s21_big_decimal.h"

#define MAX4BITE 0xffffffff // исп в other.c
#define S21_DECIMAL_LIMIT 5 // исп в other.c
#define S21_BIG_DECIMAL_SIZE 7 // исп в arithmetic.c

/*
typedef struct {
  unsigned bits[7];
  int sign;
  int scale;
} s21_big_decimal;
*/
int s21_get_sign(s21_decimal* value);              // DO
int s21_get_scale(s21_decimal* value);             // TODO
int s21_set_scale(s21_decimal* value, int scale);  // TODO
void s21_set_sign(char sign, s21_decimal *value);           //DO


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
s21_decimal s21_big_to_decimal_internal(s21_big_decimal* big_result, s21_decimal* result);
void s21_null_big_decimal(s21_big_decimal* value);
int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2);
int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2);
int s21_divide_mantissa_by_10(s21_decimal* value);
int s21_normalize_and_check_overflow(s21_big_decimal* value);
void s21_set_sign_internal(s21_decimal* result, int sign);
int s21_normalize_big_pair(s21_big_decimal* value_1, s21_big_decimal* value_2);
s21_decimal s21_get_zero();

#endif
