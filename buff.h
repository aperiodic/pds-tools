#include <stdint.h>

typedef struct CharBuff {
  char* chars;
  uint32_t size;
  uint32_t capacity;
  uint32_t pos;
} CharBuff;

CharBuff* new_buff(int size);
int insert(CharBuff* buff, int c);
char* copy_contents(CharBuff* buff);
void reset_buff(CharBuff* buff);
void destroy_buff(CharBuff* buff);
