#include "../../s21_decimal.h"
#include "../../headers/s21_utils.h"

int s21_from_decimal_to_float(s21_decimal src, float* dst);

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

int s21_from_float_to_decimal(float src, s21_decimal* dst);

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
