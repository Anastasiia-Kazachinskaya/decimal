#include <../headers/s21_helpers.h>
#define S21_BIG_DECIMAL_DATA_BITS 3

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

void s21_null_decimal(s21_decimal* value) { *value = (s21_decimal){0}; }

int s21_get_scale(s21_decimal* value) {
  int scale = 0;
  if (value) {
    scale = value->bits[3] >> 16 & 0xFF;
  }
  if (scale > 28) {
    return 0;
  }
  return scale;
}

int s21_get_sign(s21_decimal* value) {
  int sign = 0;
  if (value) {
    sign = (value->bits[3] & 1u << 31) != 0;
  }
  return sign;
}

// при конвертации big_decimal в decimal
int s21_set_sign(s21_big_decimal* val, s21_decimal* value) {
  if (val->sign == 1) value->bits[3] |= 1u << 31;
}

