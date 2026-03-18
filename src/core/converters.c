#include <float.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../headers/s21_helpers.h"

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int res = CONVERTATION_ERROR;
  if (!dst) return res;
  if (s21_decimal_check(src)) return res;
  *dst = 0.0f;
  if (s21_is_zero(src)) {
    res = OK;
  } else {
    double result = src.bits[0];
    if (src.bits[1] != 0) result += (double)src.bits[1] * 4294967296.0;
    if (src.bits[2] != 0)
      result += (double)src.bits[2] * 18446744073709551616.0;
    int scale = s21_get_scale(&src);
    if (scale != 0)
      for (int i = 0; i < scale; ++i) result /= 10;
    if (s21_get_sign(&src) != 0) result = -result;
    if (result > FLT_MAX || result < -FLT_MAX) {
      res = CONVERTATION_ERROR;
    } else {
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
  copy_decimal(&src, &result);
  if (s21_is_zero(src)) {
    res = OK;
  } else {
    int sign = s21_get_sign(&result);
    int scale = s21_get_scale(&result);
    s21_decimal div;
    s21_from_int_to_decimal(10, &div);
    for (int i = 0; i < scale; i++) {
      s21_decimal tmp;
      s21_div(result, div, &tmp);
      result = tmp;
    }
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
  }
  return res;
}

// int s21_from_float_to_decimal(float src, s21_decimal* dst) {
//   int res = CONVERTATION_ERROR;
//   if (!dst) return res;
//   s21_null_decimal(dst);
//   if (isinf(src) || isnan(src)) {
//     return res;
//   }
//   if (src < 0) dst->bits[3] |= 1u << 31;
//   float result = 0.00;
//   if (src == 0.0f) {
//     if (src < 0) dst->bits[3] |= 1u << 31;
//     res = OK;
//   } else {
//     char str[100];
//     sprintf(str, "%.8g", src);
//     int i = strlen(str);
//     char *e_pos = strchr(str, 'e');
//     if (!e_pos) e_pos = strchr(str, 'E');
//     if (strchr(str, small_ex)){

//     }
//     if (strchr(str, big_ex)){

//     }
//     for (; i > 0 ; --i) {
//       if (str[i] == '0' && str[i+1] == '\0')
//       strcpy(tmp, str - 1);
//       strcpy(str, tmp);
//     }
//   }
//   return res;
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

s21_big_decimal s21_decimal_to_big(s21_decimal* value) {
  s21_big_decimal res = {0};
  if (value) {
    for (int i = 0; i < 3; ++i) res.bits[i] = value->bits[i];
    if (s21_get_sign(value)) res.sign = 1;
    res.scale = s21_get_scale(value);
  }
  return res;
}
