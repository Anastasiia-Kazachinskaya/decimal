#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result){
//   s21_big_decimal* lhs = s21_decimal_to_big(&value_1);
//   s21_big_decimal* rhs = s21_decimal_to_big(&value_2);

//   s21_big_decimal* big_result = s21_big_zero();
//   int code = s21_big_decimal_add(lhs, rhs, big_result);
//   free(lhs);
//   free(rhs);

//   s21_decimal another_result = s21_big_to_decimal(big_result);
//   memcpy(result, &another_result, sizeof(s21_decimal));

//   return code;
// }


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) {
    return ERROR;
  }

  s21_null_decimal(result);

  s21_big_decimal big1 = s21_decimal_to_big_internal(&value_1);
  s21_big_decimal big2 = s21_decimal_to_big_internal(&value_2);

  int sign1 = big1.sign;
  int sign2 = big2.sign;

  s21_normalize_big_pair(&big1, &big2);

  s21_big_decimal res_big;
  s21_null_big_decimal(&res_big);

  int target_scale = big1.scale;

  int code = s21_big_perform_signed_operation(sign1, sign2, &big1, &big2, &res_big);
  if (code != OK) {
    return CALCULATION_ERROR;
  }

  res_big.scale = target_scale;

  int has_overflow = check_overflow(res_big);
  if (has_overflow) {

    if (s21_big_apply_bankers_round(&res_big) != OK) {
      return CALCULATION_ERROR;
    }

    if (check_overflow_after_bankers_round(res_big) != 0) {
      return CALCULATION_ERROR;
    };

    // При округлении scale должен уменьшиться на 1
    if (res_big.scale > 0) {
      res_big.scale--;
    }
  }
  if (res_big.scale == 0) {
    // Проверяем, не превышает ли мантисса максимальное значение 7999 в десятичной
    int is_greater_than_max = check_mantissa(res_big);

    if (is_greater_than_max) {
        if (res_big.sign == 0) {
            return 1;  // 1 - слишком велико
        } else {
            return 2;  // 2 - слишком мало
        }
    }
  }

  s21_big_to_decimal_internal(&res_big, result);

  return OK;
}

