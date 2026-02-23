#include <stdint.h> 
#include "../headers/s21_helpers.h"


int s21_floor(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);

int s21_truncate(s21_decimal value, s21_decimal* result) {
    if (!result) {
        return CALCULATION_ERROR;
    }
    int val_scale;
    val_scale = s21_get_scale(&value);
    if (val_scale == 0) {
        for (int i = 0; i < 4; i++) {
            result->bits[i] = value.bits[i];
        }
    }
    return OK;
}

int s21_normalize_big_pair(s21_big_decimal* value_1, s21_big_decimal* value_2) {
    (void) value_1;
    (void) value_2;
    return 0;
}


int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2) {
    (void) value_1;
    (void) value_2;
    return 0;
}

int s21_get_overflow(s21_big_decimal* value) {
    int overflow = 0;
    for (int i = 0; i < S21_DECIMAL_LIMIT; i++) {
        value->bits[i] += overflow;
        overflow = (uint64_t)value->bits[i] >> 32;
        value->bits[i] &= MAX4BITE;
    }
    if(overflow) {
            return ERROR; // 1
        }
    return OK; // 0
}