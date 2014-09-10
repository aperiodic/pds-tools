#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buff.h"
#include "token.h"

//
// Constructor Utilities
//

bool int_literal_has_radix(const CharBuff* literal) {
    for (int i = 0; i < literal->size; i++) {
        if (literal->chars[i] == '#') {
            return true;
        }

        if (i >= 2) {
            return false; // since base >= 100 is impractical in ascii
        }
    }
    return false;
}

int hex_digit_value(char digit) {
    if (digit < '0') {
        return 0;
    } else if (digit <= '9') {
        return digit - '0';
    } else if (digit >= 'A' && digit <= 'Z') {
        return digit - 'A' + 10;
    } else if (digit >= 'a' && digit <= 'z') {
        return digit - 'a' + 10;
    } else {
        return 0;
    }
}

unsigned long parse_radixed_int_literal(const CharBuff* literal) {
    int radix = 0;
    sscanf(literal->chars, "%i", &radix);
#ifdef DEBUG
    printf("radixed literal's radix is %d\n", radix);
#endif

    int radix_pos = 0;
    while(literal->chars[radix_pos] != '#') {
        radix_pos++;
    }
    char* digits = &(literal->chars[radix_pos+1]);

    unsigned long value = 0;
    unsigned long pow = 1;
    int digit_count = literal->size - radix_pos - 2;
    for (int i = 0; i < digit_count; i++) {
        int digit = hex_digit_value(digits[i]);
        value += digit * pow;
#ifdef DEBUG
        printf("radixed literal's next digit has value %u, pow is %u, value is now %u\n", digit, pow, value);
#endif
        pow *= radix;
    }

    return value;
}

//
// Token Constructors
//

Token new_onechar_token(char c, enum ObjectType t) {
    TknGeneric token;
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

Token new_fixed_str_token(char* contents, enum ObjectType t) {
    char content_bytes = strlen(contents) + 1;
    TknGeneric token;
    token.type = t;
    token.str = malloc(sizeof(char) * content_bytes);
    memcpy(token.str, contents, content_bytes);
    return (Token) token;
}

Token new_token_begin_object() {
    return new_fixed_str_token("OBJECT", TKN_BEGIN_OBJECT);
}

Token new_token_end_object() {
    return new_fixed_str_token("END_OBJECT", TKN_END_OBJECT);
}

Token new_token_end() {
    return new_fixed_str_token("END", TKN_END);
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
    if (int_literal_has_radix(token_buff)) {
        token.intval = parse_radixed_int_literal(token_buff);
#ifdef DEBUG
        printf("integer literal %s has value %u\n", token_buff->chars, token.intval);
#endif
    } else {
        sscanf(token.str, "%d", &token.intval);
    }
    return (Token) token;
}

Token new_token_rational(CharBuff* token_buff) {
    TknRational token;
    token.type = TKN_RATIONAL;
    token.str = copy_contents(token_buff);
    sscanf(token.str, "%f", &token.floatval);
    return (Token) token;
}

Token null_token() {
    Token null;
    memset(&null, 0, sizeof(Token));
    return null;
}

char* token_name(Token t) {
    switch(t.generic.type) {
        case TKN_EOL:
            return "EOL";
        case TKN_EQUALS:
            return "EQUALS";
        case TKN_LEFT_PAREN:
            return "LEFT PAREN";
        case TKN_RIGHT_PAREN:
            return "RIGHT PAREN";
        case TKN_IDENTIFIER:
            return "IDENTIFIER";
        case TKN_BEGIN_OBJECT:
            return "OBJECT";
        case TKN_END_OBJECT:
            return "END_OBJECT";
        case TKN_END:
            return "END";
        case TKN_STRING:
            return "STRING";
        case TKN_DATE:
            return "DATE";
        case TKN_INTEGER:
            return "INTEGER";
        case TKN_RATIONAL:
            return "RATIONAL";
        case TKN_UNIT:
            return "UNIT";
    }
}

void print_token(Token t) {
    printf("%s: %s\r\n", token_name(t), t.generic.str);
}
