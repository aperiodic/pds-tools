#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"
#include "pool.h"
#include "token.h"
#include "token_stream.h"
#include "util.h"

#define MAX_LABEL_OBJECTS 10
#define MAX_LABEL_ASSOCS 100
#define MAX_OBJECT_ASSOCS 200

//
// CFG Term Constructors
//

PrimAssoc new_prim_assoc(char* key, Value value) {
    PrimAssoc assoc;
    assoc.type = CFG_ASSOCIATION_PRIMITIVE;
    assoc.key = key;
    assoc.value = value;
    return assoc;
}

TupleAssoc new_tuple_assoc(char* key, const TupleValue* values) {
    TupleAssoc assoc;
    assoc.type = CFG_ASSOCIATION_TUPLE;
    assoc.key = key;
    assoc.values = *values;
    return assoc;
}

StrVal new_shallow_str_val(char* contents) {
    StrVal val;
    val.type = CFG_STRING;
    val.value = contents;
    return val;
}

DateVal new_shallow_date_val(char* date_str) {
    DateVal val;
    val.type = CFG_DATE;
    val.value = date_str;
    return val;
}

IntVal new_int_val_with_shallow_units(int value, char* units) {
    IntVal val;
    val.type = CFG_NUMBER_INTEGRAL;
    val.value = value;
    val.units = units;
    return val;
}

IntVal new_int_val(int value) {
    return new_int_val_with_shallow_units(value, "");
}

FloatVal new_float_val_with_shallow_units(float value, char* units) {
    FloatVal val;
    val.type = CFG_NUMBER_RATIONAL;
    val.value = value;
    val.units = units;
    return val;
}

FloatVal new_float_val(float value) {
    return new_float_val_with_shallow_units(value, "");
}

//
// Parsing Functions
//

TupleValue parse_tuple(TokenStream* stream) {
    printf("WARNING: parse_tuple not implemented!\n");
    TupleValue tv;
    return tv;
}

Value parse_value(TokenStream* stream) {
    Token head = peek(stream);

#ifdef DEBUG
    printf("parsing value, head is %s \"%s\"\n", token_name(head), head.generic.str);
#endif

    if (head.generic.type == TKN_STRING || head.generic.type == TKN_IDENTIFIER) {
        TknGeneric* string = (TknGeneric*) next_token(stream);
#ifdef DEBUG
        printf("parsed string value \"%s\"\n", string->str);
#endif
        return (Value) new_shallow_str_val(string->str);
    } else if (head.generic.type == TKN_DATE) {
        TknDate* date = (TknDate*) next_token(stream);
        assert(date->type == TKN_DATE);
#ifdef DEBUG
        printf("parsed date value %s\n", date->str);
#endif
        return (Value) new_shallow_date_val(date->str);
    } else {
        assert(head.generic.type == TKN_INTEGER || head.generic.type == TKN_RATIONAL);
        Value value;

        if (head.generic.type == TKN_INTEGER) {
            TknInteger* integer = (TknInteger*) next_token(stream);
            value = (Value) new_int_val(integer->intval);
#ifdef DEBUG
            printf("parsed int value %i\n", value.integer.value);
#endif
        } else {
            // head must be a rational
            TknRational* rational = (TknRational*) next_token(stream);
            value = (Value) new_float_val(rational->floatval);
#ifdef DEBUG
            printf("parsed float value %f\n", value.rational.value);
#endif
        }

        Token after_val = peek(stream);
#ifdef DEBUG
        printf("parsing value, post-value token type is %s\n", token_name(after_val));
#endif
        if (after_val.generic.type == TKN_UNIT) {
            TknUnit* units = (TknUnit*) next_token(stream);
            if (value.generic.type == CFG_NUMBER_INTEGRAL) {
                IntVal* int_val = (IntVal*) &value;
                int_val->units = units->str;
            } else {
                FloatVal* float_val = (FloatVal*) &value;
                float_val->units = units->str;
            }
#ifdef DEBUG
            printf("parsed value units \"%s\"\n", units->str);
#endif
        }
        return value;
    }
}

Association parse_association(TokenStream* stream) {
    Token* key = next_token(stream);
#ifdef DEBUG
    printf("parsing association, key token is %s \"%s\"\n", token_name(*key), key->generic.str);
#endif
    assert(key->generic.type == TKN_IDENTIFIER);

    Token* equals = next_token(stream);
#ifdef DEBUG
    printf("parsing association, equals token is %s \"%s\"\n", token_name(*equals), equals->generic.str);
#endif
    assert(equals->generic.type == TKN_EQUALS);

    Association assoc;
    Token head = peek(stream);

#ifdef DEBUG
    printf("parsing association value, head is %s \"%s\"\n", token_name(head), head.generic.str);
#endif
    if (head.generic.type == TKN_LEFT_PAREN) {
        TupleValue tuple_val = parse_tuple(stream);
        assoc = (Association) new_tuple_assoc(key->generic.str, &tuple_val);
    } else {
        Value value = parse_value(stream);
        assoc = (Association) new_prim_assoc(key->generic.str, value);
    }

    Token* eol = next_token(stream);
    assert(eol->generic.type == TKN_EOL);

    return assoc;
}

