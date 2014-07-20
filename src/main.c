#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "hashtable_itr.h"

#include "buff.h"
#include "cfg.h"
#include "parse.h"
#include "token.h"
#include "token_stream.h"
#include "tokenize.h"

DEFINE_HASHTABLE_SEARCH(ht_search_for_obj, char, PDSObject);
DEFINE_HASHTABLE_SEARCH(ht_search_for_value, char, Value);

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: parse <file>\n");
        return EINVAL;
    }

    char* filename = argv[1];
    FILE* pds = fopen(filename, "rt");

    if (pds == NULL) {
        fprintf(stderr, "Error: file \"%s\" does not exist or can't be opened\n", filename);
        return ENOENT;
    }

    TokenStream* tokens = tokenize(pds);

    PDSLabel* label = parse_label(tokens);
    printf( "Found a version \"%s\" label with %d objects\n"
          , label->version
          , hashtable_count(label->objects)
          );

    if (hashtable_count(label->objects) > 0) {
        struct hashtable_itr* it8r = hashtable_iterator(label->objects);
        char* name;
        PDSObject* object;

        printf("Label objects: ");
        int obj_index = 0;
        do {
            name = hashtable_iterator_key(it8r);
            object = hashtable_iterator_value(it8r);

            printf("%i) \"%s\" ", obj_index, name);
            obj_index++;
        } while (hashtable_iterator_advance(it8r));

        printf("\n");
        free(it8r);
    }

    PDSObject* img;
    if (img = ht_search_for_obj(label->objects, "IMAGE")) {
        Value* lines_val;
        int lines, line_samples;

        if (lines_val = ht_search_for_value(img->attrs, "LINES")) {
            assert(lines_val->generic.type == CFG_NUMBER_INTEGRAL);
            lines = ((IntVal*) lines_val)->value;
        }

        Value* line_samples_val;
        if (line_samples_val = ht_search_for_value(img->attrs, "LINE_SAMPLES")) {
            assert(line_samples_val->generic.type == CFG_NUMBER_INTEGRAL);
            line_samples = ((IntVal*) line_samples_val)->value;
        }

        if (lines > 0 && line_samples > 0) {
            printf( "Found IMAGE object with %d lines and %d samples per line\n"
                  , lines
                  , line_samples
                  );
        }
    }

    fclose(pds);
}
