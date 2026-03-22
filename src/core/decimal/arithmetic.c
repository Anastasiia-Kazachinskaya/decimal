#include "../../s21_decimal.h"
#include "../../headers/s21_big_decimal.h"

#include "../../headers/s21_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);


int s21_big_apply_bankers_round(s21_big_decimal* value);
int s21_big_bankers_round(s21_big_decimal* value);
int s21_big_inc_lower_96(s21_big_decimal* value);


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
        res_big.scale = big1.scale;
    } else {
        if (s21_is_big_greater(big1, big2)) {
            s21_big_sub(big1, big2, &res_big);
            res_big.sign = big1.sign;
            res_big.scale = big1.scale;
        } else if (s21_is_big_less(big1, big2)) {
            s21_big_sub(big2, big1, &res_big);
            res_big.sign = big2.sign;
            res_big.scale = big2.scale;
        } else {
            s21_null_big_decimal(&res_big);
            res_big.sign = 0;
            res_big.scale = big1.scale;
        }
    }


    int overflow_status = s21_normalize_and_check_overflow(&res_big);

        if (overflow_status == 1) { // 1 - нужно банковское округление
            if (s21_big_apply_bankers_round(&res_big) != OK) {
                return ERROR;
            }
        } else if (overflow_status == 2) { // фатальная ошибка при обработке переполнения
            return ERROR;
        }

    s21_big_to_decimal_internal(&res_big, result);
    
    return OK;
}



// Округляет 224-битное значение до 96 бит по правилу "половина к чётному"
// Возвращает OK, если успешно; ERROR, если переполнение даже после округления
int s21_big_apply_bankers_round(s21_big_decimal* value) {
    if (!value) return ERROR;
    
    // === Собираем информацию об отбрасываемых битах [3..6] ===
    // Бит 96 — это бит 0 в value->bits[3]
    uint32_t first_dropped = value->bits[3];  // биты 96..127
    uint32_t rest_dropped = 0; 
    
    // Есть ли ненулевые биты после позиции 127? #TODO: убрать break
    for (int i = 4; i < S21_BIG_DECIMAL_SIZE; i++) {
        if (value->bits[i] != 0) {
            // Есть ли НЕЗНАЧИМЫЕ биты после 127?
            rest_dropped = 1;
            break;
        }
    }
    
    // bit_96 = 1 означает: 1 * 2^-96 позиции после запятой
    int bit_96 = first_dropped & 1;
    
    // Округляем ВВЕРХ если:
    // 1. rest_dropped = 1  (биты 128+ ≠ 0)  
    // 2. ИЛИ first_dropped > 1 (биты 97+ ≠ 0)
    int greater_than_half = rest_dropped || (first_dropped > 1); // first_dropped > 1 = проверка "есть ли что-то ЕЩЁ кроме 0.5?"
    
    // === Применяем банковское правило ===
    if (greater_than_half) {
        // > 0.5 → округляем вверх (инкремент нижних 96 бит)
        if (s21_big_inc_lower_96(value) != OK) return ERROR;
        
    } else if (bit_96 == 1) {
        // == 0.5 ровно → к ближайшему чётному
        // Проверяем чётность бита 95 (младший бит сохраняемой части)
        int bit_95 = value->bits[2] & 1;
        if (bit_95 == 1) {
            // Нечётное → округляем вверх
            if (s21_big_inc_lower_96(value) != OK) return ERROR;
        }
        // Чётное → округляем вниз (ничего не делаем)
    }
    // < 0.5 → округляем вниз (ничего не делаем)
    
    // Обнуляем отброшенные биты [3..6]
    for (int i = 3; i < S21_BIG_DECIMAL_SIZE; i++) {
        value->bits[i] = 0;
    }
    
    return OK;
}

// Вспомогательная: инкремент только нижних 96 бит (биты 0..2)
int s21_big_inc_lower_96(s21_big_decimal* value) {
    if (!value) return ERROR;
    uint32_t carry = 1;
    for (int i = 0; i < 3; i++) {
        uint64_t sum = (uint64_t)value->bits[i] + carry;
        value->bits[i] = (uint32_t)(sum & MAX4BITE);
        carry = (uint32_t)(sum >> 32);
    }
    return (carry == 0) ? OK : ERROR;
}

