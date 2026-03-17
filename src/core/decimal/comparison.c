#include "../../s21_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../headers/s21_big_decimal.h"

static int s21_compare(s21_decimal value_1, s21_decimal value_2) {
    if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
        return 0;
    }
    int sign_1 = s21_get_sign(&value_1);
    int sign_2 = s21_get_sign(&value_2);

    if (sign_1 != sign_2) {
        return (sign_1 == 0) ? 1 : -1;
    }
    int result = 0;

    if (value_1.bits[2] != value_2.bits[2]) {
        result = (value_1.bits[2] > value_2.bits[2]) ? 1 : -1;
    } else if (value_1.bits[1] != value_2.bits[1]) {
        result = (value_1.bits[1] > value_2.bits[1]) ? 1 : -1;
    } else if (value_1.bits[0] != value_2.bits[0]) {
        result = (value_1.bits[0] > value_2.bits[0]) ? 1 : -1;
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