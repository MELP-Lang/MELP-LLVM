/**
 * MLP Standard Library - List (Dynamic Array) Operations Header
 * 
 * STO-compliant list operations for MELP compiler
 * YZ_200: Full list implementation with runtime allocation
 * 
 * Created: 21 Aralık 2025 (YZ_200)
 */

#ifndef MLP_LIST_H
#define MLP_LIST_H

#include <stddef.h>  // size_t
#include <stdint.h>  // int64_t

/**
 * MelpList - Dynamic array structure
 * 
 * Design Philosophy:
 * - Generic storage using void* array
 * - Capacity doubling strategy (Python/Rust Vec style)
 * - STO-compatible (heap allocation tracked)
 * - Initial capacity: 4 elements
 */
typedef struct {
    void** elements;      // Array of generic pointers
    size_t length;        // Current number of elements
    size_t capacity;      // Allocated capacity
    size_t element_size;  // Size of each element in bytes (for homogeneous lists)
} MelpList;

// -----------------------------------------------------------------------------
// Core List Operations
// -----------------------------------------------------------------------------

/**
 * Create a new list with specified element size
 * @param element_size Size of each element in bytes (e.g., sizeof(int64_t))
 * @return Pointer to newly allocated list, or NULL on failure
 */
MelpList* melp_list_create(size_t element_size);

/**
 * Free a list and all its allocated memory
 * @param list List to free
 */
void melp_list_free(MelpList* list);

/**
 * Get the current length of the list
 * @param list List to query
 * @return Number of elements in the list
 */
size_t melp_list_length(MelpList* list);

/**
 * Check if list is empty
 * @param list List to check
 * @return 1 if empty, 0 otherwise
 */
int melp_list_is_empty(MelpList* list);

// -----------------------------------------------------------------------------
// Element Access
// -----------------------------------------------------------------------------

/**
 * Get element at specified index
 * @param list List to access
 * @param index Zero-based index
 * @return Pointer to element, or NULL if index out of bounds
 */
void* melp_list_get(MelpList* list, size_t index);

/**
 * Set element at specified index
 * @param list List to modify
 * @param index Zero-based index
 * @param element Pointer to element data to copy
 * @return 0 on success, -1 on failure (index out of bounds)
 */
int melp_list_set(MelpList* list, size_t index, void* element);

// -----------------------------------------------------------------------------
// Modification Operations
// -----------------------------------------------------------------------------

/**
 * Append element to end of list
 * @param list List to modify
 * @param element Pointer to element data to copy
 * @return 0 on success, -1 on failure (memory allocation)
 */
int melp_list_append(MelpList* list, void* element);

/**
 * Prepend element to beginning of list
 * @param list List to modify
 * @param element Pointer to element data to copy
 * @return 0 on success, -1 on failure (memory allocation)
 */
int melp_list_prepend(MelpList* list, void* element);

/**
 * Remove element at specified index
 * @param list List to modify
 * @param index Zero-based index
 * @return 0 on success, -1 on failure (index out of bounds)
 */
int melp_list_remove(MelpList* list, size_t index);

/**
 * Clear all elements from list (but keep capacity)
 * @param list List to clear
 */
void melp_list_clear(MelpList* list);

// -----------------------------------------------------------------------------
// Utility Operations
// -----------------------------------------------------------------------------

/**
 * Create a copy of the list
 * @param list List to clone
 * @return New list with copied elements, or NULL on failure
 */
MelpList* melp_list_clone(MelpList* list);

/**
 * Reverse the list in-place
 * @param list List to reverse
 */
void melp_list_reverse(MelpList* list);

/**
 * Get current capacity of the list
 * @param list List to query
 * @return Allocated capacity
 */
size_t melp_list_capacity(MelpList* list);

/**
 * Reserve capacity for list (optimization to avoid multiple reallocations)
 * @param list List to modify
 * @param new_capacity Minimum capacity to reserve
 * @return 0 on success, -1 on failure
 */
int melp_list_reserve(MelpList* list, size_t new_capacity);

// -----------------------------------------------------------------------------
// Debug & Introspection
// -----------------------------------------------------------------------------

/**
 * Print list contents for debugging (assumes int64_t elements)
 * @param list List to print
 */
void melp_list_print_debug(MelpList* list);

#endif // MLP_LIST_H
