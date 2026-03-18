
#include <stdint.h>
#include <stdio.h>
#include "../../headers/s21_utils.h"
#include "../../headers/s21_big_decimal.h"

#include "../../s21_decimal.h"

#define S21_MAX_SCALE 28

int s21_floor(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);

int s21_divide_mantissa_by_10(s21_decimal *dec);
static int s21_increment_mantissa(s21_decimal* value);
int s21_normalize_and_check_overflow(s21_big_decimal* value);
static void s21_set_scale_internal(s21_decimal *value, int scale);
static void s21_pow10_big(int scale, s21_big_decimal* result);
static void s21_big_div2(s21_big_decimal* value);
static int s21_inc_decimal(s21_decimal* value);
int s21_is_big_equal(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_multiply_big_by_10(s21_big_decimal* value);
static int s21_scale_normalize_big_to(s21_big_decimal* val, int target_scale);
static void s21_compute_rounding_threshold(int scale, s21_big_decimal* half);
static int s21_compute_fractional_big(
    s21_decimal original,
    s21_decimal truncated,
    s21_big_decimal* fractional);
static int s21_apply_bankers_rounding(
    s21_decimal* result,
    s21_big_decimal* fractional,
    s21_big_decimal* half);
void s21_set_sign_internal(s21_decimal* result, int sign);


int s21_floor(s21_decimal value, s21_decimal* result) {
    int status = OK;
    
    if (!result) return CALCULATION_ERROR;

    s21_null_decimal(result);

    if (s21_is_zero(value)) {
        return status;
    }

    int sign = s21_get_sign(&value);
    int scale = s21_get_scale(&value);

    s21_truncate(value, result);
    
    if (sign && scale && !s21_is_zero(*result)) {
        if (s21_increment_mantissa(result) != OK) {
            status = CALCULATION_ERROR;
        }

    }

    return status;
}

int s21_round(s21_decimal value, s21_decimal* result) {
    int status = OK;

    if (!result) return CALCULATION_ERROR;

    s21_null_decimal(result);

    if (s21_is_zero(value)) return status;

    int scale = s21_get_scale(&value);
    if (scale == 0) {
        *result = value;
        return status;
    }

    int sign = s21_get_sign(&value);

    // 1 получаем целую часть
    s21_truncate(value, result);

    // 2 Вычисляем порог округления 10^scale / 2
    s21_big_decimal half;
    s21_null_big_decimal(&half);
    s21_compute_rounding_threshold(scale, &half);
    
    // 3 Вычисляем дробную часть
    s21_big_decimal fractional;
    s21_null_big_decimal(&fractional);
    if(s21_compute_fractional_big(value, *result, &fractional) != OK) return CALCULATION_ERROR;

    if (s21_apply_bankers_rounding(result, &fractional, &half) != OK) return CALCULATION_ERROR;
    
    s21_set_scale_internal(result, 0);
    s21_set_sign_internal(result, sign);

    return status;
}

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

    int divisions = scale > S21_MAX_SCALE ? S21_MAX_SCALE : scale;
    for (int i = 0; i < divisions; i++) {
        s21_divide_mantissa_by_10(result);
    }


    result->bits[3] = (sign << 31) | (0 << 16); 
    if (result->bits[0] == 0 && result->bits[1] == 0 && result->bits[2] == 0) {
        result->bits[3] = 0;
    }

    return OK;
}

int s21_divide_mantissa_by_10(s21_decimal* value) {
    int status = OK;

    if (!value) return CALCULATION_ERROR;
    
    uint32_t remainder = 0;
    
    for (int i = 2; i >= 0; i--) {
        uint64_t temp = ((uint64_t)remainder << 32) | (uint32_t)value->bits[i];
        value->bits[i] = (uint32_t)(temp / 10);
        remainder = (uint32_t)(temp % 10);
    }
    if (remainder) {
        status = CALCULATION_ERROR;
    }
    
    return status;
}

static int s21_increment_mantissa(s21_decimal* value) {
    int status = CALCULATION_ERROR;
    if (!value) return CALCULATION_ERROR;

    for (int i = 0; i < 3; i++) {
        if (++value->bits[i] != 0) { // при переполнении bit[i] = 0, bits[1] = 1 etc.
            status = OK;
            break;
        }
    }
    return status;
}



int s21_normalize_big_pair(s21_big_decimal* value_1, s21_big_decimal* value_2) {
    int status = OK;

    if (!value_1 || !value_2) {
        return CALCULATION_ERROR;
    } 

    int target_scale = (value_1->scale > value_2->scale) ? value_1->scale : value_2->scale;

    if (s21_scale_normalize_big_to(value_1, target_scale) != OK) {
        status = CALCULATION_ERROR;
    }

    if (s21_scale_normalize_big_to(value_2, target_scale) != OK) {
        status = CALCULATION_ERROR;
    }

    return status;
}


int s21_normalize_pair(s21_decimal* value_1, s21_decimal* value_2) {
    (void) value_1;
    (void) value_2;
    return 0;
}

int s21_normalize_and_check_overflow(s21_big_decimal* value) {
    int status = OK;

    int overflow = 0;
    for (int i = 0; i < S21_DECIMAL_LIMIT; i++) {
        value->bits[i] += overflow;
        overflow = (uint64_t)value->bits[i] >> 32;
        value->bits[i] &= MAX4BITE;
    }
    if(overflow) {
            status = CALCULATION_ERROR; // 1
        } else {
            status = OK;
        }
    return status; // 0
}


