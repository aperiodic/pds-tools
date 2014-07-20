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

typedef union PrimitiveValue {
    GenericVal generic;
    StrVal string;
    DateVal date;
    IntVal integer;
    FloatVal rational;
} PrimitiveValue;

typedef struct TupleValue {
  char type;
  char count;
  PrimitiveValue* values;
} TupleValue;

typedef union Value {
  GenericVal generic;
  PrimitiveValue primitive;
  TupleValue tuple;
} Value;

typedef struct Association {
    char type;
    char* key;
    Value value;
} Association;

// Not actually a CFG term, but has to be defined here.
typedef struct AssociationPool {
  Association* assocs;
  int capacity;
  int pos;
} AssociationPool;

typedef struct PDSObject {
    char type;
    char* name;
    struct hashtable* attrs;
    AssociationPool* assoc_pool;
} PDSObject;

// Not actually a CFG term, but has to be defined here.
typedef struct PDSObjectPool {
  PDSObject* objects;
  int capacity;
  int pos;
} PDSObjectPool;

typedef struct PDSLabel {
    char type;
    char* version;
    struct hashtable* metadata;
    AssociationPool* assoc_pool;
    struct hashtable* objects;
    PDSObjectPool* object_pool;
} PDSLabel;

typedef union CFGTerm {
    GenericTerm generic;
    StrVal string;
    IntVal integer;
    FloatVal rational;
    Association assoc;
    PDSObject object;
    PDSLabel label;
} CFGTerm;

#endif
