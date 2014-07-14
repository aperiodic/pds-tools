#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "buff.h"
#include "cfg.h"
#include "token.h"
#include "token_stream.h"

#define MAX_TOKENS 1e5
#define TOKENIZING_BUFFER_SIZE 2048

#define is_whitespace(c) (c == ' ' || c == ',' || c == '\t' || c == '\n' || c == '\r')


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

enum tkn_sm_state tkn_sm_step( char head
                             , enum tkn_sm_state state
                             , CharBuff* curr_token
                             , Token* finished
                             , Token* tail
                             )
{
    static const char* begin_object_str = "OBJECT";
    static const char* end_object_str = "END_OBJECT";
    static const char* end_str = "END";

    switch (state) {
        case TKN_SM_WHITESPACE:
            if (head == '\n') {
                Token eol_token = new_token_eol();
                if (tail->generic.type != TKN_EOL) {
                    *finished = eol_token;
                }
                return TKN_SM_WHITESPACE;
            }
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
            if (is_whitespace(head) || head == ')') {
                // check if it's a reserved identifier
                if (strcmp(curr_token->chars, begin_object_str) == 0) {
                    Token begin_object = new_token_begin_object();
                    *finished = begin_object;
                } else if (strcmp(curr_token->chars, end_object_str) == 0) {
                    Token end_object = new_token_end_object();
                    *finished = end_object;
                } else if (strcmp(curr_token->chars, end_str) == 0) {
                    Token end = new_token_end();
                    *finished = end;
                } else {
                    Token identifier = new_token_identifier(curr_token);
                    *finished = identifier;
                }

                if (head == ')') {
                    return TKN_SM_ADD_RIGHT_PAREN;
                } else {
                    return TKN_SM_WHITESPACE;
                }
            } else {
                insert(curr_token, (int) head);
                return TKN_SM_IDENTIFIER;
            }
            break;

        case TKN_SM_ADD_RIGHT_PAREN:
            assert(is_whitespace(head));
            {
                Token rp = new_token_right_paren();
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
            }
            if (is_whitespace(head)) {
                char tc = tail_char(curr_token);
                if (!is_whitespace(tc) || tc == ',') {
                    insert(curr_token, (int) head);
                }
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

    Token nullt = null_token();

    CharBuff* curr_token;
    Token completed_token;
    Token tail_token;

token_setup:
    curr_token = new_buff(TOKENIZING_BUFFER_SIZE);
    memset(&completed_token, 0, sizeof(Token));

    memset(&tail_token, 0, sizeof(Token));
    tail_token = tail_peek(stream);

    while((next_char = fgetc(input)) != EOF) {
        state = tkn_sm_step(next_char, state, curr_token, &completed_token, &tail_token);

        if (memcmp(&completed_token, &nullt, sizeof(Token)) != 0) {
            insert_token(stream, completed_token);
        }

        if (state == TKN_SM_WHITESPACE) {
            goto token_setup;
        }
    }

    rewind_stream(stream);
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
    fclose(pds);
    printf("got %d tokens!\n", tokens->size);

    PDSLabel* label = parse_label(tokens);
    printf("got a version %s label with %d metadata associations and %d objects\n", label->version, label->assoc_count, label->object_count);
}
