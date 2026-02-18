#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    int status = 0;
    s21_null_decimal(result);

    int sign1 = s21_get_sign(&value_1);
    int sign2 = s21_get_sign(&value_2);

   
    if (s21_is_zero(value_2)) {
        *result = value_1;
    }

    else if (sign1 == 1 && sign2 == 1) {

        s21_decimal a = value_1;
        s21_decimal b = value_2;

        a.bits[3] &= ~(1u << 31);
        b.bits[3] &= ~(1u << 31);

        if(b.bits[0] >= a.bits[0]) {
            result->bits[0] = b.bits[0] - a.bits[0];
        } else {
            result->bits[0] = a.bits[0] - b.bits[0];
            result->bits[3] |= 1u << 31;
        }
    } else if (sign1 == 1 && sign2 == 0) {
        result->bits[0] = value_1.bits[0] + value_2.bits[0];
        result->bits[3] |= 1u << 31;
    }
    
    else {
        if (value_1.bits[0] >= value_2.bits[0]){
            result->bits[0] = value_1.bits[0] - value_2.bits[0];
        } else {
            result->bits[0] = value_2.bits[0] - value_1.bits[0];
            result->bits[3] = 0;
            result->bits[3] |= (1u << 31);
        }
    }
    return status;
}

