#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include "../s21_decimal.h"

typedef struct {
  unsigned bits[7];
  int sign;
  int scale;
} s21_big_decimal;

s21_big_decimal s21_decimal_to_big(s21_decimal value);  // DO
s21_decimal s21_big_to_decimal(s21_big_decimal value);  // TODO

int s21_set_sign(s21_big_decimal* val, s21_decimal* value);  // DO

int s21_shift_left(s21_big_decimal* value, int shift);   // TODO
int s21_shift_right(s21_big_decimal* value, int shift);  // TODO

void s21_round_big_decimal(s21_big_decimal* value, int target_scale);  // TODO

#endif