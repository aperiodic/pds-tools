#include "PDSTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/cfg.h"
#include "../../src/parse.h"

JNIEXPORT jobject JNICALL Java_PDSTools_parse( JNIEnv *env
                                              , jobject obj
                                              , jstring Filename
                                              , jobject blank_map
                                              )
{
  const char* filename = (*env)->GetStringUTFChars(env, Filename, NULL);
  //FILE* pds = fopen(filename, "rt");

  //if (pds == NULL) {
  //    return NULL;
  //}

  //PDSLabel* label = parse_file(pds);

  //fclose(pds);
  return blank_map;
}

void main() {};
