#include <stdlib.h>
#include <stdio.h>

#define MAX_TOKENS 1e6


//
// Data Types
//

// CFG Types

typedef struct StrVal {
    char type;
    char* value;
} StrVal;

typedef struct NumVal {
    char type;
    char numtype;
    int int_value;
    float float_value;
} NumVal;

typedef union Value {
    StrVal string;
    NumVal number;
} Value;

typedef struct PolyAssoc {
    char type;
    char* key;
    char size;
    Value* values;
} PolyAssoc;

typedef struct PrimAssoc {
    char type;
    char* key;
    Value value;
} PrimAssoc;

typedef union Association {
    PrimAssoc prim;
    PolyAssoc poly;
} Association;

typedef struct PDSObj {
    char* name;
    Association* assocs;
} PDSObject;

typedef struct Label {
    char* version;
    Association* assocs;
    PDSObject* objs;
} Label;

// Token Types

// Tokens:
//      1. equals - =
//      2. version id - PDS_VERSION_ID
//      3. begin object - OBJECT
//      4. end object - EOND_OBJECT
//      5. end - END
//      6. key - ^?[A_Z]+
//      7. string value
//      8. numeric value

typedef struct TknEquals {
    char type;
    char* str;
} TknEquals;

typedef struct TknString {
    char type;
    char* str;
} TknString;

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

typedef union Token {
    TknEquals equals;
    TknString string;
    TknInteger integer;
    TknRational rational;
} Token;


//
// Tokenize
//

Token* tokenize(FILE* stream) {
    Token* tokens = malloc(sizeof(Token) * MAX_TOKENS);


    return tokens;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: parse <file>");
        return -1;
    }

    char* filename = argv[0];
    FILE* pds = fopen(filename, "rt");

    tokenize(pds);

    fclose(pds);


}
