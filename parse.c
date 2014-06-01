#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "buff.h"

#define MAX_TOKENS 1e5
#define TOKENIZING_BUFFER_SIZE 1024


//
// Data Types
//

// Grand Type Enum

typedef enum ObjectType {
    TKN_EQUALS = 1,
    TKN_LEFT_PAREN,
    TKN_RIGHT_PAREN,
    TKN_STRING,
    TKN_DATE,
    TKN_INTEGER,
    TKN_RATIONAL,
    TKN_UNIT,
    CFG_STRING,
    CFG_NUMBER_INTEGRAL,
    CFG_NUMBER_RATIONAL,
    CFG_ASSOCIATION_TUPLE,
    CFG_ASSOCIATION_PRIMITIVE,
    CFG_OBJECT,
    CFG_LABEL
} ObjectType;

// CFG Types

typedef struct StrVal {
    char type;
    char* value;
} StrVal;

typedef struct NumVal {
    char type;
    int int_value;
    float float_value;
    char* units;
} NumVal;

typedef union Value {
    StrVal string;
    NumVal number;
} Value;

typedef struct TupleAssoc {
    char type;
    char* key;
    char size;
    Value* values;
} TupleAssoc;

typedef struct PrimAssoc {
    char type;
    char* key;
    Value value;
} PrimAssoc;

typedef union Association {
    PrimAssoc prim;
    TupleAssoc tuple;
} Association;

typedef struct PDSObject {
    char type;
    char* name;
    Association* assocs;
} PDSObject;

typedef struct Label {
    char type;
    char* version;
    Association* assocs;
    PDSObject* objs;
} Label;

typedef union CFGTerm {
    StrVal string;
    NumVal number;
    TupleAssoc tuple_assoc;
    PrimAssoc primitive_assoc;
    PDSObject object;
    Label label;
} CFGTerm;

//
// Token Types
//

typedef struct TknGeneric {
    char type;
    char* str;
} TknGeneric;

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
    TknEquals equals;
    TknString string;
    TknDate date;
    TknInteger integer;
    TknRational rational;
    TknUnit unit;
} Token;

//
// Token Constructors
//

