#include <string.h>

#include "util.h"

unsigned int djb2_hash(void* str) {
    char* s = (char*) str;
    unsigned int hash = 0;
    int c;

    while (c = *s++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int voidstrs_eq(void* str1, void* str2) {
    return strcmp((char*) str1, (char*) str2) == 0;
}
