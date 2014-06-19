#include <stdlib.h>

#include "pool.h"


PDSObjectPool* new_pds_object_pool(int capacity) {
   PDSObjectPool* pool = malloc(sizeof(PDSObjectPool));
   pool->capacity = capacity;
   pool->pos = 0;
   pool->objects = malloc(sizeof(PDSObject) * pool->capacity);
   return pool;
}
PDSObject* allocate_pds_object(PDSObjectPool* pool) {
    if (pool->pos >= pool->capacity) {
        return NULL;
    }
    return &pool->objects[pool->pos++];
}

AssociationPool* new_assoc_pool(int capacity) {
   AssociationPool* pool = malloc(sizeof(AssociationPool));
   pool->capacity = capacity;
   pool->pos = 0;
   pool->assocs = malloc(sizeof(Association) * pool->capacity);
   return pool;
}

Association* allocate_assoc(AssociationPool* pool) {
    if (pool->pos >= pool->capacity) {
        return NULL;
    }
    return &pool->assocs[pool->pos++];
}
