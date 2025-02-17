// hashset.h
#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stdint.h>

// Use open addressing with linear probing instead of chaining
typedef struct {
    int* values;
    uint8_t* states;  // 0 = empty, 1 = deleted, 2 = occupied
    int capacity;
    int size;
    int deleted_count;
} HashSet;

HashSet* hashset_create(int initial_capacity);
void hashset_destroy(HashSet* set);
bool hashset_add(HashSet* set, int value);
bool hashset_contains(HashSet* set, int value);
bool hashset_remove(HashSet* set, int value);

#endif

// hashset.c
#include "hashset.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.7
#define DELETED_LOAD_THRESHOLD 0.2

// State values
#define EMPTY 0
#define DELETED 1
#define OCCUPIED 2

// Optimized hash function
static inline uint32_t hash_function(uint32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6b;
    x ^= x >> 13;
    x *= 0xc2b2ae35;
    x ^= x >> 16;
    return x;
}

// Round up to next power of 2
static inline int next_power_of_2(int n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

HashSet* hashset_create(int initial_capacity) {
    if (initial_capacity < INITIAL_CAPACITY) {
        initial_capacity = INITIAL_CAPACITY;
    }
    initial_capacity = next_power_of_2(initial_capacity);
    
    HashSet* set = malloc(sizeof(HashSet));
    if (!set) return NULL;
    
    set->values = malloc(initial_capacity * sizeof(int));
    set->states = malloc(initial_capacity * sizeof(uint8_t));
    
    if (!set->values || !set->states) {
        free(set->values);
        free(set->states);
        free(set);
        return NULL;
    }
    
    memset(set->states, EMPTY, initial_capacity);
    set->capacity = initial_capacity;
    set->size = 0;
    set->deleted_count = 0;
    
    return set;
}

void hashset_destroy(HashSet* set) {
    if (!set) return;
    free(set->values);
    free(set->states);
    free(set);
}

static bool hashset_resize(HashSet* set, int new_capacity) {
    int* new_values = malloc(new_capacity * sizeof(int));
    uint8_t* new_states = malloc(new_capacity * sizeof(uint8_t));
    
    if (!new_values || !new_states) {
        free(new_values);
        free(new_states);
        return false;
    }
    
    memset(new_states, EMPTY, new_capacity);
    
    // Rehash all existing values
    for (int i = 0; i < set->capacity; i++) {
        if (set->states[i] == OCCUPIED) {
            int value = set->values[i];
            uint32_t hash = hash_function(value);
            int index = hash & (new_capacity - 1);
            
            // Find next empty slot
            while (new_states[index] == OCCUPIED) {
                index = (index + 1) & (new_capacity - 1);
            }
            
            new_values[index] = value;
            new_states[index] = OCCUPIED;
        }
    }
    
    free(set->values);
    free(set->states);
    set->values = new_values;
    set->states = new_states;
    set->capacity = new_capacity;
    set->deleted_count = 0;
    
    return true;
}

bool hashset_add(HashSet* set, int value) {
    if (!set) return false;
    
    // Check if resize is needed
    if ((float)(set->size + set->deleted_count) / set->capacity >= LOAD_FACTOR_THRESHOLD) {
        if (!hashset_resize(set, set->capacity * 2)) {
            return false;
        }
    }
    // Check if cleanup of deleted entries is needed
    else if ((float)set->deleted_count / set->capacity >= DELETED_LOAD_THRESHOLD) {
        if (!hashset_resize(set, set->capacity)) {
            return false;
        }
    }
    
    uint32_t hash = hash_function(value);
    int index = hash & (set->capacity - 1);
    int first_deleted = -1;
    
    // Find insertion point
    while (true) {
        uint8_t state = set->states[index];
        
        if (state == EMPTY) {
            // Insert at first deleted slot if found, otherwise at empty slot
            if (first_deleted != -1) {
                index = first_deleted;
            }
            set->values[index] = value;
            set->states[index] = OCCUPIED;
            set->size++;
            return true;
        }
        
        if (state == DELETED) {
            if (first_deleted == -1) {
                first_deleted = index;
            }
        } else if (set->values[index] == value) {
            return false;  // Value already exists
        }
        
        index = (index + 1) & (set->capacity - 1);
    }
}

bool hashset_contains(HashSet* set, int value) {
    if (!set) return false;
    
    uint32_t hash = hash_function(value);
    int index = hash & (set->capacity - 1);
    
    while (true) {
        uint8_t state = set->states[index];
        
        if (state == EMPTY) {
            return false;
        }
        
        if (state == OCCUPIED && set->values[index] == value) {
            return true;
        }
        
        index = (index + 1) & (set->capacity - 1);
    }
}

bool hashset_remove(HashSet* set, int value) {
    if (!set) return false;
    
    uint32_t hash = hash_function(value);
    int index = hash & (set->capacity - 1);
    
    while (true) {
        uint8_t state = set->states[index];
        
        if (state == EMPTY) {
            return false;
        }
        
        if (state == OCCUPIED && set->values[index] == value) {
            set->states[index] = DELETED;
            set->size--;
            set->deleted_count++;
            return true;
        }
        
        index = (index + 1) & (set->capacity - 1);
    }
}