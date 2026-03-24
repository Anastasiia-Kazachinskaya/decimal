#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

// получить значение (0 или 1) конкретного бита по его индексу (0-95)
int s21_get_bit(s21_decimal value, int bit_index) {
  if (bit_index < 0 || bit_index >= 96) {
    return 0;
  }
  int word_index = bit_index / 32;
  int bit_in_word = bit_index % 32;

  return (value.bits[word_index] >> bit_in_word) & 1;
}

// установить бит с индексом в значение 0 или 1
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value) {
  if (!value || bit_index < 0 || bit_index >= 96) {
    return 0;
  }
  int word_index = bit_index / 32;
  int bit_in_word = bit_index % 32;
  if (bit_value) {
    value->bits[word_index] |= (1 << bit_in_word);
  } else {
    value->bits[word_index] &= ~(1 << bit_in_word);
  }
  return 0;
}

int s21_get_scale(s21_decimal* value) {
  int scale = 0;
  if (value) {
    scale = value->bits[3] >> 16 & 0xFF;  // 0xFF = 11111111
  }
  if (scale > 28) {
    return 0;
  }
  return scale;
}

void s21_null_decimal(s21_decimal* value) {
  if (value) {
    memset(value, 0, sizeof(s21_decimal));
  }
}

void s21_null_big_decimal(s21_big_decimal* value) {
  if (value) {
    memset(value, 0, sizeof(s21_big_decimal));
  }
}

int s21_is_zero(s21_decimal value) {
  int result = 1;
  int i;
  int has_nonzero_bit = 0;

  for (i = 0; i < S21_BIG_DECIMAL_DATA_BITS && !(has_nonzero_bit); i++) {
    if (value.bits[i] != 0) {
      has_nonzero_bit = 1;
      result = 0;
    }
  }

  return result;
}

int s21_get_sign(s21_decimal* value) {
  int sign = 0;
  if (value) {
    sign = (value->bits[3] & 1u << 31) != 0;
  }
  return sign;
}

s21_decimal s21_get_zero() {
  s21_decimal zero;
  char* ptr = (char*)&zero;
  for (size_t i = 0; i < sizeof(s21_decimal); i++) {
    ptr[i] = 0;
  }
  return zero;
}

void s21_set_sign(char val, s21_decimal* value) { value->bits[3] |= val << 31; }

int s21_decimal_check(s21_decimal value) {
  int result = 0;

  int scale = s21_get_scale(&value);

  unsigned int allowed_mask = 0;
  allowed_mask |= (scale << 16);
  if (s21_get_sign(&value)) {
    allowed_mask |= (1u << 31);
  }

  if ((value.bits[3] & ~allowed_mask) != 0) {
    result = 1;
  }

  return result;
}

void copy_decimal(s21_decimal* value_1, s21_decimal* value_2) {
  if (value_1 && value_2) {
    for (int i = 0; i < 4; ++i) value_2->bits[i] = value_1->bits[i];
  }
}

long long s21_str_to_ll(const char* str) {
  long long result = 0;
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }
  return result;
}

// Вспомогательная функция для парсинга экспоненты
int s21_str_to_int(const char* str) {
  int result = 0;
  int sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }
  return result * sign;
}

int float_or_dec_error(float src, s21_decimal* dst) {
  int res = 0;
  if (!dst) res = 1;
  float copy = src;
  s21_null_decimal(dst);
  if (isinf(src) || isnan(src)) res = 1;
  if (src < 0) copy = -src;
  if (copy != 0.0f) {
    if (copy < 1e-28f - 1e-35f ||
        copy > 7.9228162514264337593543950335e+28f + 1e+23f) {
      res = 1;
    }
  }
  return res;
}

int float_sign(float* src) {
  int res = 0;
  if (*src < 0) {
    res = 1;
    *src = -*src;
  }
  return res;
}

int scale_lower(int* scale, long long* mantissa) {
  int res = 1;
  int extra = *scale - 28;
  long long divisor = 1;
  for (int i = 0; i < extra; i++) divisor *= 10;

  if (*mantissa / divisor == 0) res = 0;

  long long remainder = *mantissa % divisor;
  *mantissa = *mantissa / divisor;

  if (remainder >= divisor / 2) (*mantissa)++;

  *scale = 28;
  return res;
}

int scale_upper(int* scale, long long* mantissa, s21_decimal* dst) {
  int res = 1;
  s21_big_decimal big_temp;
  s21_null_big_decimal(&big_temp);
  big_temp.bits[0] = (unsigned int)(*mantissa & 0xFFFFFFFF);
  big_temp.bits[1] = (unsigned int)((*mantissa >> 32) & 0xFFFFFFFF);
  big_temp.bits[2] = 0;
  big_temp.scale = 0;
  big_temp.sign = 0;

  while (*scale < 0) {
    if (s21_multiply_big_by_10(&big_temp) != OK) {
      res = 0;
    }
    if (res != 0) (*scale)++;
  }
  s21_decimal temp_dst;
  s21_null_decimal(&temp_dst);
  temp_dst = s21_big_to_decimal_internal(&big_temp, &temp_dst);
  *dst = temp_dst;
  return res;
}

void apply_exponent(char* e_pos, long long* mantissa, int* exponent,
                    int* digits_after_dot, char* str) {
  *exponent = s21_str_to_int(e_pos + 1);
  char mantissa_str[32] = {0};
  int m_idx = 0;
  for (char* p = str; p < e_pos; p++) {
    if (*p != '.' && *p != '+' && *p != '\0') {
      mantissa_str[m_idx++] = *p;
    }
  }
  mantissa_str[m_idx] = '\0';
  *mantissa = s21_str_to_ll(mantissa_str);
  char* dot_pos = strchr(str, '.');
  if (dot_pos) {
    *digits_after_dot = e_pos - dot_pos - 1;
  }
}

int set_precision(float src) {
  return src < 1e-20f ? 10 : src < 1e-10f ? 9 : src < 1e-5f ? 8 : FLOAT_DIGITS;
}