#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int res = CONVERTATION_ERROR;
  if (dst) {
    if (!s21_decimal_check(src)) {
      *dst = 0.0f;
      double result = 0.0;
      result = (unsigned)src.bits[0];
      if (src.bits[1] != 0) result += (unsigned)src.bits[1] * 4294967296.0;
      if (src.bits[2] != 0)
        result += (unsigned)src.bits[2] * 18446744073709551616.0;
      int scale = s21_get_scale(&src);
      if (scale != 0 && result != 0.0)
        for (int i = 0; i < scale; ++i) result /= 10.0L;
      if (s21_get_sign(&src) == 1) result = result == 0.0 ? -0.0 : -result;
      *dst = (float)result;
      res = OK;
    }
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int res = CONVERTATION_ERROR;
  if (!dst) return res;
  if (s21_decimal_check(src)) return res;
  *dst = 0;
  s21_decimal result;
  s21_truncate(src, &result);
  int sign = s21_get_sign(&result);
  if (result.bits[1] == 0 && result.bits[2] == 0) {
    unsigned mantissa = result.bits[0];
    unsigned max_allowed = sign ? 2147483648U : 2147483647U;
    if (mantissa <= max_allowed) {
      if (sign) {
        *dst = mantissa == 2147483648U ? MIN_INT : -(int)mantissa;
      } else {
        *dst = (int)mantissa;
      }
      res = OK;
    }
  }
  return res;
}

// int s21_from_float_to_decimal(float src, s21_decimal* dst) {
//   if (src < 0) dst->bits[3] |= 1u << 31;

//   return 0;
// }

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int res = CONVERTATION_ERROR;
  if (!dst) return res;
  s21_null_decimal(dst);
  if (src == 0) {
    res = OK;
  } else {
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

s21_big_decimal* s21_decimal_to_big(s21_decimal* value) {
  static s21_big_decimal res = {0};
  if (value) {
    for (int i = 0; i < 3; ++i) res.bits[i] = value->bits[i];
    if (s21_get_sign(value)) res.sign = 1;
    res.scale = s21_get_scale(value);
  }
  return &res;
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

s21_decimal s21_big_to_decimal_internal(s21_big_decimal* big_result,
                                        s21_decimal* result) {
  if (!big_result || !result) {
    s21_decimal empty;
    s21_null_decimal(&empty);
    return empty;
  }

  s21_null_decimal(result);
  for (int i = 0; i < 3; ++i) {
    result->bits[i] = big_result->bits[i];
  }

  if (big_result->sign) {
    result->bits[3] |= 1u << 31;
  }
  // 0xFF это маска 11111111 (гарантирует, что при scale > 28 мы не выйдем за
  // перделы диапазона 16 - 23 бит)
  result->bits[3] |= (big_result->scale & 0xFF) << 16;

  return *result;
}
