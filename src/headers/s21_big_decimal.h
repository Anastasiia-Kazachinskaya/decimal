#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include "../s21_decimal.h"
#include <stdint.h>

#define S21_BIG_DECIMAL_SIZE 8
#define ERROR 1

#define OK 0

typedef struct {
  unsigned bits[8];
  int sign;
  int scale;
} s21_big_decimal;

s21_big_decimal* s21_decimal_to_big(s21_decimal* value);  // DO
s21_decimal s21_big_to_decimal(s21_big_decimal* value);   // TODO
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

int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result);
int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result);
int s21_big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result);

int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2);
int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2);

#endif