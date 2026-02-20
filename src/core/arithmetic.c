#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"


// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    if (!result) {
        return ERROR;
    }
    
    s21_null_decimal(result);
    
    s21_big_decimal big1 = s21_decimal_to_big(&value_1);
    s21_big_decimal big2 = s21_decimal_to_big(&value_2);
    
    s21_normalize_big_pair(&big1, &big2);
    s21_big_decimal res_big;
    s21_null_big_decimal(&res_big);

    
    if (big1.sign != big2.sign) {
        s21_big_add(big1, big2, &res_big);
        res_big.sign = big1.sign;
    } else {
        if (s21_is_big_greater(big1, big2)) {
            s21_big_sub(big1, big2, &res_big);
            res_big.sign = big1.sign;
        } else if (s21_is_big_less(big1, big2)) {
            s21_big_sub(big2, big1, &res_big);
            res_big.sign = big1.sign;
        } else {
            s21_null_big_decimal(&res_big);
            res_big.sign = 0;
        }
    }
    

    s21_big_to_decimal(&res_big, result);
    
    return OK;
}

// #todo: в big_sub учесть функцию переполнения

int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result) {
    
    for (int i = 0; i < 7; i++) {
        result->bits[i] = value_1.bits[i] - value_2.bits[i];
    }

    return OK;
}

int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result) {
    (void) value_1;
    (void) value_2;
    (void) result;
    return 0;
}