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


// Округляет 224-битное значение до 96 бит по правилу "половина к чётному"
// Возвращает OK, если успешно; ERROR, если переполнение даже после округления
int s21_big_apply_bankers_round(s21_big_decimal* value) {
  if (!value) return ERROR;

  // Проверяем, есть ли биты выше 96-го
  int has_high_bits = 0;
  for (int i = 3; i < 8; i++) {
    if (value->bits[i] != 0) {
      has_high_bits = 1;
      break;
    }
  }

  if (!has_high_bits) {
    return OK;
  }

  // Делаем одно деление на 10 и запоминаем остаток для округления
  uint64_t remainder = 0;
  for (int i = 7; i >= 0; i--) {
    uint64_t current = ((uint64_t)remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(current / 10);
    remainder = current % 10;
  }

  // Увеличиваем scale
  value->scale++;

  // Банковское округление
  int round_up = 0;

  if (remainder > 5) {
    round_up = 1;
  } else if (remainder == 5) {
    // Проверяем, есть ли ещё отброшенные биты после 5
    int has_more_bits = 0;
    for (int i = 3; i < 8; i++) {
      if (value->bits[i] != 0) {
        has_more_bits = 1;
        break;
      }
    }

    if (has_more_bits) {
      // Есть ещё биты -> больше 0.5
      round_up = 1;
    } else {
      // Ровно 0.5, проверяем младший бит мантиссы
      int last_bit = value->bits[0] & 1;
      if (last_bit == 1) {
        round_up = 1;
      }
    }
  }

  // Применяем округление
  if (round_up) {
    uint32_t carry = 1;
    for (int i = 0; i < 3; i++) {
      uint64_t sum = (uint64_t)value->bits[i] + carry;
      value->bits[i] = (uint32_t)(sum & 0xFFFFFFFF);
      carry = (uint32_t)(sum >> 32);
      if (carry == 0) break;
    }

    // Если переполнение мантиссы
    if (carry) {
      value->bits[0] = 0;
      value->bits[1] = 0;
      value->bits[2] = 0;
      value->scale++;
    }
  }

  // Обнуляем старшие биты
  for (int i = 4; i < 8; i++) {
    value->bits[i] = 0;
  }

  // Проверяем, нужно ли ещё деление
  if (value->bits[3] != 0 && value->scale < 28) {
    return s21_big_apply_bankers_round(value);
  }

  // Если после всех делений scale > 28, обрезаем
  if (value->scale > 28) {
    value->scale = 28;
  }

  return OK;
}



int s21_big_perform_signed_operation(
  int sign_1,
  int sign_2,
  s21_big_decimal* big_value_1,
  s21_big_decimal* big_value_2,
  s21_big_decimal* res_big) {
  
  
  if (sign_1 != sign_2) {
    // Разные знаки: (-A) - (+B) = -(A + B)
    big_value_1->sign = 0;
    big_value_2->sign = 0;

    s21_big_add(*big_value_1, *big_value_2, res_big);

    res_big->sign = sign_1;

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
  if (res_big.bits[2] > 0x1F3F) {
    status = 1;
  } else if (res_big.bits[2] == 0x1F3F) {
    if (res_big.bits[1] > 0xFFFFFFFF) {
      status = 1;
    } else if (res_big.bits[1] == 0xFFFFFFFF) {
      if (res_big.bits[0] > 0xFFFFFFFF) {
        status = 1;
      }
    }
  }
  return status;
}


int check_overflow_after_bankers_round(s21_big_decimal res_big) {
  int overflow = 0;
  for (int i = 3; i < 8 && !overflow; i++) {
    if (res_big.bits[i] != 0) {
      overflow = 1;
    }
  }
  return overflow;
}


int s21_handle_overflow_and_rounding(s21_big_decimal* res_big) {
  int final_code = OK;
  int has_overflow = check_overflow(*res_big);
  if (has_overflow) {
    if (s21_big_apply_bankers_round(res_big) != OK) {
      final_code = CALCULATION_ERROR;
    } else if (check_overflow_after_bankers_round(*res_big) != 0) {
      final_code = CALCULATION_ERROR;
    } else {
      // При округлении scale должен уменьшиться на 1
      if (res_big->scale > 0) {
        res_big->scale--;
      }
    }
    
  }

  return final_code;
}

// обнуляет все 8 битов, а также знак и scale
void big_zero(s21_big_decimal *v) {
    for (int i = 0; i < 8; i++) v->bits[i] = 0;
    v->sign = 0;
    v->scale = 0;
}

// проверит, что все биты == 0
// если число нулевое, то вернет 1
int big_is_zero(s21_big_decimal v) {
    for (int i = 0; i < 8; i++)
        if (v.bits[i] != 0) return 0;
    return 1;
}

// сдвигает на 1 бит
void big_shift_left1(s21_big_decimal *v) {
    uint32_t carry = 0; // беззнаковое 32-битное целое
    for (int i = 0; i < 8; i++) {
        uint64_t val = ((uint64_t)v->bits[i] << 1) | carry;
        v->bits[i] = (uint32_t)(val & 0xFFFFFFFF);
        carry = (uint32_t)(val >> 32);
    }
}


// банковское округление
void big_bankers_round(s21_big_decimal *quotient,
                       s21_big_decimal remainder,
                       s21_big_decimal divisor) {
    s21_big_decimal rem2 = remainder;
    big_shift_left1(&rem2);
    if (big_gte(rem2, divisor)) {
        int equal = big_gte(divisor, rem2);
        if (equal) {
            if (quotient->bits[0] & 1) big_add1(quotient);
        } else {
            big_add1(quotient);
        }
    }
}

// делит 2 беззнаковых big_decimal, возвращает частное в quotient, scale в out_scale
int big_div_mantissa(s21_big_decimal dividend, s21_big_decimal divisor,
                     s21_big_decimal *quotient, int *out_scale) {
    s21_big_decimal remainder;
    big_zero(&remainder);
    big_zero(quotient);
    *out_scale = 0;

    for (int bit_idx = 255; bit_idx >= 0; bit_idx--) {
        big_shift_left1(&remainder);
        int word    = bit_idx / 32;
        int bit_pos = bit_idx % 32;
        int d_bit   = (dividend.bits[word] >> bit_pos) & 1;
        remainder.bits[0] |= d_bit;
        if (big_gte(remainder, divisor)) {
            big_sub(&remainder, divisor);
            quotient->bits[word] |= (1u << bit_pos);
        }
    }

    while (*out_scale < 28) {
        if (big_is_zero(remainder)) break;

        big_mul10(&remainder);

        s21_big_decimal digit;
        big_zero(&digit);
        s21_big_decimal rem_copy = remainder;
        big_zero(&remainder);

        for (int bit_idx = 255; bit_idx >= 0; bit_idx--) {
            big_shift_left1(&remainder);
            int word    = bit_idx / 32;
            int bit_pos = bit_idx % 32;
            int r_bit   = (rem_copy.bits[word] >> bit_pos) & 1;
            remainder.bits[0] |= r_bit;
            if (big_gte(remainder, divisor)) {
                big_sub(&remainder, divisor);
                digit.bits[word] |= (1u << bit_pos);
            }
        }

        if (big_mul10(quotient)) return ERROR;
        big_add(quotient, digit);
        (*out_scale)++;
    }

    if (*out_scale == 28 && !big_is_zero(remainder)) {
        big_bankers_round(quotient, remainder, divisor);
    }

    return OK;
}

// проверка влезет ли мантисса в 96 бит, если нет - то делит на 10, пока не влезет
int big_normalize(s21_big_decimal *v) {
    // пока не влезает в 96 бит
    while (v->bits[3] != 0 || v->bits[4] != 0 || v->bits[5] != 0
       || v->bits[6] != 0 || v->bits[7] != 0) {
        if (v->scale == 0) return ERROR;

        // делим мантиссу на 10, остаток в remainder
        uint64_t remainder = 0;
        for (int i = 7; i >= 0; i--) {
            uint64_t cur = (remainder << 32) | v->bits[i];
            v->bits[i] = (uint32_t)(cur / 10);
            remainder = cur % 10;
        }

        v->scale--;

        // банковское округление по цифре remainder
        int round_up = 0;
        if (remainder > 5) {
            round_up = 1;
        } else if (remainder == 5) {
            round_up = (v->bits[0] & 1);
        }
        if (round_up) big_add1(v);
    }
    return OK;
}


