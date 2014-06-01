#ifndef TYPES_H
#define TYPES_H

//
// Grand Type Enum
//

typedef enum ObjectType {
    TKN_EOL = 1,
    TKN_EQUALS,
    TKN_LEFT_PAREN,
    TKN_RIGHT_PAREN,
    TKN_STRING,
    TKN_IDENTIFIER,
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

#endif
