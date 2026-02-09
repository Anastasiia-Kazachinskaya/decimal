#include <check.h>
#include <string.h>
#include <stdio.h>

// ИСКУССТВЕННАЯ структура с гарантированным padding (для демонстрации)
typedef struct {
    unsigned int bits[3];  // 12 байт
    char         dummy;    // 1 байт → требует выравнивания следующего поля!
    unsigned int scale;    // 4 байта → должен начинаться с адреса % 4 == 0
    unsigned int sign;     // 4 байта
} s21_decimal_padded;

// Вспомогательная функция для дампа памяти
static void dump_memory(const char* label, const void* ptr, size_t size) {
    printf("%-25s: ", label);
    const unsigned char* bytes = (const unsigned char*)ptr;
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

// Тест: показывает, как {0} НЕ обнуляет padding
START_TEST(test_literal_fails_with_padding) {
    printf("\n");
    printf("========================================\n");
    printf("ДЕМОНСТРАЦИЯ УЯЗВИМОСТИ {0} К PADDING\n");
    printf("========================================\n");
    printf("Структура с искусственным padding:\n");
    printf("  bits[3] (12 байт) + dummy (1) + padding (3) + scale (4) + sign (4)\n");
    printf("  Итого: %zu байт (данные: 21, padding: %zu)\n\n", 
           sizeof(s21_decimal_padded), 
           sizeof(s21_decimal_padded) - 21);

    // Создаём два объекта через {0}
    s21_decimal_padded d1 = (s21_decimal_padded){0};
    s21_decimal_padded d2 = (s21_decimal_padded){0};
    
    // Загрязняем память ДО инициализации (как может быть в реальности)
    memset(&d1, 0xCC, sizeof(d1));  // мусор 0xCC во всей памяти
    memset(&d2, 0xCC, sizeof(d2));
    
    // Теперь "обнуляем" через составной литерал {0}
    d1 = (s21_decimal_padded){0};  // ← ОПАСНО: не трогает padding!
    d2 = (s21_decimal_padded){0};
    
    // Искусственно "испортим" padding первого объекта (как может сделать компилятор)
    unsigned char* bytes1 = (unsigned char*)&d1;
    if (sizeof(s21_decimal_padded) > 21) {
        // Padding находится между dummy и scale (после 13-го байта)
        for (size_t i = 13; i < 16; i++) {  // 3 байта padding
            bytes1[i] = 0xFF;  // мусор в зоне padding
        }
    }
    
    // Выводим побайтовое представление
    dump_memory("d1 (с мусором в padding)", &d1, sizeof(d1));
    dump_memory("d2 (чистый)", &d2, sizeof(d2));
    
    // Логически оба объекта — нули!
    ck_assert_int_eq(d1.bits[0], 0);
    ck_assert_int_eq(d1.bits[1], 0);
    ck_assert_int_eq(d1.bits[2], 0);
    ck_assert_int_eq(d1.dummy, 0);
    ck_assert_int_eq(d1.scale, 0);
    ck_assert_int_eq(d1.sign, 0);
    
    ck_assert_int_eq(d2.bits[0], 0);
    ck_assert_int_eq(d2.bits[1], 0);
    ck_assert_int_eq(d2.bits[2], 0);
    ck_assert_int_eq(d2.dummy, 0);
    ck_assert_int_eq(d2.scale, 0);
    ck_assert_int_eq(d2.sign, 0);
    
    // НО ПОБАЙТОВО ОНИ РАЗНЫЕ!
    int cmp = memcmp(&d1, &d2, sizeof(s21_decimal_padded));
    ck_assert_msg(
        cmp != 0,
        "❌ БАГ: два логических нуля не равны побайтово из-за мусора в padding!"
    );
    
    printf("\n⚠️  memcmp() вернул %d → объекты НЕ равны!\n", cmp);
    printf("   Это сломает:\n");
    printf("   • хеширование (разные хеши для одинаковых значений)\n");
    printf("   • сериализацию (бинарный дамп содержит мусор)\n");
    printf("   • кэширование (один и тот же объект не найдётся в кэше)\n");
}
END_TEST

// Тест: memset всегда безопасен
START_TEST(test_memset_always_works) {
    printf("\n");
    printf("========================================\n");
    printf("ДЕМОНСТРАЦИЯ НАДЁЖНОСТИ MEMSET\n");
    printf("========================================\n");
    
    s21_decimal_padded d;
    memset(&d, 0xCC, sizeof(d));  // заполняем мусором
    
    printf("До обнуления:          ");
    dump_memory("", &d, sizeof(d));
    
    // Обнуляем через memset — как делает s21_null_decimal
    memset(&d, 0, sizeof(d));
    
    printf("После memset:          ");
    dump_memory("", &d, sizeof(d));
    
    // Проверяем ВСЕ байты, включая padding
    unsigned char* bytes = (unsigned char*)&d;
    for (size_t i = 0; i < sizeof(d); i++) {
        ck_assert_uint_eq(bytes[i], 0);
    }
    
    printf("\n✅ ВСЕ %zu байт обнулены, включая padding!\n", sizeof(d));
}
END_TEST

Suite* padding_demo_suite(void) {
    Suite *s = suite_create("Padding Vulnerability Demo");
    TCase *tc = tcase_create("Demo");
    tcase_add_test(tc, test_literal_fails_with_padding);
    tcase_add_test(tc, test_memset_always_works);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = padding_demo_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    printf("\n");
    printf("========================================\n");
    printf("ВЫВОД ДЛЯ КОМАНДЫ:\n");
    printf("========================================\n");
    printf("1. Составной литерал {0} обнуляет ТОЛЬКО поля структуры.\n");
    printf("2. Padding (выравнивающие байты) остаётся с мусором.\n");
    printf("3. Мусор в padding ломает memcmp(), хеширование, сериализацию.\n");
    printf("4. memset() обнуляет ВСЮ память — поля + padding.\n");
    printf("5. Функция 'обнулить ВСЕ биты' ДОЛЖНА использовать memset().\n");
    printf("========================================\n\n");
    
    return (failed == 0) ? 0 : 1;
}