PDSObject* parse_object(TokenStream* stream) {
    PDSObject* object = malloc(sizeof(PDSObject));
    object->type = CFG_OBJECT;

    Token* bo = next_token(stream);
    assert(bo->generic.type == TKN_BEGIN_OBJECT);

    Token* eq = next_token(stream);
    assert(eq->generic.type == TKN_EQUALS);

    Token* name = next_token(stream);
    assert(name->generic.type == TKN_IDENTIFIER);
    object->name = name->generic.str;
#ifdef DEBUG
    printf("parsing the %s object\n", object->name);
#endif

    Token* eol = next_token(stream);
    assert(eol->generic.type == TKN_EOL);

    AssociationPool* assoc_pool = new_assoc_pool(MAX_OBJECT_ASSOCS);
    object->assocs = assoc_pool->assocs;
    object->assoc_count = 0;

    Token head;
    while((head = peek(stream)).generic.type != TKN_END_OBJECT) {

#ifdef DEBUG
        printf("parsing object, head token is %s \"%s\"\n", token_name(head), head.generic.str);
#endif

        assert(head.generic.type == TKN_IDENTIFIER);
        Association* next_assoc_slot = allocate_assoc(assoc_pool);
        *next_assoc_slot = parse_association(stream);
    }

    next_token(stream); // consume end object token

    eq = next_token(stream);
    assert(eq->generic.type == TKN_EQUALS);

    TknIdentifier* end_name = (TknIdentifier*) next_token(stream);
    assert(end_name->type == TKN_IDENTIFIER);
    assert(strlen(object->name) == strlen(end_name->str));
    assert(memcmp(object->name, end_name->str, strlen(object->name)) == 0);

    eol = next_token(stream);
    assert(eol->generic.type == TKN_EOL);

    return object;
}

void parse_label_subterm( TokenStream* stream
                         , PDSObjectPool* object_pool
                         , AssociationPool* assoc_pool
                         , const Token* nullt
                         , PDSLabel* label
                         )
{
    static const char* version_str = "PDS_VERSION_ID";

    Token head = peek(stream);
    char* head_str = head.generic.str;

    assert(head.generic.type == TKN_IDENTIFIER
           || head.generic.type == TKN_BEGIN_OBJECT
           || head.generic.type == TKN_END);
    assert(memcmp(&head, &nullt, sizeof(Token)) != 0);

#ifdef DEBUG
    printf("parsing label subterm, head token is %s \"%s\"\n", token_name(head), head_str);
#endif

    if (head.generic.type == TKN_END) {
        return;
    }

    if (head.generic.type == TKN_EOL) {
        next_token(stream);
        return;
    }

    // parse object
    if (head.generic.type == TKN_BEGIN_OBJECT) {
        PDSObject* obj = parse_object(stream);
        PDSObject* next_obj_slot = allocate_pds_object(object_pool);
        *next_obj_slot = *obj;
        free(obj);
        label->object_count++;
        return;
    }

    // parse label metadata association
    Association assoc = parse_association(stream);
    if (strcmp(assoc.generic.key, version_str) == 0) {
        label->version = assoc.generic.key;
    } else {
        Association* next_assoc_slot = allocate_assoc(assoc_pool);
        *next_assoc_slot = assoc;
        label->assoc_count++;
    }
}

PDSLabel* parse_label(TokenStream* stream) {
    // return value
    PDSLabel* label = malloc(sizeof(PDSLabel));

    // struct pools
    // This program isn't long-running so dynamic memory management is more
    // trouble than it's worth. We'll just use some fixed-size pools that we
    // never free.
    PDSObjectPool* objects = new_pds_object_pool(MAX_LABEL_OBJECTS);
    AssociationPool* assocs = new_assoc_pool(MAX_LABEL_ASSOCS);

    label->objects = objects->objects;
    label->object_count = 0;
    label->assocs = assocs->assocs;
    label->assoc_count = 0;

    Token nullt = null_token();
    Token head;

    while((head = peek(stream)).generic.type != TKN_END) {
        parse_label_subterm(stream, objects, assocs, &nullt, label);
    }

    next_token(stream); // consume end token
    return label;
}
