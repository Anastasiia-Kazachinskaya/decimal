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

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int res = CONVERTATION_ERROR;
  if (float_or_dec_error(src, dst)) return res;

  int sign = float_sign(&src);
  char str[64];
  int precision = set_precision(src);
  snprintf(str, sizeof(str), "%.*e", precision - 1, src);
  long long mantissa = 0;
  int exponent = 0;
  int digits_after_dot = 0;
  char* e_pos = strchr(str, 'e');
  if (!e_pos) e_pos = strchr(str, 'E');
  if (e_pos) {
    apply_exponent(e_pos, &mantissa, &exponent, &digits_after_dot, str);
  }
  int scale = digits_after_dot - exponent;

  if (scale > 28) {
    if (scale_lower(&scale, &mantissa)) res = OK;
  }

  while (mantissa % 10 == 0 && mantissa != 0 && scale > 0) {
    mantissa /= 10;
    scale--;
  }

  if (scale < 0) {
    if (scale_upper(&scale, &mantissa, dst)) res = OK;
  } else {
    dst->bits[0] = (unsigned int)(mantissa & 0xFFFFFFFF);
    dst->bits[1] = (unsigned int)((mantissa >> 32) & 0xFFFFFFFF);
    dst->bits[2] = 0;
    res = OK;
  }

  dst->bits[3] = (unsigned int)((scale & 0xFF) << 16);
  if (sign) dst->bits[3] |= (1u << 31);
  return res;
}

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

  // Проверяем, есть ли биты выше 96-го (bits[3] и выше)
  // Но если bits[3] ненулевой, это часть 128-битной мантиссы
  // Проверяем bits[4] и выше для реального переполнения
  int has_overflow = 0;
  for (int i = 4; i < 8; i++) {
    if (big_result->bits[i] != 0) {
      has_overflow = 1;
      break;
    }
  }

  if (has_overflow) {
    // Есть переполнение - нужно уменьшать scale
    // Для простоты пока возвращаем ошибку
    return *result;
  }

  // Копируем 96-битную мантиссу (bits[0-2])
  // bits[3] содержит старшие биты 128-битного числа, но для decimal
  // нужно 96 бит, поэтому берём только bits[0-2]
  result->bits[0] = big_result->bits[0];
  result->bits[1] = big_result->bits[1];
  result->bits[2] = big_result->bits[2];

  // Устанавливаем знак
  if (big_result->sign) {
    result->bits[3] |= 1u << 31;
  }

  // Устанавливаем scale
  int scale = big_result->scale;
  if (scale > 28) scale = 28;
  result->bits[3] |= (scale & 0xFF) << 16;

  return *result;
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

// конвертация из децимал в биг_децимал
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