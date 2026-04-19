#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include <stdint.h>

#include "../s21_decimal.h"

#define MASK_32_BIT 0xFFFFFFFF

#define ERROR 1
#define OK 0

typedef struct {
  unsigned bits[8];
  int sign;
  int scale;
} s21_big_decimal;

#define S21_BIG_DECIMAL_SIZE 8
#define UNSIGNED_SIZE (sizeof(unsigned) * 8)
#define SCALE (int)((S21_BIG_DECIMAL_SIZE - 1) * sizeof(unsigned))

#define RANDOM_FLOAT_1 4294967296.0
#define RANDOM_FLOAT_2 18446744073709551616.0

#define RANDOM_LONG_1 2147483648U
#define RANDOM_LONG_2 2147483647U

s21_big_decimal* s21_decimal_to_big(s21_decimal* value);

int s21_big_add(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result);
int s21_big_sub(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result);
int s21_big_mul(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result);

int s21_is_big_greater(const s21_big_decimal* value1,
                       const s21_big_decimal* value2);
int s21_is_big_less(const s21_big_decimal* value1,
                    const s21_big_decimal* value2);

int big_div_mantissa(const s21_big_decimal* dividend,
                     const s21_big_decimal* divisor, s21_big_decimal* quotient,
                     int* out_scale);
void big_bankers_round(s21_big_decimal* quotient, s21_big_decimal remainder,
                       s21_big_decimal divisor);
int big_inc(s21_big_decimal* v);
int big_mul_ten(s21_big_decimal* v);

int big_gte(const s21_big_decimal* a, const s21_big_decimal* b);
void big_shift_left1(s21_big_decimal* v);

int check_overflow(const s21_big_decimal* value);

int big_is_zero(const s21_big_decimal* v);
int big_normalize(s21_big_decimal* v);
int s21_big_perform_signed_operation(int sign_1, int sign_2,
                                     s21_big_decimal* big_value_1,
                                     s21_big_decimal* big_value_2,
                                     s21_big_decimal* res_big);

int check_mantissa(const s21_big_decimal* res_big);
int s21_handle_overflow_and_rounding(s21_big_decimal* res_big);

int big_to_decimal(s21_big_decimal v, s21_decimal* result);
s21_big_decimal decimal_to_big(s21_decimal v);
int s21_normalize_big_pair(s21_big_decimal* value_1, s21_big_decimal* value_2);
int s21_multiply_big_by_10(s21_big_decimal* value);
int s21_normalize_and_check_overflow(s21_big_decimal* value);
s21_big_decimal s21_decimal_to_big_internal(s21_decimal* value);
s21_decimal s21_big_to_decimal_internal(s21_big_decimal* big_result,
                                        s21_decimal* result);
void s21_null_big_decimal(s21_big_decimal* value);
void big_zero(s21_big_decimal* v);

#endif