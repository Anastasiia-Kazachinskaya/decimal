#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include "../s21_decimal.h"

#define S21_BIG_DECIMAL_SIZE 7
#define ERROR 1;
#define OK 0

typedef struct {
  unsigned bits[8];
  int sign;
  int scale;
} s21_big_decimal;

s21_big_decimal* s21_decimal_to_big(s21_decimal* value);  // DO
s21_decimal s21_big_to_decimal(s21_big_decimal* value);  // TODO
s21_big_decimal* s21_big_zero();

int get_scale(s21_big_decimal* value);

void s21_div_ten(s21_big_decimal* value);

void s21_mul_ten(s21_big_decimal* value);

int s21_big_decimal_add(s21_big_decimal* value_1, s21_big_decimal* value_2,
            s21_big_decimal* result);  // TODO
int s21_big_decimal_sub(s21_big_decimal* value_1, s21_big_decimal* value_2,
            s21_big_decimal* result);
int s21_big_decimal_mul(s21_big_decimal* value_1, s21_big_decimal* value_2,
            s21_big_decimal* result);
int s21_big_decimal_div(s21_big_decimal* value_1, s21_big_decimal* value_2,
            s21_big_decimal* result);

int big_get_bit(s21_big_decimal* value, int index);

int big_set_bit(s21_big_decimal* value, int index, char bit);

int s21_big_add_internal(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result);
int s21_big_sub_internal(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result);

#endif