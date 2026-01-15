/**
 * MLP Standard Library - Map (Hash Table) Operations Implementation
 * 
 * Hash table with chaining for collision resolution
 * YZ_201: Map/Dictionary Type Implementation
 * 
 * Created: 21 Aralık 2025 (YZ_201)
 */

#include "mlp_map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE  // For strdup on some systems
#endif

// Constants
#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75
#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL

// -----------------------------------------------------------------------------
// Hash Function: FNV-1a (Fowler-Noll-Vo)
// -----------------------------------------------------------------------------

uint64_t melp_map_hash(const char* key) {
    if (!key) return 0;
    
    uint64_t hash = FNV_OFFSET_BASIS;
    
    while (*key) {
        hash ^= (uint64_t)(unsigned char)(*key);
        hash *= FNV_PRIME;
        key++;
    }
    
    return hash;
}

// -----------------------------------------------------------------------------
// Map Creation and Destruction
// -----------------------------------------------------------------------------

MelpMap* melp_map_create(size_t value_size) {
    MelpMap* map = (MelpMap*)malloc(sizeof(MelpMap));
    if (!map) {
        fprintf(stderr, "MELP Runtime Error: Failed to allocate map\n");
        return NULL;
    }
    
    map->capacity = INITIAL_CAPACITY;
    map->length = 0;
    map->value_size = value_size;
    
    // Allocate bucket array (calloc for NULL initialization)
    map->buckets = (MelpMapNode**)calloc(map->capacity, sizeof(MelpMapNode*));
    if (!map->buckets) {
        fprintf(stderr, "MELP Runtime Error: Failed to allocate map buckets\n");
        free(map);
        return NULL;
    }
    
    return map;
}

void melp_map_free(MelpMap* map) {
    if (!map) return;
    
    // Free all nodes in all buckets
    for (size_t i = 0; i < map->capacity; i++) {
        MelpMapNode* node = map->buckets[i];
        while (node) {
            MelpMapNode* next = node->next;
            free(node->key);      // Free copied key
            free(node->value);    // Free copied value
            free(node);           // Free node itself
            node = next;
        }
    }
    
    free(map->buckets);
    free(map);
}

// -----------------------------------------------------------------------------
// Map Resize (Internal)
// -----------------------------------------------------------------------------

int melp_map_resize(MelpMap* map, size_t new_capacity) {
    if (!map || new_capacity == 0) return 0;
    
    // Allocate new bucket array
    MelpMapNode** new_buckets = (MelpMapNode**)calloc(new_capacity, sizeof(MelpMapNode*));
    if (!new_buckets) {
        fprintf(stderr, "MELP Runtime Error: Failed to resize map\n");
        return 0;
    }
    
    // Rehash all existing nodes
    for (size_t i = 0; i < map->capacity; i++) {
        MelpMapNode* node = map->buckets[i];
        while (node) {
            MelpMapNode* next = node->next;
            
            // Compute new bucket index
            uint64_t hash = melp_map_hash(node->key);
            size_t new_index = hash % new_capacity;
            
            // Insert at head of new bucket (preserves O(1) insertion)
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;
            
            node = next;
        }
    }
    
    // Replace old buckets with new ones
    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
    
    return 1;
}

// -----------------------------------------------------------------------------
// Map Insert
// -----------------------------------------------------------------------------

int melp_map_insert(MelpMap* map, const char* key, const void* value) {
    if (!map || !key || !value) return 0;
    
    // Check if resize needed (load factor > 0.75)
    if ((double)map->length / (double)map->capacity > LOAD_FACTOR) {
        if (!melp_map_resize(map, map->capacity * 2)) {
            return 0;  // Resize failed
        }
    }
    
    // Compute bucket index
    uint64_t hash = melp_map_hash(key);
    size_t index = hash % map->capacity;
    
    // Check if key already exists (update if found)
    MelpMapNode* node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Key exists, update value
            memcpy(node->value, value, map->value_size);
            return 1;
        }
        node = node->next;
    }
    
    // Key doesn't exist, create new node
    MelpMapNode* new_node = (MelpMapNode*)malloc(sizeof(MelpMapNode));
    if (!new_node) {
        fprintf(stderr, "MELP Runtime Error: Failed to allocate map node\n");
        return 0;
    }
    
    // Copy key (take ownership)
    new_node->key = strdup(key);
    if (!new_node->key) {
        fprintf(stderr, "MELP Runtime Error: Failed to copy map key\n");
        free(new_node);
        return 0;
    }
    
    // Copy value (take ownership)
    new_node->value = malloc(map->value_size);
    if (!new_node->value) {
        fprintf(stderr, "MELP Runtime Error: Failed to allocate map value\n");
        free(new_node->key);
        free(new_node);
        return 0;
    }
    memcpy(new_node->value, value, map->value_size);
    
    // Insert at head of bucket (O(1) insertion)
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
    
    map->length++;
    return 1;
}

// -----------------------------------------------------------------------------
// Map Get
// -----------------------------------------------------------------------------

void* melp_map_get(MelpMap* map, const char* key) {
    if (!map || !key) return NULL;
    
    // Compute bucket index
    uint64_t hash = melp_map_hash(key);
    size_t index = hash % map->capacity;
    
    // Search in bucket chain
    MelpMapNode* node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;  // Found!
        }
        node = node->next;
    }
    
    return NULL;  // Not found
}

// -----------------------------------------------------------------------------
// Map Remove
// -----------------------------------------------------------------------------

int melp_map_remove(MelpMap* map, const char* key) {
    if (!map || !key) return 0;
    
    // Compute bucket index
    uint64_t hash = melp_map_hash(key);
    size_t index = hash % map->capacity;
    
    // Search in bucket chain (track previous node for deletion)
    MelpMapNode* node = map->buckets[index];
    MelpMapNode* prev = NULL;
    
    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Found! Remove node from chain
            if (prev) {
                prev->next = node->next;  // Remove from middle/end
            } else {
                map->buckets[index] = node->next;  // Remove from head
            }
            
            // Free node memory
            free(node->key);
            free(node->value);
            free(node);
            
            map->length--;
            return 1;  // Success
        }
        
        prev = node;
        node = node->next;
    }
    
    return 0;  // Key not found
}

// -----------------------------------------------------------------------------
// Map Has Key
// -----------------------------------------------------------------------------

int melp_map_has_key(MelpMap* map, const char* key) {
    return melp_map_get(map, key) != NULL;
}

// -----------------------------------------------------------------------------
// Map Length
// -----------------------------------------------------------------------------

size_t melp_map_length(MelpMap* map) {
    if (!map) return 0;
    return map->length;
}
