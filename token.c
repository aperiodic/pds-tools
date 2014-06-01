#include <stdlib.h>
#include <stdio.h>

#include "token.h"

Token new_onechar_token(char c, enum ObjectType t) {
    TknEquals token;
    token.type = t;
    token.str = malloc(sizeof(char) * 2);
    token.str[0] = c;
    token.str[1] = '\0';
    return (Token) token;
}

Token new_token_eol() {
    return new_onechar_token('\n', TKN_EOL);
}

Token new_token_equals() {
    return new_onechar_token('=', TKN_EQUALS);
}

Token new_token_left_paren() {
    return new_onechar_token('(', TKN_LEFT_PAREN);
}

Token new_token_right_paren() {
    return new_onechar_token(')', TKN_RIGHT_PAREN);
}

Token new_token_identifier(CharBuff* token_buff) {
    TknIdentifier token;
    token.type = TKN_IDENTIFIER;
    token.str = copy_contents(token_buff);
    return (Token) token;
}

Token new_token_string(CharBuff* token_buff) {
    TknString token;
    token.type = TKN_STRING;
    token.str = copy_contents(token_buff);
    return (Token) token;
}

Token new_token_date(CharBuff* token_buff) {
    TknDate token;
    token.type = TKN_DATE;
    token.str = copy_contents(token_buff);
    return (Token) token;
}

Token new_token_unit(CharBuff* token_buff) {
    TknUnit token;
    token.type = TKN_UNIT;
    token.str = copy_contents(token_buff);
    return (Token) token;
}

Token new_token_integer(CharBuff* token_buff) {
    TknInteger token;
    token.type = TKN_INTEGER;
    token.str = copy_contents(token_buff);
    sscanf(token.str, "%d", &token.intval);
    return (Token) token;
}

Token new_token_rational(CharBuff* token_buff) {
    TknRational token;
    token.type = TKN_RATIONAL;
    token.str = copy_contents(token_buff);
    sscanf(token.str, "%f", &token.floatval);
    return (Token) token;
}

void print_token(Token t) {
    char* name;
    switch(t.generic.type) {
        case TKN_EOL:
            name = "EOL";
            break;
        case TKN_EQUALS:
            name = "EQUALS";
            break;
        case TKN_LEFT_PAREN:
            name = "LEFT PAREN";
            break;
        case TKN_RIGHT_PAREN:
            name = "RIGHT PAREN";
            break;
        case TKN_IDENTIFIER:
            name = "IDENTIFIER";
            break;
        case TKN_STRING:
            name = "STRING";
            break;
        case TKN_DATE:
            name = "DATE";
            break;
        case TKN_INTEGER:
            name = "INTEGER";
            break;
        case TKN_RATIONAL:
            name = "RATIONAL";
            break;
        case TKN_UNIT:
            name = "UNIT";
            break;
    }
    printf("%s: %s\r\n", name, t.generic.str);
}
