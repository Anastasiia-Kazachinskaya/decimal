#include "../../headers/s21_utils.h"
#include "../../headers/s21_big_decimal.h"
#include "../../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  if (s21_is_zero(src)) *dst = 0.0f;
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int res = CONVERTATION_ERROR;
  if (!dst) return res;
  *dst = 0;
  if (s21_is_zero(src)) {
    res = OK;
  } else if (src.bits[2] == 0) {
    int sign = s21_get_sign(&src);
    int scale = s21_get_scale(&src);
    unsigned int max_allowed = sign ? 2147483648U : 2147483647U;
    unsigned long long mantissa = src.bits[0];
    mantissa |= (unsigned long long)src.bits[1] << 32;
    for (int i = 0; i < scale && mantissa > 0; i++) mantissa /= 10;
    if (mantissa <= max_allowed) {
      if (sign && mantissa == 2147483648U)
        *dst = MIN_INT;
      else
        *dst = !sign ? (int)mantissa : -(int)mantissa;
      res = OK;
    }
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  if (src < 0) dst->bits[3] |= 1u << 31;
  
  return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int res = CONVERTATION_ERROR;
  if (!dst) return res;
  s21_null_decimal(dst);
  if (src == 0) res = OK;
  if (res == CONVERTATION_ERROR) {
    if (src < 0) {
      dst->bits[3] |= 1u << 31;
      if (src == MIN_INT) {
        dst->bits[0] = 0x80000000U;
        res = OK;
      } else {
        dst->bits[0] = (unsigned)(-src);
        res = OK;
      }
    } else {
      dst->bits[0] = (unsigned)(src);
      res = OK;
    }
  }
  return res;
}


s21_big_decimal s21_decimal_to_big_internal(s21_decimal* value) {
    s21_big_decimal res = {0};
    if (value) {
    for (int i = 0; i < 3; ++i) res.bits[i] = value->bits[i];
    if (s21_get_sign(value)) res.sign = 1;
    res.scale = s21_get_scale(value);
    }
    return res;
}


s21_decimal s21_big_to_decimal_internal(s21_big_decimal* big_result, s21_decimal* result) {
  s21_null_decimal(result);
  result->bits[0] = big_result->bits[0];
  if(big_result->sign) {
    result->bits[3] |= 1u << 31;
  }

  result->bits[3] |= big_result->scale << 16;
  
  return *result;
}