Token new_onechar_token(char c, enum ObjectType t) {
    TknEquals token;
    token.type = t;
    token.str = malloc(sizeof(char) * 2);
    token.str[0] = c;
    token.str[1] = '\0';

    return (Token) token;
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

//
// Tokenizing
//

typedef struct TokenStream {
    Token* tokens;
    int pos;
    int size;
    int capacity;
} TokenStream;

enum tkn_sm_state {
    TKN_SM_WHITESPACE,
    TKN_SM_COMMENT,
    TKN_SM_COMMENT_PENULTIMATE,
    TKN_SM_IDENTIFIER,
    TKN_SM_ADD_RIGHT_PAREN,
    TKN_SM_UNIT,
    TKN_SM_STRING_LITERAL,
    TKN_SM_DATE_OR_INT_OR_RATIONAL,
    TKN_SM_DATE,
    TKN_SM_INT_OR_RATIONAL,
    TKN_SM_RATIONAL
};

TokenStream* new_token_stream(int capacity) {
    TokenStream* ts = malloc(sizeof(TokenStream));
    ts->pos = 0;
    ts->size = 0;
    ts->capacity = capacity;
    ts->tokens = malloc(sizeof(Token) * capacity);

    return ts;
}

void destroy_token_stream(TokenStream* stream) {
    free(stream->tokens);
    free(stream);
}

void rewind_stream(TokenStream* stream) {
    stream->pos = 0;
}

Token* next_token(TokenStream* stream) {
    if (stream->pos >= stream->size) {
        return NULL;
    } else {
        return &stream->tokens[stream->pos++];
    }
}

int insert_token(TokenStream* stream, Token token) {
    if (stream->size < stream->capacity) {
        stream->tokens[stream->size++] = token;
        return 1;
    }
    return 0;
}

#define is_whitespace(c) (c == ' ' || c == ',' || c == '\t' || c == '\n' || c == '\r')

enum tkn_sm_state tkn_sm_step( char head
                             , enum tkn_sm_state state
                             , CharBuff* curr_token
                             , Token* finished
                             ) {
    switch (state) {
        case TKN_SM_WHITESPACE:
            if (is_whitespace(head)) {
                return TKN_SM_WHITESPACE;
            }
            if (head == '/') {
                return TKN_SM_COMMENT;
            }
            if (head == '"') {
                return TKN_SM_STRING_LITERAL;
            }
            if (head == '<') {
                return TKN_SM_UNIT;
            }
            if (head == '=') {
                Token eq_token = new_token_equals();
                *finished = eq_token;
                return TKN_SM_WHITESPACE;
            }
            if (head == '(') {
                Token lp_token = new_token_left_paren();
                *finished = lp_token;
                return TKN_SM_WHITESPACE;
            }
            if (head == ')') {
                Token rp_token = new_token_right_paren();
                *finished = rp_token;
                return TKN_SM_WHITESPACE;
            }
            if (head == '-') {
                insert(curr_token, (int) head);
                return TKN_SM_INT_OR_RATIONAL;
            }
            if (head >= '0' && head <= '9') {
                insert(curr_token, (int) head);
                return TKN_SM_DATE_OR_INT_OR_RATIONAL;
            }
            if (head >= 'A' && head <= 'Z') {
                insert(curr_token, (int) head);
                return TKN_SM_IDENTIFIER;
            }
            if (head == '^') {
                insert(curr_token, (int) head);
                return TKN_SM_IDENTIFIER;
            }
            break;

        case TKN_SM_COMMENT:
            if (head == '*') {
                return TKN_SM_COMMENT_PENULTIMATE;
            } else {
                return TKN_SM_COMMENT;
            }
            break;

        case TKN_SM_COMMENT_PENULTIMATE:
            if (head == '/') {
                return TKN_SM_WHITESPACE;
            } else {
                return TKN_SM_COMMENT;
            }
            break;

        case TKN_SM_IDENTIFIER:
            if (is_whitespace(head)) {
                Token identifier = new_token_string(curr_token);
                *finished = identifier;
                return TKN_SM_WHITESPACE;
            }
            if (head == ')') {
                Token identifier = new_token_string(curr_token);
                *finished = identifier;
                return TKN_SM_ADD_RIGHT_PAREN;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_IDENTIFIER;
            }
            break;

        case TKN_SM_ADD_RIGHT_PAREN:
            assert(is_whitespace(head));
            {
                Token rp = new_token_right_paren(')');
                *finished = rp;
                return TKN_SM_WHITESPACE;
            }
            break;

        case TKN_SM_UNIT:
            if (head == '>') {
                Token unit = new_token_unit(curr_token);
                *finished = unit;
                return TKN_SM_WHITESPACE;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_UNIT;
            }

        case TKN_SM_STRING_LITERAL:
            if (head == '"') {
                Token string = new_token_string(curr_token);
                *finished = string;
                return TKN_SM_WHITESPACE;
            }
            if (head == '\n' || head == '\r') {
                return TKN_SM_STRING_LITERAL;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_STRING_LITERAL;
            }

        case TKN_SM_DATE_OR_INT_OR_RATIONAL:
            if (is_whitespace(head)) {
                // if the token ends and we're in this state, the token must be
                // an integer (since we didn't see anything that indicated
                // otherwise).
                Token integer = new_token_integer(curr_token);
                *finished = integer;
                return TKN_SM_WHITESPACE;
            }
            if (head == '-') {
                insert(curr_token, (int) head);
                return TKN_SM_DATE;
            }
            if (head == '.') {
                insert(curr_token, (int) head);
                return TKN_SM_RATIONAL;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_DATE_OR_INT_OR_RATIONAL;
            }

        case TKN_SM_INT_OR_RATIONAL:
            if (is_whitespace(head)) {
                // if the token ends and we're in this state, the token must be
                // an integer (since we didn't see anything that indicated
                // that it's a rational).
                Token integer = new_token_integer(curr_token);
                *finished = integer;
                return TKN_SM_WHITESPACE;
            }
            if (head == '.') {
                insert(curr_token, (int) head);
                return TKN_SM_RATIONAL;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_INT_OR_RATIONAL;
            }

        case TKN_SM_DATE:
            if (is_whitespace(head)) {
                Token date = new_token_date(curr_token);
                *finished = date;
                return TKN_SM_WHITESPACE;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_DATE;
            }

        case TKN_SM_RATIONAL:
            if (is_whitespace(head)) {
                Token rational = new_token_rational(curr_token);
                *finished = rational;
                return TKN_SM_WHITESPACE;
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_RATIONAL;
            }
    }
}

TokenStream* tokenize(FILE* input) {
    TokenStream* stream = new_token_stream(MAX_TOKENS);

    enum tkn_sm_state state = TKN_SM_WHITESPACE;
    char next_char;

    Token null_token;
    memset(&null_token, 0, sizeof(Token));

    CharBuff* curr_token;
    Token completed_token;

token_setup:
    curr_token = new_buff(TOKENIZING_BUFFER_SIZE);
    memset(&completed_token, 0, sizeof(Token));

    while((next_char = fgetc(input)) != EOF) {
        state = tkn_sm_step(next_char, state, curr_token, &completed_token);

        if (memcmp(&completed_token, &null_token, sizeof(Token)) != 0) {
            //char* token_str = copy_contents(curr_token);
            //printf("%s\r\n", token_str);
            //free(token_str);
            insert_token(stream, completed_token);
        }

        if (state == TKN_SM_WHITESPACE) {
            goto token_setup;
        }
    }

    return stream;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: parse <file>");
        return -1;
    }

    char* filename = argv[1];
    FILE* pds = fopen(filename, "rt");

    TokenStream* tokens = tokenize(pds);
    printf("got %d tokens!\n", tokens->size);

    Token* curr;
    while ((curr = next_token(tokens)) != NULL) {
        char* name;
        switch(curr->generic.type) {
            case TKN_EQUALS:
                name = "EQUALS";
                break;
            case TKN_LEFT_PAREN:
                name = "LEFT PAREN";
                break;
            case TKN_RIGHT_PAREN:
                name = "RIGHT PAREN";
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
        printf("%s: %s\r\n", name, curr->generic.str);
    }

    fclose(pds);


}
