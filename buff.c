#include <stdlib.h>
#include <stdio.h>

#include "buff.h"


CharBuff* new_buff(int capacity) {
    CharBuff* buff = malloc(sizeof(CharBuff));
    buff->pos = 0;
    buff->size = 0;
    buff->capacity = capacity;

    char* chars = malloc(sizeof(char) * capacity);
    buff->chars = chars;

    return buff;
}

int insert(CharBuff* buff, int c) {
    if (c == EOF) return EOF;
    if (buff->size >= buff->capacity) return EOF;

    buff->chars[buff->pos++] = (char) c;
    if (buff->pos > buff->size) {
        buff->size = buff->pos;
    }
    return (int) c;
}

char* copy_contents(CharBuff* buff) {
    char* copy = malloc(sizeof(char) * (1 + buff->size));
    for (int i = 0; i < buff->size; i++) {
        copy[i] = buff->chars[i];
    }
    copy[buff->size] = '\0';
    return copy;
}

char tail_char(CharBuff* buff) {
    if (buff->size <= 0) {
        return '\0';
    } else  {
        return buff->chars[buff->size - 1];
    }
}

void reset_buff(CharBuff* buff) {
    buff->size = 0;
    buff->pos = 0;
}

void destroy_buff(CharBuff* buff) {
    free(buff->chars);
    free(buff);
}
