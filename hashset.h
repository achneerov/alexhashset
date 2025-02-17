// hashset.h
#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>

typedef struct HashNode {
    int value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    int size;
    int capacity;
} HashSet;

HashSet* hashset_create(int initial_capacity);
void hashset_destroy(HashSet* set);
bool hashset_add(HashSet* set, int value);
bool hashset_contains(HashSet* set, int value);
bool hashset_remove(HashSet* set, int value);

#endif
