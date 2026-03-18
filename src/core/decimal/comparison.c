#include "../../s21_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../headers/s21_big_decimal.h"

static int s21_compare(s21_decimal value_1, s21_decimal value_2) {
    if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
        return 0;
    }

    int sign_1 = s21_get_sign(value_1);
    int sign_2 = s21_get_sign(value_2);

    if (sign_1 != sign_2) {
        return (sign_1 == 0) ? 1 : -1;
    }

    s21_big_decimal big_1 = s21_decimal_to_big_internal(&value_1);
    s21_big_decimal big_2 = s21_decimal_to_big_internal(&value_2);

    int scale_1 = s21_get_scale(&value_1);
    int scale_2 = s21_get_scale(&value_2);

    while (scale_1 < scale_2) {
        s21_multiply_big_by_10(&big_1);
        scale_1++;
    }
    while (scale_2 < scale_1) {
        s21_multiply_big_by_10(&big_2);
        scale_2++;
    }

    int result = 0;

    if (s21_is_big_greater(big_1, big_2)) {
        result = 1;
    } else if (s21_is_big_less(big_1, big_2)) {
        result = -1;
    } else {
        result = 0;
    }

    if (sign_1 == 1) {
        result = -result;
    }

    return result;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) == 0 ? 1 : 0;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) != 0 ? 1 : 0;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) > 0 ? 1 : 0;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) >= 0 ? 1 : 0;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) < 0 ? 1 : 0;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return s21_compare(value_1, value_2) <= 0 ? 1 : 0;
}