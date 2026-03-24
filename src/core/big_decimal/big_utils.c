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


