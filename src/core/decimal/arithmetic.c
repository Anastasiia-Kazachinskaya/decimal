#include "../../s21_decimal.h"
#include "../../headers/s21_big_decimal.h"

#include <stdlib.h>
#include <string.h>

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result){
  s21_big_decimal* lhs = s21_decimal_to_big(&value_1);
  s21_big_decimal* rhs = s21_decimal_to_big(&value_2);
  
  s21_big_decimal* big_result = s21_big_zero();
  int code = s21_big_decimal_add(lhs, rhs, big_result);
  free(lhs);
  free(rhs);
  
  s21_decimal another_result = s21_big_to_decimal(big_result);
  memcpy(result, &another_result, sizeof(s21_decimal));
  
  return code;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
