#include <stdint.h> 
#include "../headers/s21_helpers.h"


int s21_floor(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_divide_mantissa_by_10(s21_decimal *dec);

int s21_truncate(s21_decimal value, s21_decimal* result) {
    if (!result) {
        return CALCULATION_ERROR;
    }

    s21_null_decimal(result);

    int scale = s21_get_scale(&value);
    int sign = s21_get_sign(&value);

    // Копируем мантиссу
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];

    int divisions = scale > 28 ? 28 : scale;
    for (int i = 0; i < divisions && !s21_is_zero(*result); i++) {
        s21_divide_mantissa_by_10(result);
    }

    if (s21_is_zero(*result)) {
        sign = 0;
    }

    // Собираем bits[3]: знак + scale
    result->bits[3] = 0;
    if (sign) {
        result->bits[3] |= 1u << 31;
    }

    return OK;
}


int s21_floor(s21_decimal value, s21_decimal* result) {
    if (!result) {
        return CALCULATION_ERROR;
    }

    s21_null_decimal(result);

    if (s21_is_zero(value)) {
        return OK;
    }

    int sign = s21_get_sign(&value);
    int scale = s21_get_scale(&value);

    s21_truncate(value, result);
    int overflow = s21_get_overflow_decimal(result);

    if (sign && scale && !s21_is_zero(*result) && !overflow) {
        result->bits[0] = result->bits[0] + 1;
    } 

    return OK;
}




int s21_divide_mantissa_by_10(s21_decimal* value) {
    uint32_t remainder = 0;

    for (int i = 2; i >= 0; i--) {
        uint64_t temp = ((uint64_t) remainder << 32) | value->bits[i];
        value->bits[i] = (uint32_t)(temp / 10);
        
        remainder = temp % 10; 
    }
    return remainder == 0 ? OK : 1;
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


int s21_get_overflow_decimal(s21_decimal* value) {
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