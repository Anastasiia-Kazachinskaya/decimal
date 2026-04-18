#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include <stdint.h>

#include "../s21_decimal.h"

#define S21_BIG_DECIMAL_SIZE 8
#define ERROR 1

#define OK 0

typedef struct {
  unsigned bits[8];
  int sign;
  int scale;
} s21_big_decimal;

s21_big_decimal* s21_decimal_to_big(s21_decimal* value);

int s21_big_add(s21_big_decimal* value_1, s21_big_decimal* value_2,
                s21_big_decimal* result);
int s21_big_sub(s21_big_decimal* value_1, s21_big_decimal* value_2,
                s21_big_decimal* result);
int s21_big_mul(s21_big_decimal* value_1, s21_big_decimal* value_2,
                s21_big_decimal* result);

int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2);
int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2);

int big_div_mantissa(s21_big_decimal* dividend, s21_big_decimal* divisor,
                     s21_big_decimal* quotient, int* out_scale);
void big_bankers_round(s21_big_decimal* quotient, s21_big_decimal remainder,
                       s21_big_decimal divisor);
void big_add(s21_big_decimal* a, s21_big_decimal b);
int big_inc(s21_big_decimal* v);
int big_mul_ten(s21_big_decimal* v);
void big_sub(s21_big_decimal* a, s21_big_decimal b);

int big_gte(const s21_big_decimal* a, const s21_big_decimal* b);

#endif