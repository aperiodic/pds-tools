#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"
#include "hashtable.h"
#include "hashtable_itr.h"
#include "parse.h"

#include "org_aperiodic_PDSTools.h"

#define OBJECT "Ljava/lang/Object;"
#define MAP_CAPACITY 32

DEFINE_HASHTABLE_SEARCH(ht_search_for_obj, char, PDSObject);
DEFINE_HASHTABLE_SEARCH(ht_search_for_value, char, Value);

void dump_pdsobj_to_jmap(PDSObject* obj, jobject jmap, JNIEnv *env, jmethodID put);
jobject pdsvalue_to_jobject(Value* val, JNIEnv* env);

//
// JNI Interface Implementation
//

JNIEXPORT jobject JNICALL Java_org_aperiodic_PDSTools_parse( JNIEnv *env
                                                           , jobject obj
                                                           , jstring Filename
                                                           )
{
  // Parse the File
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

  // Do the setup for being able to create & put into hashmaps.
  jclass mapclass = (*env)->FindClass(env, "java/util/HashMap");
  if (mapclass == NULL) {
      return NULL;
  }

  jmethodID map_init = (*env)->GetMethodID(env, mapclass, "<init>", "(I)V");
  jmethodID put = (*env)->GetMethodID( env
                                     , mapclass
                                     , "put"
                                     , "(" OBJECT "" OBJECT ")" OBJECT
  );

  jobject jlabel = (*env)->NewObject(env, mapclass, map_init, MAP_CAPACITY);
  jstring jversion_key = (*env)->NewStringUTF(env, "PDS_VERSION_ID");
  jstring jversion = (*env)->NewStringUTF(env, label->version);
  (*env)->CallObjectMethod(env, jlabel, put, jversion_key, jversion);

  jobject jmetadata = (*env)->NewObject(env, mapclass, map_init, MAP_CAPACITY);
  jstring jmetadata_key = (*env)->NewStringUTF(env, "METADATA");
  (*env)->CallObjectMethod(env, jlabel, put, jmetadata_key, jmetadata);

  jobject jpds_objs = (*env)->NewObject(env, mapclass, map_init, MAP_CAPACITY);
  jstring jobjs_key = (*env)->NewStringUTF(env, "OBJECTS");
  (*env)->CallObjectMethod(env, jlabel, put, jobjs_key, jpds_objs);

  // stick all the metadata into the label map
  int datum_count = hashtable_count(label->metadata);
  if (datum_count > 0) {
      struct hashtable_itr* it8r = hashtable_iterator(label->metadata);
      char* key;
      Value* value;
      do {
          key = hashtable_iterator_key(it8r);
          value = hashtable_iterator_value(it8r);

          jstring jkey = (*env)->NewStringUTF(env, key);
          jobject jvalue = pdsvalue_to_jobject(value, env);
          (*env)->CallObjectMethod(env, jmetadata, put, jkey, jvalue);
      } while (hashtable_iterator_advance(it8r));
      free(it8r);
  }

  // stick all the objects in the label map
  int object_count = hashtable_count(label->objects);
  if (object_count > 0) {
      jobject* obj_jmaps = malloc(sizeof(jobject) * object_count);
      for (int i = 0; i < object_count; i++) {
          obj_jmaps[i] = (*env)->NewObject(env, mapclass, map_init, MAP_CAPACITY);
      }

      struct hashtable_itr* it8r = hashtable_iterator(label->objects);
      char* name;
      PDSObject* pds_obj;
      int i = 0;
      do {
          name = hashtable_iterator_key(it8r);
          pds_obj = hashtable_iterator_value(it8r);

          jstring jobj_key = (*env)->NewStringUTF(env, name);
          dump_pdsobj_to_jmap(pds_obj, obj_jmaps[i], env, put);
          (*env)->CallObjectMethod(env, jpds_objs, put, jobj_key, obj_jmaps[i]);

          i++;
      } while (hashtable_iterator_advance(it8r));

      free(it8r);
  }

  return jlabel;
}

