/**
 * MLP Standard Library - Map (Hash Table) Operations Header
 * 
 * STO-compliant map operations for MELP compiler
 * YZ_201: Hash table with chaining implementation
 * 
 * Created: 21 Aralık 2025 (YZ_201)
 */

#ifndef MLP_MAP_H
#define MLP_MAP_H

#include <stddef.h>  // size_t
#include <stdint.h>  // int64_t, uint64_t

/**
 * Hash Table Node - Singly linked list for collision handling
 */
typedef struct MelpMapNode {
    char* key;                  // String key (owned by node)
    void* value;                // Generic value pointer
    struct MelpMapNode* next;   // Next node in chain (for collisions)
} MelpMapNode;

/**
 * MelpMap - Hash table with chaining
 * 
 * Design Philosophy:
 * - Open hashing (chaining) for collision resolution
 * - FNV-1a hash function for string keys
 * - Load factor: 0.75 (resize when length > capacity * 0.75)
 * - Initial capacity: 16 buckets
 * - String keys only (for stage0 simplicity)
 * - Generic values (void* pointers)
 */
typedef struct {
    MelpMapNode** buckets;    // Array of bucket heads
    size_t length;            // Current number of key-value pairs
    size_t capacity;          // Number of buckets
    size_t value_size;        // Size of each value in bytes (for type safety)
} MelpMap;

// -----------------------------------------------------------------------------
// Core Map Operations
// -----------------------------------------------------------------------------

/**
 * Create a new map with specified value size
 * @param value_size Size of each value in bytes (e.g., sizeof(int64_t))
 * @return Pointer to newly allocated map, or NULL on failure
 */
MelpMap* melp_map_create(size_t value_size);

/**
 * Free a map and all its allocated memory (keys, values, nodes)
 * @param map Map to free
 */
void melp_map_free(MelpMap* map);

/**
 * Insert or update a key-value pair in the map
 * @param map Target map
 * @param key String key (will be copied internally)
 * @param value Pointer to value data (will be copied)
 * @return 1 on success, 0 on failure
 */
int melp_map_insert(MelpMap* map, const char* key, const void* value);

/**
 * Get a value by key
 * @param map Target map
 * @param key String key to lookup
 * @return Pointer to value data, or NULL if key not found
 * 
 * NOTE: Returned pointer is owned by the map, do not free!
 */
void* melp_map_get(MelpMap* map, const char* key);

/**
 * Remove a key-value pair from the map
 * @param map Target map
 * @param key String key to remove
 * @return 1 if key was found and removed, 0 if key not found
 */
int melp_map_remove(MelpMap* map, const char* key);

/**
 * Check if a key exists in the map
 * @param map Target map
 * @param key String key to check
 * @return 1 if key exists, 0 otherwise
 */
int melp_map_has_key(MelpMap* map, const char* key);

/**
 * Get the number of key-value pairs in the map
 * @param map Target map
 * @return Current length of the map
 */
size_t melp_map_length(MelpMap* map);

// -----------------------------------------------------------------------------
// Internal Helper Functions
// -----------------------------------------------------------------------------

/**
 * FNV-1a hash function for string keys
 * @param key String to hash
 * @return 64-bit hash value
 */
uint64_t melp_map_hash(const char* key);

/**
 * Resize map to new capacity (internal use)
 * @param map Target map
 * @param new_capacity New number of buckets
 * @return 1 on success, 0 on failure
 */
int melp_map_resize(MelpMap* map, size_t new_capacity);

#endif // MLP_MAP_H
