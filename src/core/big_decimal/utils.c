#include "../../headers/s21_big_decimal.h"

// при конвертации big_decimal в decimal
int s21_set_sign(s21_big_decimal* val, s21_decimal* value) {
  if (val->sign == 1) value->bits[3] |= 1u << 31;
}
