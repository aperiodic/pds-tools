#ifndef TOKEN_H
#define TOKEN_H

#include "types.h"
#include "buff.h"

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

typedef struct TknDate {
    char type;
    char* str;
} TknDate;

typedef struct TknInteger {
    char type;
    char* str;
    int intval;
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
    TknString string;
    TknDate date;
    TknInteger integer;
    TknRational rational;
    TknUnit unit;
} Token;


Token new_onechar_token(char c, enum ObjectType t);
Token new_token_eol();
Token new_token_equals();
Token new_token_left_paren();
Token new_token_right_paren();
Token new_token_identifier(CharBuff* token_buff);
Token new_token_string(CharBuff* token_buff);
Token new_token_date(CharBuff* token_buff);
Token new_token_unit(CharBuff* token_buff);
Token new_token_integer(CharBuff* token_buff);
Token new_token_rational(CharBuff* token_buff);

#endif