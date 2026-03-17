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



int s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result) {
    uint32_t carry = 0;
    
    // Складываем по словам с учётом переноса
    for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
        uint64_t sum = (uint64_t)value_1.bits[i] + value_2.bits[i] + carry;
        result->bits[i] = (uint32_t)(sum & 0xFFFFFFFF);  // младшие 32 бита
        carry = (uint32_t)(sum >> 32);                    // старшие биты = перенос
    }
    
    // Если после обработки последнего слова остался carry — это переполнение
    if (carry) {
        return ERROR;
    }
    
    return OK;
}

int s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal* result) {
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