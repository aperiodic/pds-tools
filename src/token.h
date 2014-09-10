#ifndef TOKEN_H
#define TOKEN_H

#include "types.h"
#include "buff.h"

//
// Typedefs
//

typedef struct TknGeneric {
    char type;
    char* str;
} TknGeneric;

typedef struct TknEOL {
    char type;
    char* str;
} TknEOL;

typedef struct TknEquals {
    char type;
    char* str;
} TknEquals;

typedef struct TknLeftParen {
    char type;
    char* str;
} TknLeftParen;

typedef struct TknRightParen {
    char type;
    char* str;
} TknRightParen;

typedef struct TknString {
    char type;
    char* str;
} TknString;

typedef struct TknIdentifier {
    char type;
    char* str;
} TknIdentifier;

typedef struct TknBeginObject {
  char type;
  char* str;
} TknBeginObject;

typedef struct TknEndObject {
  char type;
  char* str;
} TknEndObject;

typedef struct TknEnd {
  char type;
  char* str;
} TknEnd;

typedef struct TknDate {
    char type;
    char* str;
} TknDate;

typedef struct TknInteger {
    char type;
    char* str;
    unsigned long intval;
} TknInteger;

typedef struct TknRational {
    char type;
    char* str;
    float floatval;
} TknRational;

typedef struct TknUnit {
    char type;
    char* str;
} TknUnit;

typedef union Token {
    TknGeneric generic;
    TknEOL eol;
    TknEquals equals;
    TknRightParen right_paren;
    TknLeftParen left_paren;
    TknIdentifier identifier;
    TknBeginObject begin_object;
    TknEndObject end_object;
    TknEnd end;
    TknString string;
    TknDate date;
    TknInteger integer;
    TknRational rational;
    TknUnit unit;
} Token;

//
// Public API
//

Token new_onechar_token(char c, enum ObjectType t);
Token new_token_eol();
Token new_token_equals();
Token new_token_left_paren();
Token new_token_right_paren();
Token new_token_identifier(CharBuff* token_buff);
Token new_token_begin_object();
Token new_token_end_object();
Token new_token_end();
Token new_token_string(CharBuff* token_buff);
Token new_token_date(CharBuff* token_buff);
Token new_token_unit(CharBuff* token_buff);
Token new_token_integer(CharBuff* token_buff);
Token new_token_rational(CharBuff* token_buff);
Token null_token();

char* token_name(Token t);
void print_token(Token t);

#endif
