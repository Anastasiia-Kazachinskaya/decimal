#include <stdint.h>
#include <stdlib.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

/*
int s21_big_decimal_add(s21_big_decimal* value_1, s21_big_decimal* value_2,
s21_big_decimal* result){ if (value_1 -> sign != value_2 -> sign){ value_1 ->
sign *= -1; return s21_big_decimal_sub(value_1, value_2, result);
  }
  s21_big_decimal* bigger;
  s21_big_decimal* smaller;
  if (value_1 -> scale > value_2 -> scale){
    bigger = value_1;
    smaller = value_2;
  } else {
    bigger = value_2;
    smaller = value_1;
  }
  while (bigger -> scale != smaller -> scale){
    s21_mul_ten(bigger);
  }

  result->sign = smaller -> sign;
  result->scale = smaller -> scale;
  char p = 0;
  //awful
  for (size_t i = 32 * 8 - 1; i + 1 >= 1; i--){
    char result_bit = big_get_bit(bigger, i) + big_get_bit(smaller, i) + p;
    p = result_bit / 2;
    result_bit %= 2;
    big_set_bit(result, i, result_bit);
  }

  if (p > 0){
    return NUMNER_TO_LARGE;
  }

  return 0;
}

*/

int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result) {
  uint32_t carry = 0;

  // Складываем по словам с учётом переноса
  for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
    uint64_t sum = (uint64_t)value_1.bits[i] + value_2.bits[i] + carry;
    result->bits[i] = (uint32_t)(sum & 0xFFFFFFFF);  // младшие 32 бита
    carry = (uint32_t)(sum >> 32);  // старшие биты = перенос
  }

  // Если после обработки последнего слова остался carry — это переполнение
  if (carry) {
    return ERROR;
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
