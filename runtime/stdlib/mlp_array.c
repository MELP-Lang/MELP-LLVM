/**
 * MLP Standard Library - Simple Array Operations
 * 
 * Minimal array support for Stage 2 bridge
 * Wrapper around melp_list for fixed-size numeric arrays
 * 
 * Task 0.3 - Stage 2 Bridge
 * Date: 2 Ocak 2026
 */

#include "mlp_array.h"
#include <stdlib.h>
#include <string.h>

/**
 * Create a fixed-size numeric array
 * Pre-allocates space for 'size' int64_t elements
 */
MelpList* mlp_array_create(size_t size) {
    // Create list with element size = int64_t
    MelpList* arr = melp_list_create(sizeof(int64_t));
    if (!arr) return NULL;
    
    // Pre-allocate space by appending zeros
    int64_t zero = 0;
    for (size_t i = 0; i < size; i++) {
        melp_list_append(arr, &zero);
    }
    
    return arr;
}

/**
 * Get element from array
 * Returns the int64_t value (not pointer)
 */
int64_t mlp_array_get(MelpList* arr, size_t index) {
    if (!arr) return 0;
    
    void* ptr = melp_list_get(arr, index);
    if (!ptr) return 0;  // Out of bounds
    
    // Dereference to get int64_t value
    return *(int64_t*)ptr;
}

/**
 * Set element in array
 */
int mlp_array_set(MelpList* arr, size_t index, int64_t value) {
    if (!arr) return -1;
    
    return melp_list_set(arr, index, &value);
}
