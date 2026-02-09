#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <s21_decimal.h>

#define CONVERTATION_ERROR 1
#define OK 0

#define MIN_INT -2147483648
#define MAX_INT 2147483647


typedef struct {
  unsigned bits[7];
  int sign;
  int scale;
} s21_big_decimal;

s21_big_decimal s21_decimal_to_big(s21_decimal value);               // DO
s21_decimal s21_big_to_decimal(s21_big_decimal value);               // TODO
int s21_get_sign(s21_decimal *value);                                 // DO
int s21_set_sign(s21_big_decimal* val, s21_decimal *value);           //DO
int s21_get_scale(s21_decimal *value);                                // TODO
//int s21_set_scale(s21_decimal* value, int scale);                    // TODO
int s21_get_bit(s21_decimal value, int bit_index);                   // TODO
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value);   // TODO
int s21_is_zero(s21_decimal value);                                  // DONE
void s21_null_decimal(s21_decimal* value);                             // TODO
int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2);  // TODO
int s21_shift_left(s21_big_decimal* value, int shift);               // TODO
int s21_shift_right(s21_big_decimal* value, int shift);              // TODO


int s21_from_decimal_to_float(s21_decimal src, float* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst); //done
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_int_to_decimal(int src, s21_decimal* dst); //done

void s21_round_big_decimal(s21_big_decimal* value, int target_scale);  // TODO

#endif
