#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) {
    return ERROR;
  }

  s21_null_decimal(result);

  s21_big_decimal big1 = s21_decimal_to_big_internal(&value_1);
  s21_big_decimal big2 = s21_decimal_to_big_internal(&value_2);

  int sign1 = big1.sign;
  int sign2 = big2.sign ? 0 : 1;

  s21_normalize_big_pair(&big1, &big2);
  s21_big_decimal res_big;
  s21_null_big_decimal(&res_big);

  int target_scale = big1.scale;
  int code =
      s21_big_perform_signed_operation(sign1, sign2, &big1, &big2, &res_big);

  if (code != OK) {
    return res_big.sign ? 2 : 1;
  }

  res_big.scale = target_scale;
  int final_code = s21_handle_overflow_and_rounding(&res_big);
  if (final_code != OK) {
    return res_big.sign ? 2 : 1;
  }
  if (res_big.scale == 0) {
    int is_greater_than_max = check_mantissa(res_big);
    if (is_greater_than_max) {
      return res_big.sign ? 2 : 1;
    }
  }
  s21_big_to_decimal_internal(&res_big, result);
  return OK;
}

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
  int code =
      s21_big_perform_signed_operation(sign1, sign2, &big1, &big2, &res_big);

  if (code != OK) {
    return res_big.sign ? 2 : 1;
  }

  res_big.scale = target_scale;
  int final_code = s21_handle_overflow_and_rounding(&res_big);
  if (final_code != OK) {
    return res_big.sign ? 2 : 1;
  }
  if (res_big.scale == 0) {
    int is_greater_than_max = check_mantissa(res_big);
    if (is_greater_than_max) {
      return res_big.sign ? 2 : 1;
    }
  }
  s21_big_to_decimal_internal(&res_big, result);
  return OK;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) {
    return ERROR;
  }

  s21_null_decimal(result);

  if (s21_is_zero(value_1) || s21_is_zero(value_2)) {
    return OK;
  }

  s21_big_decimal big1 = s21_decimal_to_big_internal(&value_1);
  s21_big_decimal big2 = s21_decimal_to_big_internal(&value_2);

  int result_sign = big1.sign ^ big2.sign;
  int result_scale = big1.scale + big2.scale;

  big1.sign = 0;
  big2.sign = 0;

  s21_big_decimal res_big;
  s21_null_big_decimal(&res_big);

  int code = s21_big_mul(&big1, &big2, &res_big);
  if (code != OK) {
    return result_sign ? 2 : 1;
  }

  res_big.sign = result_sign;
  res_big.scale = result_scale;

  int final_code = s21_handle_overflow_and_rounding(&res_big);
  if (final_code == OK && res_big.scale == 0) {
    int is_greater_than_max = check_mantissa(res_big);
    if (is_greater_than_max) {
      final_code = res_big.sign ? 2 : 1;
    }
  }
  if (final_code != OK) {
    return res_big.sign ? 2 : 1;
  }

  s21_big_to_decimal_internal(&res_big, result);
  return OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return ERROR;
  s21_null_decimal(result);
  if (s21_is_zero(value_2)) return DIVISION_BY_ZERO;
  if (s21_is_zero(value_1)) {
    s21_null_decimal(result);
    return OK;
  }

  s21_big_decimal big1 = decimal_to_big(value_1);
  s21_big_decimal big2 = decimal_to_big(value_2);

  // знак результата
  int result_sign = big1.sign ^ big2.sign;

  int scale_correction = big1.scale - big2.scale;

  // убираем знаки и scale — работаем с чистыми мантиссами
  big1.sign = 0;
  big1.scale = 0;
  big2.sign = 0;
  big2.scale = 0;

  s21_big_decimal quotient;
  int out_scale = 0;
  int code = big_div_mantissa(big1, big2, &quotient, &out_scale);
  if (code != OK) return result_sign ? NUMNER_TO_SMALL : NUMNER_TO_LARGE;

  // итоговый scale
  int final_scale = out_scale + scale_correction;

  // коррекция отрицательного scale
  if (final_scale < 0) {
    for (int i = 0; i < -final_scale; i++)
      if (big_mul10(&quotient))
        return result_sign ? NUMNER_TO_SMALL : NUMNER_TO_LARGE;
    final_scale = 0;
  }

  // коррекция scale > 28
  while (final_scale > 28) {
    uint64_t rem = 0;
    for (int w = 7; w >= 0; w--) {
      uint64_t cur = (rem << 32) | quotient.bits[w];
      quotient.bits[w] = (uint32_t)(cur / 10);
      rem = cur % 10;
    }
    final_scale--;

    int round_up = 0;
    if (rem > 5) {
      round_up = 1;
    } else if (rem == 5) {
      round_up = (quotient.bits[0] & 1);
    }
    if (round_up) big_add1(&quotient);
  }

  quotient.sign = result_sign;
  quotient.scale = final_scale;

  if (big_normalize(&quotient) != OK)
    return result_sign ? NUMNER_TO_SMALL : NUMNER_TO_LARGE;

  return big_to_decimal(quotient, result);
}
