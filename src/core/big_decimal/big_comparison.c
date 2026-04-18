#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"

int s21_is_big_less(s21_big_decimal value1, s21_big_decimal value2) {
  int result = 0;
  int found_diff = 0;
  for (int i = 6; i >= 0 && (!found_diff); i--) {
    if (value1.bits[i] < value2.bits[i]) {
      found_diff = 1;
      result = 1;
    } else if (value1.bits[i] > value2.bits[i]) {
      found_diff = 1;
      result = 0;
    }
  }

  return result;
}

int s21_is_big_greater(s21_big_decimal value1, s21_big_decimal value2) {
  int result = 0;
  int found_diff = 0;
  for (int i = 6; i >= 0 && (!found_diff); i--) {
    if (value1.bits[i] > value2.bits[i]) {
      found_diff = 1;
      result = 1;
    } else if (value1.bits[i] < value2.bits[i]) {
      found_diff = 1;
      result = 0;
    }
  }

  return result;
}

// проверяет, что a >= b
int big_gte(const s21_big_decimal* a, const s21_big_decimal* b) {
  for (int i = FLOAT_DIGITS; i >= 0; i--) {
    if (a->bits[i] > b->bits[i]) {
      return 1;
    }
    if (a->bits[i] < b->bits[i]) {
      return 0;
    }
  }
  return 1;
}