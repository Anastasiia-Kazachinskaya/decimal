#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
   
    if (s21_is_zero(value_2)) {
        *result = value_1;
    } else {
        result->bits[0] = value_1.bits[0] - value_2.bits[0];
        result->bits[1] = 0;
        result->bits[2] = 0;
        result->bits[3] = 0;
    }
    return 0;
}

