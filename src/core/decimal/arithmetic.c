#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/s21_big_decimal.h"
#include "../../headers/s21_utils.h"
#include "../../s21_decimal.h"

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
    printf("%s\n", "ошибка №1");
    return ERROR;
  }

  s21_null_decimal(result);

  s21_big_decimal big1 = s21_decimal_to_big_internal(&value_1);
  s21_big_decimal big2 = s21_decimal_to_big_internal(&value_2);

  int sign1 = big1.sign;
  int sign2 = big2.sign;

  s21_normalize_big_pair(&big1, &big2);

  s21_big_decimal res_big;
  s21_null_big_decimal(&res_big);

  int target_scale = big1.scale;

  if (sign1 != sign2) {
    // Разные знаки: (-A) - (+B) = -(A + B)
    big1.sign = 0;
    big2.sign = 0;

    s21_big_add(big1, big2, &res_big);

    res_big.sign = sign1;

  } else {
    // Одинаковые знаки
    if (s21_is_big_greater(big1, big2)) {
      int code = s21_big_sub(big1, big2, &res_big);
      if (code != OK) {
        printf("%s\n", "ошибка №3");
        return CALCULATION_ERROR;
      }
      res_big.sign = sign1;
    } else if (s21_is_big_less(big1, big2)) {
      int code = s21_big_sub(big2, big1, &res_big);
      if (code != OK) {
        printf("%s\n", "ошибка №4");
        return CALCULATION_ERROR;
      }
      res_big.sign = sign1 ? 0 : 1;
    } else {
      s21_null_big_decimal(&res_big);
      res_big.sign = 0;
    }
  }

  res_big.scale = target_scale;

  // === ВАЖНО: Проверяем и обрабатываем переполнение ===
  int has_overflow = 0;
  for (int i = 3; i < 8; i++) {
    if (res_big.bits[i] != 0) {
      has_overflow = 1;
      break;
    }
  }

  if (has_overflow) {
    printf("Applying bankers round (bits[3]=%08x)\n", res_big.bits[3]);

    if (s21_big_apply_bankers_round(&res_big) != OK) {
      printf("%s\n", "ошибка №5");
      return CALCULATION_ERROR;
    }

    for (int i = 3; i < 8; i++) {
      if (res_big.bits[i] != 0) {
        printf("FATAL: bits[%d] = %08x after rounding\n", i, res_big.bits[i]);
        printf("%s\n", "ошибка №6");
        return CALCULATION_ERROR;
      }
    }

    // При округлении scale должен уменьшиться на 1
    if (res_big.scale > 0) {
      res_big.scale--;
    }
  }
  if (res_big.scale == 0) {
    // Проверяем, не превышает ли мантисса максимальное значение
    int is_greater_than_max = 0;

    if (res_big.bits[2] > 0x1F3F) {
      is_greater_than_max = 1;
    } else if (res_big.bits[2] == 0x1F3F) {
      if (res_big.bits[1] > 0xFFFFFFFF) {
        is_greater_than_max = 1;
      } else if (res_big.bits[1] == 0xFFFFFFFF) {
        if (res_big.bits[0] > 0xFFFFFFFF) {
          is_greater_than_max = 1;
        }
      }
    }

    if (is_greater_than_max) {
        if (res_big.sign == 0) {
            return 1;  // 1 - слишком велико
        } else {
            return 2;  // 2 - слишком мало
        }
    }
  }
  printf("Final res_big before conversion:\n");
  printf("  bits[0-3]: %08x %08x %08x %08x\n", res_big.bits[0], res_big.bits[1],
         res_big.bits[2], res_big.bits[3]);
  printf("  scale = %d, sign = %d\n", res_big.scale, res_big.sign);

  s21_big_to_decimal_internal(&res_big, result);

  return OK;
}

// Округляет 224-битное значение до 96 бит по правилу "половина к чётному"
// Возвращает OK, если успешно; ERROR, если переполнение даже после округления
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
