// #ifndef CONVERTERS_H
// #define CONVERTERS_H

// #include "../../headers/s21_big_decimal.h" 
// #include "../../headers/s21_utils.h" 

// #include <string.h>
// #include <stdlib.h>

// s21_big_decimal* s21_decimal_to_big(s21_decimal* value){
//   if (value == NULL){
//     return NULL;
//   }
  
//   s21_big_decimal* result = calloc(1, sizeof(s21_big_decimal));
//   if (result == NULL){
//     return NULL;
//   }
  
//   result->sign = s21_get_sign(value);
//   result->scale = s21_get_scale(value);
//   memcpy(&(result->bits[5]), value, S21_BIG_DECIMAL_DATA_BITS * sizeof(int));
//   return result;
// }


// s21_decimal s21_big_to_decimal(s21_big_decimal* value){
//   if (value == NULL){
//     return s21_get_zero();
//   }
  
//   s21_decimal result = s21_get_zero();
  
//   while (get_scale(value) >= 3 * 32){
//     s21_div_ten(value);
//   }
  
//   memcpy(&(result.bits[0]), &(value->bits[5]), S21_BIG_DECIMAL_DATA_BITS * sizeof(int));
//   s21_set_sign(value->sign, &result);
//   free(value);
//   return result;
// }


// #endif