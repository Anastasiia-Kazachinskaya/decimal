#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"

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

void s21_multiply_big_by_10(s21_big_decimal* value) {
  unsigned long long carry = 0;

  for (int i = 0; i < 8; i++) {
    unsigned long long temp = (unsigned long long)value->bits[i] * 10 + carry;
    value->bits[i] = (unsigned int)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }
}
