#include "../headers/s21_helpers.h"

int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);


int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2) {
    int result = 0;
    int found_diff = 0;
    for (int i = 6; i >= 0 && (!found_diff); i--) {
        if (value1.bits[i] < value2.bits[i]) {
            found_diff = 1;
            result = 1;
        } else if (value1.bits[i] > value2.bits[i]) {
            found_diff = 1;
            result = 0;
        }
    }

    return result;
}

int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2) {
    int result = 0;
    int found_diff = 0;
    for (int i = 6; i >= 0 && (!found_diff); i--) {
        if (value1.bits[i] > value2.bits[i]) {
            found_diff = 1;
            result = 1;
        } else if (value1.bits[i] < value2.bits[i]) {
            found_diff = 1;
            result = 0;
        }
    }

    return result;
}

