#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

int get_scale(s21_big_decimal* value) {
  int p = 0;
  while (value->bits[p] == 0 && p < 8 * 32) {
    p++;
  }
  return 8 * 32 - p;
}

s21_big_decimal* s21_big_zero() { return calloc(1, sizeof(s21_big_decimal)); }

int big_get_bit(s21_big_decimal* value, int bit_index) {
  if (bit_index < 0 || bit_index >= 8 * 32) {
    return 0;
  }
  int word_index = bit_index / 32;
  int bit_in_word = bit_index % 32;

  return (value->bits[word_index] >> bit_in_word) & 1;
}

int big_set_bit(s21_big_decimal* value, int bit_index, char bit) {
  if (!value || bit_index < 0 || bit_index >= 8 * 32) {
    return 0;
  }
  int word_index = bit_index / 32;
  int bit_in_word = bit_index % 32;
  if (bit) {
    value->bits[word_index] |= (1 << bit_in_word);
  } else {
    value->bits[word_index] &= ~(1 << bit_in_word);
  }
  return 0;
}


/* Целочисленное деление мантиссы (8×uint32) на 10 — как при сдвиге запятой
 * при нормализации; повторное округление на каждом шаге ломало точность (max*max). */
static void s21_big_div10_truncate(s21_big_decimal* value) {
  uint64_t remainder = 0;
  for (int i = 7; i >= 0; i--) {
    uint64_t current = ((uint64_t)remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(current / 10);
    remainder = current % 10;
  }
}



int s21_big_perform_signed_operation(
  int sign_1,
  int sign_2,
  s21_big_decimal* big_value_1,
  s21_big_decimal* big_value_2,
  s21_big_decimal* res_big) {
  
  
  if (sign_1 != sign_2) {
    // Разные знаки: складываем модули (для s21_add после перекодировки знаков)
    big_value_1->sign = 0;
    big_value_2->sign = 0;

    int add_code = s21_big_add(*big_value_1, *big_value_2, res_big);
    res_big->sign = sign_1;
    if (add_code != OK) {
      return CALCULATION_ERROR;
    }

  } else {
    // Одинаковые знаки
    if (s21_is_big_greater(*big_value_1, *big_value_2)) {
      int code = s21_big_sub(*big_value_1, *big_value_2, res_big);
      if (code != OK) {
        return CALCULATION_ERROR;
      }
      res_big->sign = sign_1;
    } else if (s21_is_big_less(*big_value_1, *big_value_2)) {
      int code = s21_big_sub(*big_value_2, *big_value_1, res_big);
      if (code != OK) {
        return CALCULATION_ERROR;
      }
      res_big->sign = sign_1 ? 0 : 1;
    } else {
      s21_null_big_decimal(res_big);
      res_big->sign = 0;
    }
  }
  return OK;
}



int check_overflow(s21_big_decimal value) {
    int overflow = 0;
    for (int i = 3; i < 8 && !overflow; i++) {
      if (value.bits[i] != 0) {
        overflow = 1;
      }
    }
    return overflow;
  }


int check_mantissa(s21_big_decimal res_big) {
  int status = 0;
  for (int i = 3; i < S21_BIG_DECIMAL_SIZE; i++) {
    if (res_big.bits[i] != 0) {
      status = 1;
      break;
    }
  }
  return status;
}


int s21_handle_overflow_and_rounding(s21_big_decimal* res_big) {
  if (!res_big) {
    return CALCULATION_ERROR;
  }
  while (check_overflow(*res_big) || res_big->scale > 28) {
    if (res_big->scale <= 0) {
      return CALCULATION_ERROR;
    }
    s21_big_div10_truncate(res_big);
    res_big->scale--;
  }
  return OK;
}