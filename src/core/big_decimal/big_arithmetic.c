#include <stdint.h>
#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result) {
  uint32_t carry = 0;

  // Складываем по словам с учётом переноса
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    uint64_t sum = (uint64_t)value_1.bits[i] + value_2.bits[i] + carry;
    result->bits[i] = (uint32_t)(sum & 0xFFFFFFFF);  // младшие 32 бита
    carry = (uint32_t)(sum >> 32);                   // старшие биты = перенос
  }

  // Если после обработки последнего слова остался carry — это переполнение
  if (carry) {
    return ERROR;
  }

  return OK;
}

int s21_big_mul(s21_big_decimal* value_1, s21_big_decimal* value_2,
                s21_big_decimal* result) {
  if (!result) return ERROR;

  uint32_t temp[16];
  for (int i = 0; i < 16; i++) {
    temp[i] = 0;
  }

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    uint64_t carry = 0;
    for (int j = 0; j < S21_BIG_DECIMAL_SIZE; j++) {
      uint64_t prod = (uint64_t)value_1->bits[i] * (uint64_t)value_2->bits[j] +
                      temp[i + j] + carry;
      temp[i + j] = (uint32_t)(prod & 0xFFFFFFFFu);
      carry = prod >> 32;
    }
    int k = i + S21_BIG_DECIMAL_SIZE;
    while (carry && k < 16) {
      uint64_t sum = (uint64_t)temp[k] + carry;
      temp[k] = (uint32_t)(sum & 0xFFFFFFFFu);
      carry = sum >> 32;
      k++;
    }
    if (carry) {
      return ERROR;
    }
  }

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    result->bits[i] = temp[i];
  }
  for (int i = S21_BIG_DECIMAL_SIZE; i < 16; i++) {
    if (temp[i] != 0) {
      return ERROR;
    }
  }

  return OK;
}

int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result) {
  if (!result) return ERROR;

  uint32_t borrow = 0;

  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    uint64_t a = value_1.bits[i];
    uint64_t b = value_2.bits[i];

    // вычисляем разность
    uint64_t diff = a - b - borrow;

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
int big_div_mantissa(s21_big_decimal dividend, s21_big_decimal divisor,
                     s21_big_decimal* quotient, int* out_scale) {
  s21_big_decimal remainder;
  big_zero(&remainder);
  big_zero(quotient);
  *out_scale = 0;

  for (int bit_idx = 255; bit_idx >= 0; bit_idx--) {
    big_shift_left1(&remainder);
    int word = bit_idx / 32;
    int bit_pos = bit_idx % 32;
    int d_bit = (dividend.bits[word] >> bit_pos) & 1;
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
      int word = bit_idx / 32;
      int bit_pos = bit_idx % 32;
      int r_bit = (rem_copy.bits[word] >> bit_pos) & 1;
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

// банковское округление
void big_bankers_round(s21_big_decimal* quotient, s21_big_decimal remainder,
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

// сложение двух биг_децимал
void big_add(s21_big_decimal* a, s21_big_decimal b) {
  uint32_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t sum = (uint64_t)a->bits[i] + b.bits[i] + carry;
    a->bits[i] = (uint32_t)(sum & 0xFFFFFFFF);
    carry = (uint32_t)(sum >> 32);
  }
}

// прибавление 1, нужно для округления
int big_add1(s21_big_decimal* v) {
  uint32_t carry = 1;  // беззнаковое 32-битное целое
  for (int i = 0; i < 8; i++) {
    uint64_t val = (uint64_t)v->bits[i] + carry;
    v->bits[i] = (uint32_t)(val & 0xFFFFFFFF);
    carry = (uint32_t)(val >> 32);
    if (carry == 0) break;
  }
  return carry != 0 ? 1 : 0;
}

// умножение на 10, возращает 1, если переполнение
int big_mul10(s21_big_decimal* v) {
  uint32_t carry = 0;  // беззнаковое 32-битное целое
  for (int i = 0; i < 8; i++) {
    uint64_t val = ((uint64_t)v->bits[i] * 10) + carry;
    v->bits[i] = (uint32_t)(val & 0xFFFFFFFF);
    carry = (uint32_t)(val >> 32);
  }
  return carry != 0 ? 1 : 0;
}

// вычитает из a b, если a >= b
void big_sub(s21_big_decimal* a, s21_big_decimal b) {
  uint32_t borrow = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t diff = (uint64_t)a->bits[i] - b.bits[i] - borrow;
    a->bits[i] = (uint32_t)(diff & 0xFFFFFFFF);
    borrow = (diff > 0xFFFFFFFF) ? 1 : 0;
  }
}
