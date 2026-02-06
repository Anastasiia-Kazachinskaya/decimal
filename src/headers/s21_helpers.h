#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "s21_decimal.h"

typedef struct {
  unsigned bits[8];
} s21_big_decimal;

s21_big_decimal s21_decimal_to_big(s21_decimal value);               // TODO
s21_decimal s21_big_to_decimal(s21_big_decimal value);               // TODO
int s21_get_sign(s21_decimal value);                                 // TODO
int s21_set_sign(s21_decimal* value, int sign);                      // TODO
int s21_get_scale(s21_decimal value);                                // TODO
int s21_set_scale(s21_decimal* value, int scale);                    // TODO
int s21_get_bit(s21_decimal value, int bit_index);                   // TODO
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value);   // TODO
int s21_is_zero(s21_decimal value);                                  // UNDER REVIEW
int s21_null_decimal(s21_decimal value);                             // TODO
int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2);  // TODO
int s21_shift_left(s21_big_decimal* value, int shift);               // TODO
int s21_shift_right(s21_big_decimal* value, int shift);              // TODO

void s21_round_big_decimal(s21_big_decimal* value, int target_scale);  // TODO

#endif
