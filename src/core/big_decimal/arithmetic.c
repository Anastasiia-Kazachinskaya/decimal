#include "../../headers/s21_big_decimal.h"

#include <stdlib.h>

int s21_big_decimal_add(s21_big_decimal* value_1, s21_big_decimal* value_2, s21_big_decimal* result){
  if (value_1 -> sign != value_2 -> sign){
    value_1 -> sign *= -1;
    return s21_big_decimal_sub(value_1, value_2, result);
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