#include <stdint.h> 
#include "../s21_decimal.h"
#include "../headers/s21_helpers.h"

#define S21_BIG_DECIMAL_SIZE 7 

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);



int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    if (!result) {
        return ERROR;
    }
    
    s21_null_decimal(result);
    
    s21_big_decimal big1 = s21_decimal_to_big(&value_1);
    s21_big_decimal big2 = s21_decimal_to_big(&value_2);
    
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
    s21_big_to_decimal(&res_big, result);
    
    return OK;
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
    int32_t borrow = 0;
    
    for (int i = 0; i < S21_BIG_DECIMAL_SIZE; i++) {
        // Приводим к int64_t, чтобы корректно обработать отрицательный результат
        int64_t diff = (int64_t)value_1.bits[i] - value_2.bits[i] - borrow;
        
        if (diff < 0) {
            result->bits[i] = (uint32_t)(diff + 0x100000000LL);  // "занимаем" единицу из старшего разряда
            borrow = 1;
        } else {
            result->bits[i] = (uint32_t)diff;
            borrow = 0;
        }
    }
    
    // Если после последнего разряда остался borrow — переполнение (underflow)
    if (borrow) {
        return ERROR;
    }
    
    return OK;
}