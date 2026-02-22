#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define OK 0
#define CONVERTATION_ERROR 1
#define NUMNER_TO_LARGE 1
#define NUMNER_TO_SMALL 2
#define DIVISION_BY_ZERO 3

#define TRUE 1
#define FALSE 0

#define MIN_INT -2147483648
#define MAX_INT 2147483647

#define S21_BIG_DECIMAL_DATA_BITS 3

///[][][][0-15/scale(16-23)/24-30/sign]
typedef struct {
  int bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // TODO
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // TODO
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // TODO
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal* result);  // TODO

int s21_is_less(s21_decimal, s21_decimal);           // TODO
int s21_is_less_or_equal(s21_decimal, s21_decimal);  // TODO

int s21_is_greater(s21_decimal, s21_decimal);           // TODO
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // TODO

int s21_is_equal(s21_decimal, s21_decimal);      // TODO
int s21_is_not_equal(s21_decimal, s21_decimal);  // TODO

int s21_from_int_to_decimal(int src, s21_decimal* dst);      // TODO
int s21_from_float_to_decimal(float src, s21_decimal* dst);  // TODO

int s21_from_decimal_to_int(s21_decimal src, int* dst);      // TODO
int s21_from_decimal_to_float(s21_decimal src, float* dst);  // TODO

int s21_floor(s21_decimal value, s21_decimal* result);     // TODO
int s21_round(s21_decimal value, s21_decimal* result);     // TODO
int s21_truncate(s21_decimal value, s21_decimal* result);  // TODO
int s21_negate(s21_decimal value, s21_decimal* result);    // TODO

#endif
