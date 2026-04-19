#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

/* Деление мантиссы на 10 с округлением к ближайшему целому (banker: при .5 — к
 * чётному). */
static int s21_big_div10_bankers(s21_big_decimal* value) {
  uint64_t remainder = 0;
  for (int i = S21_BIG_DECIMAL_SIZE - 1; i >= 0; i--) {
    const uint64_t current =
        ((uint64_t)remainder << UNSIGNED_SIZE) | value->bits[i];
    value->bits[i] = (uint32_t)(current / 10);
    remainder = current % 10;
  }

  int round_up = 0;
  if (remainder > 5) {
    round_up = 1;
  } else if (remainder == 5) {
    if ((value->bits[0] & 1u) != 0) {
      round_up = 1;
    }
  }

  if (round_up) {
    uint64_t carry = 1;
    for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
      const uint64_t sum = (uint64_t)value->bits[i] + carry;
      value->bits[i] = (uint32_t)(sum & 0xFFFFFFFFu);
      carry = sum >> UNSIGNED_SIZE;
    }
    if (carry) {
      return ERROR;
    }
  }
  return OK;
}

int s21_big_perform_signed_operation(const int sign_1, const int sign_2,
                                     s21_big_decimal* big_value_1,
                                     s21_big_decimal* big_value_2,
                                     s21_big_decimal* res_big) {
  if (sign_1 != sign_2) {
    // Разные знаки: складываем модули (для s21_add после перекодировки знаков)
    big_value_1->sign = 0;
    big_value_2->sign = 0;

    const int add_code = s21_big_add(big_value_1, big_value_2, res_big);
    res_big->sign = sign_1;
    if (add_code != OK) {
      return CALCULATION_ERROR;
    }

  } else {
    // Одинаковые знаки
    if (s21_is_big_greater(big_value_1, big_value_2)) {
      const int code = s21_big_sub(big_value_1, big_value_2, res_big);
      if (code != OK) {
        return CALCULATION_ERROR;
      }
      res_big->sign = sign_1;
    } else if (s21_is_big_less(big_value_1, big_value_2)) {
      const int code = s21_big_sub(big_value_2, big_value_1, res_big);
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

int check_overflow(const s21_big_decimal* value) {
  int overflow = 0;
  for (int i = 3; i < 8 && !overflow; i++) {
    if (value->bits[i] != 0) {
      overflow = 1;
    }
  }
  return overflow;
}

int check_mantissa(const s21_big_decimal* res_big) {
  int status = 0;
  for (int i = 3; i < S21_BIG_DECIMAL_SIZE; i++) {
    if (res_big->bits[i] != 0) {
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
  while (check_overflow(res_big) || res_big->scale > 28) {
    if (res_big->scale <= 0) {
      return CALCULATION_ERROR;
    }
    if (s21_big_div10_bankers(res_big) != OK) {
      return CALCULATION_ERROR;
    }
    res_big->scale--;
  }
  return OK;
}

// проверка влезет ли мантисса в 96 бит, если нет - то делит на 10, пока не
// влезет
int big_normalize(s21_big_decimal* v) {
  // пока не влезает в 96 бит
  while (v->bits[3] != 0 || v->bits[4] != 0 || v->bits[5] != 0 ||
         v->bits[6] != 0 || v->bits[7] != 0) {
    if (v->scale == 0) return ERROR;

    // делим мантиссу на 10, остаток в remainder
    uint64_t remainder = 0;
    for (int i = 7; i >= 0; i--) {
      const uint64_t cur = (remainder << S21_BIG_DECIMAL_SIZE) | v->bits[i];
      v->bits[i] = (uint32_t)(cur / 10);
      remainder = cur % 10;
    }

    v->scale--;

    // банковское округление по цифре remainder
    unsigned round_up = 0;
    if (remainder > 5) {
      round_up = 1;
    } else if (remainder == 5) {
      round_up = (v->bits[0] & 1);
    }
    if (round_up) big_inc(v);
  }
  return OK;
}

// сдвигает на 1 бит
void big_shift_left1(s21_big_decimal* v) {
  uint32_t carry = 0;  // беззнаковое 32-битное целое
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    const uint64_t val = ((uint64_t)v->bits[i] << 1) | carry;
    v->bits[i] = (uint32_t)(val & MASK_32_BIT);
    carry = (uint32_t)(val >> UNSIGNED_SIZE);
  }
}

// проверит, что все биты == 0
// если число нулевое, то вернет 1
int big_is_zero(const s21_big_decimal* v) {
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++)
    if (v->bits[i] != 0) return FALSE;
  return TRUE;
}

// helpers
// обнуляет все 8 битов, а также знак и scale
void big_zero(s21_big_decimal* v) {
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) v->bits[i] = 0;
  v->sign = 0;
  v->scale = 0;
}