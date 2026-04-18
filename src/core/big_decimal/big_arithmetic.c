#include <stdint.h>
#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

int s21_big_add(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result) {
  uint32_t carry = 0;

  // Складываем по словам с учётом переноса
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    const uint64_t sum = (uint64_t)value_1->bits[i] + value_2->bits[i] + carry;
    result->bits[i] = (uint32_t)(sum & MASK_32_BIT);  // младшие 32 бита
    carry = (uint32_t)(sum >> UNSIGNED_SIZE);                    // старшие биты = перенос
  }

  // Если после обработки последнего слова остался carry — это переполнение
  if (carry) {
    return ERROR;
  }

  return OK;
}

int s21_big_mul(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result) {
  if (!result) return ERROR;

  uint32_t temp[S21_BIG_DECIMAL_SIZE * 2];
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE * 2; i++) {
    temp[i] = 0;
  }

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    uint64_t carry = 0;

    for (int j = 0; j < S21_BIG_DECIMAL_SIZE; j++) {
      const uint64_t prod =
          (uint64_t)value_1->bits[i] * (uint64_t)value_2->bits[j] +
          temp[i + j] + carry;
      temp[i + j] = (uint32_t)(prod & MASK_32_BIT);
      carry = prod >> UNSIGNED_SIZE;
    }

    int k = i + S21_BIG_DECIMAL_SIZE;
    while (carry && k < S21_BIG_DECIMAL_SIZE * 2) {
      const uint64_t sum = (uint64_t)temp[k] + carry;
      temp[k] = (uint32_t)(sum & MASK_32_BIT);
      carry = sum >> UNSIGNED_SIZE;
      k++;
    }

    if (carry) {
      return ERROR;
    }
  }

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    result->bits[i] = temp[i];
  }

  for (int i = S21_BIG_DECIMAL_SIZE; i < S21_BIG_DECIMAL_SIZE * 2; i++) {
    if (temp[i] != 0) {
      return ERROR;
    }
  }

  return OK;
}

int s21_big_sub(const s21_big_decimal* value_1, const s21_big_decimal* value_2,
                s21_big_decimal* result) {
  if (!result) return ERROR;

  uint32_t borrow = 0;

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    const uint64_t a = value_1->bits[i];
    const uint64_t b = value_2->bits[i];

    // вычисляем разность
    const uint64_t diff = a - b - borrow;

    borrow = (a < b + borrow) ? 1 : 0;

    result->bits[i] = (uint32_t)diff;
  }

  // Если после последнего разряда остался borrow — переполнение (underflow)
  if (borrow) {
    return ERROR;
  }

  return OK;
}

// делит 2 беззнаковых big_decimal, возвращает частное в quotient, scale в
// out_scale
int big_div_mantissa(const s21_big_decimal* dividend,
                     const s21_big_decimal* divisor, s21_big_decimal* quotient,
                     int* out_scale) {
  s21_big_decimal remainder;
  big_zero(&remainder);
  big_zero(quotient);
  *out_scale = 0;

  for (int bit_idx = UNSIGNED_SIZE * S21_BIG_DECIMAL_SIZE - 1; bit_idx >= 0; bit_idx--) {
    big_shift_left1(&remainder);

    const int word = bit_idx / UNSIGNED_SIZE;
    const int bit_pos = bit_idx % UNSIGNED_SIZE;
    const unsigned d_bit = (dividend->bits[word] >> bit_pos) & 1;

    remainder.bits[0] |= d_bit;

    if (big_gte(&remainder, divisor)) {
      s21_big_sub(&remainder, divisor, &remainder);
      quotient->bits[word] |= (1u << bit_pos);
    }
  }

  while (*out_scale < SCALE) {
    if (big_is_zero(remainder)) break;

    big_mul_ten(&remainder);

    s21_big_decimal digit;
    big_zero(&digit);
    const s21_big_decimal rem_copy = remainder;
    big_zero(&remainder);

    for (int bit_idx = UNSIGNED_SIZE * S21_BIG_DECIMAL_SIZE - 1; bit_idx >= 0; bit_idx--) {
      big_shift_left1(&remainder);

      const int word = bit_idx / UNSIGNED_SIZE;
      const int bit_pos = bit_idx % UNSIGNED_SIZE;
      const unsigned r_bit = (rem_copy.bits[word] >> bit_pos) & 1;

      remainder.bits[0] |= r_bit;

      if (big_gte(&remainder, divisor)) {
        s21_big_sub(&remainder, divisor, &remainder);
        digit.bits[word] |= (1u << bit_pos);
      }
    }

    if (big_mul_ten(quotient)) return ERROR;
    s21_big_add(quotient, &digit, quotient);
    (*out_scale)++;
  }

  if (*out_scale == SCALE && !big_is_zero(remainder)) {
    big_bankers_round(quotient, remainder, *divisor);
  }

  return OK;
}

// банковское округление
void big_bankers_round(s21_big_decimal* quotient,
                       const s21_big_decimal remainder,
                       const s21_big_decimal divisor) {
  s21_big_decimal rem2 = remainder;
  big_shift_left1(&rem2);
  if (big_gte(&rem2, &divisor)) {
    int equal = big_gte(&divisor, &rem2);

    if (equal) {
      if (quotient->bits[0] & 1) big_inc(quotient);
    } else {
      big_inc(quotient);
    }
  }
}

// прибавление 1, нужно для округления
int big_inc(s21_big_decimal* v) {
  uint32_t carry = 1;  // беззнаковое 32-битное целое
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    const uint64_t val = (uint64_t)v->bits[i] + carry;
    v->bits[i] = (uint32_t)(val & MASK_32_BIT);
    carry = (uint32_t)(val >> UNSIGNED_SIZE);
    if (carry == 0) break;
  }
  return carry != 0 ? 1 : 0;
}

// умножение на 10, возращает 1, если переполнение
int big_mul_ten(s21_big_decimal* v) {
  uint32_t carry = 0;  // беззнаковое 32-битное целое
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    const uint64_t val = ((uint64_t)v->bits[i] * 10) + carry;
    v->bits[i] = (uint32_t)(val & MASK_32_BIT);
    carry = (uint32_t)(val >> UNSIGNED_SIZE);
  }
  return carry != 0 ? 1 : 0;
}