static void s21_set_scale_internal(s21_decimal *value, int scale) {
    if (!value) return;
    if (scale < 0) {
        scale = 0;
    }
    if (scale  > 28) {
        scale = 28;
    }
    value->bits[3] = (value->bits[3] & ~(0xFF << 16)) | (scale << 16);
}


// вычисляем divisor (10^scale) , где scale 0-28
static void s21_pow10_big(int scale, s21_big_decimal* result) {
    s21_null_big_decimal(result);
    result->bits[0] = 1;
    result->scale = 0;
    result->sign = 0;

    for (int i = 0; i < scale; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < 7; j++) {
            uint64_t prod = (uint64_t)result->bits[j] * 10ULL + carry; // 10ULL (unsigned long long) 64 битное умножение
            result->bits[j] = (uint32_t)(prod & 0xFFFFFFFFULL);
            carry = prod >> 32; // получаем старшие биты через перенос
        }
    }
}


// Вспомогательная: деление big_decimal на 2
static void s21_big_div2(s21_big_decimal* value) {
    unsigned int remainder = 0;
    for (int i = 6; i >= 0; i--) {
        uint32_t current = (uint32_t)value->bits[i];
        value->bits[i] = (current >> 1) | (remainder ? 0x80000000u : 0); // 0x80000000u - 32-битное число с установленным старшим битом = 2^31
        remainder = (current & 1u);
    }
}

// Безопасный инкремент decimal (96 бит с переносом)
static int s21_inc_decimal(s21_decimal* value) {
    int status = CALCULATION_ERROR;

    if (!value) {
        return status;
    }

    for (int i = 0; i < 3; i++) {
        if (++value->bits[i] != 0) {
            status = OK;
            break;
        }
    }
    return status;
}


int s21_is_big_equal(s21_big_decimal value_1, s21_big_decimal value_2) {
    if (value_1.sign != value_2.sign || value_1.scale != value_2.scale) return 0;
    for (int i = 0; i < 7; i++) {
        if (value_1.bits[i] != value_2.bits[i]) return 0;
    }
    return 1;
}

// Умножает big_decimal на 10, возвращает 0 при успехе, 1 при переполнении
int s21_multiply_big_by_10(s21_big_decimal* value) {
    int status = OK;

    if (!value) return CALCULATION_ERROR;
    uint64_t carry = 0;
    for (int j = 0; j < 7; j++) {
        uint64_t prod = value->bits[j] * 10ULL + carry;
        value->bits[j] = (uint32_t)(prod & 0xFFFFFFFFULL);
        carry = prod >> 32;
    }
    if (carry) {
        status = CALCULATION_ERROR;  // переполнение 224 бит (28 scale)
    } else {
         value->scale++;
    }

    return status;
}

static int s21_scale_normalize_big_to(s21_big_decimal* val, int target_scale) {
    int status = OK;

    if (!val || target_scale < val->scale) return CALCULATION_ERROR;
    while (val->scale < target_scale) {
        if (s21_multiply_big_by_10(val) != OK) {
            break;
        }
    }
    return status;
}

// вычисляет порог округления 10^scale / 2
static void s21_compute_rounding_threshold(int scale, s21_big_decimal* half) {
    s21_big_decimal divisor;
    s21_null_big_decimal(&divisor);
    s21_pow10_big(scale, &divisor); // divisor = 10^scale
    *half = divisor; // копируем
    s21_big_div2(half);  // half = divisor / 2
    half->scale = scale;
    half->sign = 0;
}

static int s21_compute_fractional_big(s21_decimal original, s21_decimal truncated, s21_big_decimal* fractional) {
    int status = OK;
    s21_big_decimal big_orig = s21_decimal_to_big_internal(&original); // исходное число
    s21_big_decimal big_truncated = s21_decimal_to_big_internal(&truncated); // целая часть

    if(s21_scale_normalize_big_to(&big_truncated, big_orig.scale) != OK) {
        status = CALCULATION_ERROR;
    } else {
        // fractional = original - truncated
        s21_null_big_decimal(fractional);
        s21_big_sub(big_orig, big_truncated, fractional);

        fractional->scale = big_orig.scale;
        fractional->sign = 0;   // дробная часть всегда положительная
    }


    return status;
}

static int s21_apply_bankers_rounding(
    s21_decimal* result,
    s21_big_decimal* fractional,
    s21_big_decimal* half) {
    int status = OK;

    // банковское округление
    if (s21_is_big_greater(*fractional, *half)) {
        // > 0.5 → округляем "от нуля"
        if (s21_inc_decimal(result) != OK) {
            status = CALCULATION_ERROR;
        }
    } else if (s21_is_big_equal(*fractional, *half)) {
        // == 0.5 → округляем к ближайшему чётному
        // Проверяем чётность всей 96-битной мантиссы (достаточно bits[0])
        if (result->bits[0] % 2 != 0) {
            if (s21_inc_decimal(result) != OK) {
                status = CALCULATION_ERROR;
            }
        }
    }

    // fractional < half → округление вниз (ничего не делаем)
    return status;
}


void s21_set_sign_internal(s21_decimal* result, int sign) {
    if (sign){
        result->bits[3] |= (1u << 31);
    } else {
        result->bits[3] &= ~(1u << 31);
    }

}

