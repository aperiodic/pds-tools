#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

typedef struct TokenStream {
    Token* tokens;
    int pos;
    int size;
    int capacity;
} TokenStream;


TokenStream* new_token_stream(int capacity);
void destroy_token_stream(TokenStream* stream);

int insert_token(TokenStream* stream, Token token);
void rewind_stream(TokenStream* stream);
Token tail_peek(TokenStream* stream);
Token* next_token(TokenStream* stream);

#endif
