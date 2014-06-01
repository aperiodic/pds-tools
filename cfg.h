#ifndef CFG_H
#define CFG_H

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

#endif
