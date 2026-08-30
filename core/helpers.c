#include <../headers/s21_helpers.h>


// получить значение (0 или 1) конкретного бита по его индексу (0-95)
int s21_get_bit(s21_decimal value, int bit_index) {
    if (bit_index < 0 || bit_index >= 96) {
        return 0;
    }
    int word_index = bit_index / 32;  
    int bit_in_word = bit_index % 32; 
    
    return (value.bits[word_index] >> bit_in_word) & 1; 
}

// установить бит с индексом в значение 0 или 1
int s21_set_bit(s21_decimal* value, int bit_index, int bit_value) {
    if (!value || bit_index < 0 || bit_index >= 96) {
        return 0;
    }
    int word_index = bit_index / 32;
    int bit_in_word = bit_index % 32;
    if (bit_value) {
        value->bits[word_index] |= (1 << bit_in_word);
    } else {
        value->bits[word_index] &= ~(1 << bit_in_word);
    }
    return 0;
}