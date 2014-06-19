#ifndef POOL_H
#define POOL_H

#include "cfg.h"

//
// Typedefs
//

typedef struct PDSObjectPool {
  PDSObject* objects;
  int capacity;
  int pos;
} PDSObjectPool;

typedef struct AssociationPool {
  Association* assocs;
  int capacity;
  int pos;
} AssociationPool;


//
// API
//

PDSObjectPool* new_pds_object_pool(int capacity);
PDSObject* allocate_pds_object(PDSObjectPool* pool);

AssociationPool* new_assoc_pool(int capacity);
Association* allocate_assoc(AssociationPool* pool);

#endif
