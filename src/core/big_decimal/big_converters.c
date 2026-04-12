#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

// #include <string.h>
// #include <stdlib.h>

// s21_big_decimal* s21_decimal_to_big(s21_decimal* value){
//   if (value == NULL){
//     return NULL;
//   }

//   s21_big_decimal* result = calloc(1, sizeof(s21_big_decimal));
//   if (result == NULL){
//     return NULL;
//   }

//   result->sign = s21_get_sign(value);
//   result->scale = s21_get_scale(value);
//   memcpy(&(result->bits[5]), value, S21_BIG_DECIMAL_DATA_BITS * sizeof(int));
//   return result;
// }

// s21_decimal s21_big_to_decimal(s21_big_decimal* value){
//   if (value == NULL){
//     return s21_get_zero();
//   }

//   s21_decimal result = s21_get_zero();

//   while (get_scale(value) >= 3 * 32){
//     s21_div_ten(value);
//   }

//   memcpy(&(result.bits[0]), &(value->bits[5]), S21_BIG_DECIMAL_DATA_BITS *
//   sizeof(int)); s21_set_sign(value->sign, &result); free(value); return
//   result;
// }

s21_big_decimal decimal_to_big(s21_decimal v) {
    s21_big_decimal result;
    big_zero(&result);
    result.bits[0] = (uint32_t)v.bits[0];
    result.bits[1] = (uint32_t)v.bits[1];
    result.bits[2] = (uint32_t)v.bits[2];
    result.sign  = (v.bits[3] >> 31) & 1;
    result.scale = (v.bits[3] >> 16) & 0xFF;
    return result;
}

// конвертация из биг_децимал в децимал
int big_to_decimal(s21_big_decimal v, s21_decimal *result) {
    for (int i = 3; i < 8; i++)
        if (v.bits[i] != 0) return ERROR;
    result->bits[0] = v.bits[0];
    result->bits[1] = v.bits[1];
    result->bits[2] = v.bits[2];
    result->bits[3] = ((uint32_t)v.sign << 31) | ((uint32_t)v.scale << 16);
    return OK;
}

#endif