void dump_pdsobj_to_jmap(PDSObject* obj, jobject jmap, JNIEnv* env, jmethodID put) {
    jstring jname_key = (*env)->NewStringUTF(env, "ID");
    jstring jobj_name = (*env)->NewStringUTF(env, obj->name);
    (*env)->CallObjectMethod(env, jmap, put, jname_key, jobj_name);

    if (hashtable_count(obj->attrs) > 0) {
        struct hashtable_itr* it8r = hashtable_iterator(obj->attrs);
        char* attr;
        Value* value;

        do {
            attr = hashtable_iterator_key(it8r);
            jstring jattr = (*env)->NewStringUTF(env, attr);

            value = hashtable_iterator_value(it8r);
            jobject jvalue = pdsvalue_to_jobject(value, env);
            (*env)->CallObjectMethod(env, jmap, put, jattr, jvalue);

        } while (hashtable_iterator_advance(it8r));

        free(it8r);
    }
}

jobject pdsvalue_to_jobject(Value* val, JNIEnv* env) {
    if (val->generic.type == CFG_VALUE_TUPLE) {
        jclass jpdsval_class = (*env)->FindClass(env, "org/aperiodic/PDSValue");
        if (jpdsval_class == NULL) {
            return (jobject) (*env)->NewStringUTF(env, "PDSTools/noPDSValueClass");
        }

        TupleValue tval = val->tuple;
        jobjectArray jtuple = (*env)->NewObjectArray(env, (jsize) tval.count, jpdsval_class, NULL);

        for (int i = 0; i < tval.count; i++) {
            jobject jval = pdsvalue_to_jobject((Value*) &(tval.values[i]), env);
            jmethodID jpdsval_init = (*env)->GetMethodID( env
                                                        , jpdsval_class
                                                        , "<init>"
                                                        , "(Ljava/lang/Object;)V"
                                                        );
            jobject jpdsval = (*env)->NewObject(env, jpdsval_class, jpdsval_init, jval);
            (*env)->SetObjectArrayElement(env, jtuple, i, jpdsval);
        }

        return (jobject) jtuple;
    } else {
        PrimitiveValue pval = val->primitive;
        jclass formatclass, intclass, floatclass;

        // get a value for jval
        switch (pval.generic.type) {
            case CFG_STRING:
                return (jobject) (*env)->NewStringUTF(env, pval.string.value);
                break;

            case CFG_DATE:
                formatclass = (*env)->FindClass(env, "java/text/SimpleDateFormat");
                jstring ISOFORMAT = (*env)->NewStringUTF(env, "yyyy-MM-dd'T'HH:mm:ss");
                if (formatclass == NULL) {
                    return (jobject) (*env)->NewStringUTF(env, "PDSTools/noSimpleDateFormatClass");
                }
                jmethodID isof_init = (*env)->GetMethodID( env
                                                         , formatclass
                                                         , "<init>"
                                                         , "(Ljava/lang/String;)V"
                                                         );

                // could very well segfault here
                jobject isof = (*env)->NewObject(env, formatclass, isof_init, ISOFORMAT);
                jmethodID isof_parse = (*env)->GetMethodID( env
                                                          , formatclass
                                                          , "parse"
                                                          , "(Ljava/lang/String;)Ljava/util/Date;"
                                                          );
                jstring date_jstring = (*env)->NewStringUTF(env, pval.date.value);
                return (*env)->CallObjectMethod(env, isof, isof_parse, date_jstring);
                break;

            case CFG_NUMBER_INTEGRAL:
                intclass = (*env)->FindClass(env, "java/lang/Integer");
                if (intclass == NULL) {
                    return (jobject) (*env)->NewStringUTF(env, "PDSTools/noIntegerClass");
                }
                jmethodID jint_init = (*env)->GetMethodID(env, intclass, "<init>", "(I)V");
                return (*env)->NewObject(env, intclass, jint_init, pval.integer.type);
                break;

            case CFG_NUMBER_RATIONAL:
                floatclass = (*env)->FindClass(env, "java/lang/Float");
                if (floatclass == NULL) {
                    return (jobject) (*env)->NewStringUTF(env, "PDSTools/noIntegerClass");
                }
                jmethodID jfloat_init = (*env)->GetMethodID(env, floatclass, "<init>", "(F)V");
                return (*env)->NewObject(env, floatclass, jfloat_init, pval.rational.value);
                break;

            default:
                return (jobject) (*env)->NewStringUTF(env, "PDSTools/unsupported");
        }
    }
}

void main() {};
