// hashset.c
#include "hashset.h"
#include <stdlib.h>

#define LOAD_FACTOR_THRESHOLD 0.75

static int hash_function(int value, int capacity) {
    return abs(value) % capacity;
}

HashSet* hashset_create(int initial_capacity) {
    HashSet* set = malloc(sizeof(HashSet));
    set->capacity = initial_capacity;
    set->size = 0;
    set->buckets = calloc(initial_capacity, sizeof(HashNode*));
    return set;
}

void hashset_destroy(HashSet* set) {
    for (int i = 0; i < set->capacity; i++) {
        HashNode* current = set->buckets[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(set->buckets);
    free(set);
}

static void resize_if_needed(HashSet* set) {
    if ((float)set->size / set->capacity >= LOAD_FACTOR_THRESHOLD) {
        int new_capacity = set->capacity * 2;
        HashNode** new_buckets = calloc(new_capacity, sizeof(HashNode*));
        
        // Rehash all existing elements
        for (int i = 0; i < set->capacity; i++) {
            HashNode* current = set->buckets[i];
            while (current != NULL) {
                HashNode* next = current->next;
                int new_index = hash_function(current->value, new_capacity);
                current->next = new_buckets[new_index];
                new_buckets[new_index] = current;
                current = next;
            }
        }
        
        free(set->buckets);
        set->buckets = new_buckets;
        set->capacity = new_capacity;
    }
}

bool hashset_add(HashSet* set, int value) {
    resize_if_needed(set);
    int index = hash_function(value, set->capacity);
    
    // Check if value already exists
    HashNode* current = set->buckets[index];
    while (current != NULL) {
        if (current->value == value) {
            return false;  // Value already exists
        }
        current = current->next;
    }
    
    // Add new node
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->value = value;
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
    set->size++;
    return true;
}

bool hashset_contains(HashSet* set, int value) {
    int index = hash_function(value, set->capacity);
    HashNode* current = set->buckets[index];
    while (current != NULL) {
        if (current->value == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool hashset_remove(HashSet* set, int value) {
    int index = hash_function(value, set->capacity);
    HashNode* current = set->buckets[index];
    HashNode* prev = NULL;
    
    while (current != NULL) {
        if (current->value == value) {
            if (prev == NULL) {
                set->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            set->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}
