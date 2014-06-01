#include <stdlib.h>

#include "token.h"
#include "token_stream.h"

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

Token peek(TokenStream* stream) {
    return stream->tokens[stream->pos];
}

int insert_token(TokenStream* stream, Token token) {
    if (stream->size < stream->capacity) {
        stream->tokens[stream->size++] = token;
        return 1;
    }
    return 0;
}
