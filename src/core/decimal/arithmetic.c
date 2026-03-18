#include <stdlib.h>
#include <string.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result){
//   s21_big_decimal* lhs = s21_decimal_to_big(&value_1);
//   s21_big_decimal* rhs = s21_decimal_to_big(&value_2);

//   s21_big_decimal* big_result = s21_big_zero();
//   int code = s21_big_decimal_add(lhs, rhs, big_result);
//   free(lhs);
//   free(rhs);

//   s21_decimal another_result = s21_big_to_decimal(big_result);
//   memcpy(result, &another_result, sizeof(s21_decimal));

//   return code;
// }

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) {
    return ERROR;
  }

  s21_null_decimal(result);

  s21_big_decimal big1 = s21_decimal_to_big_internal(&value_1);
  s21_big_decimal big2 = s21_decimal_to_big_internal(&value_2);

  s21_normalize_big_pair(&big1, &big2);
  s21_big_decimal res_big;
  s21_null_big_decimal(&res_big);

  if (big1.sign != big2.sign) {
    s21_big_add(big1, big2, &res_big);
    res_big.sign = big1.sign;
  } else {
    if (s21_is_big_greater(big1, big2)) {
      s21_big_sub(big1, big2, &res_big);
      res_big.sign = big1.sign;
    } else if (s21_is_big_less(big1, big2)) {
      s21_big_sub(big2, big1, &res_big);
      res_big.sign = big2.sign;
    } else {
      s21_null_big_decimal(&res_big);
      res_big.sign = 0;
    }
  }

  if (s21_normalize_and_check_overflow(&res_big) == 1) {
    return ERROR;
  }
  s21_big_to_decimal_internal(&res_big, result);

  return OK;
}
