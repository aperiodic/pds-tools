#include <stdlib.h>
#include <stdio.h>

#include "buff.h"


CharBuff* new_buff(int size) {
    CharBuff* buff = malloc(sizeof(CharBuff));
    buff->pos = 0;
    buff->size = size;

    char* chars = malloc(sizeof(char) * size);
    buff->chars = chars;

    return buff;
}

int insert(CharBuff* buff, int c) {
    if (c == EOF) return EOF;
    if (buff->pos >= buff->size) return EOF;

    buff->chars[buff->pos++] = c;
    if (buff->pos > buff->capacity) {
        buff->capacity = buff->pos;
    }
    return (int) c;
}

char* copy_contents(CharBuff* buff) {
    char* copy = malloc(sizeof(char) * buff->capacity);
    for (int i = 0; i < buff->capacity; i++) {
        copy[i] = buff->chars[i];
    }
    return copy;
}

void reset_buff(CharBuff* buff) {
    buff->capacity = 0;
    buff->pos = 0;
}

void destroy_buff(CharBuff* buff) {
    free(buff->chars);
    free(buff);
}
