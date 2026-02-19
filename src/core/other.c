#include "../headers/s21_helpers.h"

int s21_floor(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);



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