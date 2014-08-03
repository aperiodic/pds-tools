#include "org_aperiodic_PDSTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/cfg.h"
#include "../../src/parse.h"

#define OBJECT "Ljava/lang/Object;"

JNIEXPORT jobject JNICALL Java_org_aperiodic_PDSTools_parse( JNIEnv *env
                                                           , jobject obj
                                                           , jstring Filename
                                                           )
{
  const char* filename = (*env)->GetStringUTFChars(env, Filename, NULL);
  FILE* pds = fopen(filename, "rt");

  if (pds == NULL) {
      return NULL;
  }

  PDSLabel* label = parse_file(pds);
  fclose(pds);
  if (label == NULL) {
      return NULL;
  }

  jclass mapclass = (*env)->FindClass(env, "java/util/HashMap");
  if (mapclass == NULL) {
      return NULL;
  }

  jmethodID jinit = (*env)->GetMethodID(env, mapclass, "<init>", "(I)V");
  jobject jlabel = (*env)->NewObject(env, mapclass, jinit, 32);

  jmethodID put = (*env)->GetMethodID( env
                                     , mapclass
                                     , "put"
                                     , "(" OBJECT "" OBJECT ")" OBJECT
  );

  jstring jversion_key = (*env)->NewStringUTF(env, "PDS_VERSION_ID");
  jstring jversion = (*env)->NewStringUTF(env, label->version);
  (*env)->CallObjectMethod(env, jlabel, put, jversion_key, jversion);

  return jlabel;
}

void main() {};
