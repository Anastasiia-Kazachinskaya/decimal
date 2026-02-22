#ifndef S21_UTIL_H
#define S21_UTIL_H

#include "../s21_decimal.h"

int s21_get_sign(s21_decimal* value);              // DO
int s21_get_scale(s21_decimal* value);             // TODO
int s21_set_scale(s21_decimal* value, int scale);  // TODO

int s21_get_bit(s21_decimal value, int bit_index);                  // TODO
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value);  // TODO
int s21_is_zero(s21_decimal value);                                 // DONE

void s21_null_decimal(s21_decimal* value);                           // TODO
int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2);  // TODO

int s21_from_decimal_to_float(s21_decimal src, float* dst);  // TODO
int s21_from_decimal_to_int(s21_decimal src, int* dst);      // DONE
int s21_from_float_to_decimal(float src, s21_decimal* dst);  // TODO
int s21_from_int_to_decimal(int src, s21_decimal* dst);      // DONE

#endif
