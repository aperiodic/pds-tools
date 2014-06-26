#ifndef CFG_H
#define CFG_H

#include "token_stream.h"

typedef struct GenericTerm {
    char type;
} GenericTerm;

typedef struct GenericVal {
    char type;
} GenericVal;

typedef struct StrVal {
    char type;
    char* value;
} StrVal;

typedef struct DateVal {
    char type;
    char* value;
} DateVal;

typedef struct IntVal {
    char type;
    int value;
    char* units;
} IntVal;

typedef struct FloatVal {
    char type;
    float value;
    char* units;
} FloatVal;

typedef union Value {
    GenericVal generic;
    StrVal string;
    DateVal date;
    IntVal integer;
    FloatVal rational;
} Value;

typedef struct TupleValue {
  char type;
  char count;
  Value* values;
} TupleValue;

typedef struct TupleAssoc {
    char type;
    char* key;
    TupleValue values;
} TupleAssoc;

typedef struct PrimAssoc {
    char type;
    char* key;
    Value value;
} PrimAssoc;

typedef struct GenericAssoc {
    char type;
    char* key;
} GenericAssoc;

typedef union Association {
    GenericAssoc generic;
    PrimAssoc prim;
    TupleAssoc tuple;
} Association;

typedef struct PDSObject {
    char type;
    char* name;
    char assoc_count;
    Association* assocs;
} PDSObject;

typedef struct PDSLabel {
    char type;
    char* version;
    int assoc_count;
    Association* assocs;
    int object_count;
    PDSObject* objects;
} PDSLabel;

typedef union CFGTerm {
    GenericTerm generic;
    StrVal string;
    IntVal integer;
    FloatVal rational;
    TupleAssoc tuple_assoc;
    PrimAssoc primitive_assoc;
    PDSObject object;
    PDSLabel label;
} CFGTerm;


PDSLabel* parse_label(TokenStream* stream);

#endif
