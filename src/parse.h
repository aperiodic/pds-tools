#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#include "cfg.h"

PDSLabel* parse_tokens(TokenStream* stream);
PDSLabel* parse_file(FILE* pds_file);

#endif
