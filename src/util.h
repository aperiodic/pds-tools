#ifndef UTIL_H
#define UTIL_H

#define min(x,y) ((x)<(y) ? (x) : (y))

unsigned int djb2_hash(void* str);
int voidstrs_eq(void* str1, void* str2);

#endif
