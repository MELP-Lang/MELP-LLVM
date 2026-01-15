/**
 * MLP Standard Library - Simple Array Operations
 * 
 * Minimal array support for Stage 2 bridge
 * Wrapper around melp_list for fixed-size numeric arrays
 * 
 * Task 0.3 - Stage 2 Bridge
 * Date: 2 Ocak 2026
 */

#ifndef MLP_ARRAY_H
#define MLP_ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include "mlp_list.h"

/**
 * Create a fixed-size numeric array
 * 
 * @param size Number of elements to allocate
 * @return Pointer to array (actually a MelpList*), or NULL on failure
 * 
 * Usage in MELP:
 *   array tokens = create_array(100)
 *   tokens[0] = 5
 *   numeric val = tokens[0]
 */
MelpList* mlp_array_create(size_t size);

/**
 * Get element from array
 * 
 * @param arr Array (MelpList*)
 * @param index Zero-based index
 * @return Value at index, or 0 if out of bounds
 */
int64_t mlp_array_get(MelpList* arr, size_t index);

/**
 * Set element in array
 * 
 * @param arr Array (MelpList*)
 * @param index Zero-based index
 * @param value Value to set
 * @return 0 on success, -1 on failure
 */
int mlp_array_set(MelpList* arr, size_t index, int64_t value);

#endif // MLP_ARRAY_H
