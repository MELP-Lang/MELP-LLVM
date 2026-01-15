/**
 * STO Runtime Implementation
 * Transparent Type Optimization runtime support
 */

#include "sto_runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

// ============================================================================
// Phase 3.1: Overflow Detection & Promotion
// ============================================================================

bool sto_would_overflow_add(int64_t a, int64_t b) {
    return (b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b);
}

bool sto_would_overflow_sub(int64_t a, int64_t b) {
    return (b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b);
}

bool sto_would_overflow_mul(int64_t a, int64_t b) {
    if (a == 0 || b == 0) return false;
    if (a > 0 && b > 0 && a > INT64_MAX / b) return true;
    if (a > 0 && b < 0 && b < INT64_MIN / a) return true;
    if (a < 0 && b > 0 && a < INT64_MIN / b) return true;
    if (a < 0 && b < 0 && a < INT64_MAX / b) return true;
    return false;
}

bool sto_safe_add_i64(int64_t a, int64_t b, int64_t* result) {
    if (sto_would_overflow_add(a, b)) {
        *result = 0;
        return true;  // Overflow occurred
    }
    *result = a + b;
    return false;  // No overflow
}

bool sto_safe_sub_i64(int64_t a, int64_t b, int64_t* result) {
    if (sto_would_overflow_sub(a, b)) {
        *result = 0;
        return true;
    }
    *result = a - b;
    return false;
}

bool sto_safe_mul_i64(int64_t a, int64_t b, int64_t* result) {
    if (sto_would_overflow_mul(a, b)) {
        *result = 0;
        return true;
    }
    *result = a * b;
    return false;
}

// ============================================================================
// Phase 3.2: BigDecimal Runtime Library
// ============================================================================

BigDecimal* bigdec_from_i64(int64_t value) {
    BigDecimal* bd = malloc(sizeof(BigDecimal));
    bd->digits = malloc(32);
    bd->negative = (value < 0);
    if (bd->negative) value = -value;
    snprintf(bd->digits, 32, "%lld", (long long)value);
    bd->length = strlen(bd->digits);
    bd->scale = 0;
    bd->refcount = 1;
    return bd;
}

BigDecimal* bigdec_from_double(double value) {
    BigDecimal* bd = malloc(sizeof(BigDecimal));
    bd->digits = malloc(64);
    bd->negative = (value < 0);
    if (bd->negative) value = -value;
    snprintf(bd->digits, 64, "%.15g", value);
    bd->length = strlen(bd->digits);
    bd->scale = 15;
    bd->refcount = 1;
    return bd;
}

BigDecimal* bigdec_from_string(const char* str) {
    BigDecimal* bd = malloc(sizeof(BigDecimal));
    size_t len = strlen(str);
    bd->digits = malloc(len + 1);
    strcpy(bd->digits, str);
    bd->negative = (str[0] == '-');
    bd->length = bd->negative ? len - 1 : len;
    bd->scale = 0;
    bd->refcount = 1;
    return bd;
}

BigDecimal* bigdec_add(BigDecimal* a, BigDecimal* b) {
    // Simplified implementation
    BigDecimal* result = malloc(sizeof(BigDecimal));
    size_t len = a->length + b->length + 10;
    result->digits = malloc(len);
    snprintf(result->digits, len, "(%s+%s)", a->digits, b->digits);
    result->length = strlen(result->digits);
    result->scale = (a->scale > b->scale) ? a->scale : b->scale;
    result->negative = false;
    result->refcount = 1;
    return result;
}

BigDecimal* bigdec_sub(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = malloc(sizeof(BigDecimal));
    size_t len = a->length + b->length + 10;
    result->digits = malloc(len);
    snprintf(result->digits, len, "(%s-%s)", a->digits, b->digits);
    result->length = strlen(result->digits);
    result->scale = (a->scale > b->scale) ? a->scale : b->scale;
    result->negative = false;
    result->refcount = 1;
    return result;
}

BigDecimal* bigdec_mul(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = malloc(sizeof(BigDecimal));
    size_t len = a->length + b->length + 10;
    result->digits = malloc(len);
    snprintf(result->digits, len, "(%s*%s)", a->digits, b->digits);
    result->length = strlen(result->digits);
    result->scale = a->scale + b->scale;
    result->negative = (a->negative != b->negative);
    result->refcount = 1;
    return result;
}

BigDecimal* bigdec_div(BigDecimal* a, BigDecimal* b) {
    BigDecimal* result = malloc(sizeof(BigDecimal));
    size_t len = a->length + b->length + 10;
    result->digits = malloc(len);
    snprintf(result->digits, len, "(%s/%s)", a->digits, b->digits);
    result->length = strlen(result->digits);
    result->scale = a->scale - b->scale + 15;
    result->negative = (a->negative != b->negative);
    result->refcount = 1;
    return result;
}

int bigdec_compare(BigDecimal* a, BigDecimal* b) {
    if (a->negative != b->negative) {
        return a->negative ? -1 : 1;
    }
    return strcmp(a->digits, b->digits);
}

char* bigdec_to_string(BigDecimal* bd) {
    size_t len = bd->length + (bd->negative ? 2 : 1);
    char* str = malloc(len);
    if (bd->negative) {
        snprintf(str, len, "-%s", bd->digits);
    } else {
        strcpy(str, bd->digits);
    }
    return str;
}

int64_t bigdec_to_i64(BigDecimal* bd) {
    int64_t value = atoll(bd->digits);
    return bd->negative ? -value : value;
}

double bigdec_to_double(BigDecimal* bd) {
    double value = atof(bd->digits);
    return bd->negative ? -value : value;
}

void bigdec_free(BigDecimal* bd) {
    if (bd) {
        free(bd->digits);
        free(bd);
    }
}

void bigdec_retain(BigDecimal* bd) {
    if (bd) bd->refcount++;
}

void bigdec_release(BigDecimal* bd) {
    if (bd && --bd->refcount == 0) {
        bigdec_free(bd);
    }
}

// ============================================================================
// Phase 3.3: SSO (Small String Optimization)
// ============================================================================

SSOString* sso_create(const char* str) {
    SSOString* sso = malloc(sizeof(SSOString));
    size_t len = strlen(str);
    
    if (len < SSO_MAX_SIZE) {
        // Use small string optimization
        sso->is_heap = false;
        strcpy(sso->storage.sso.data, str);
        sso->storage.sso.length = (uint8_t)len;
    } else {
        // Allocate on heap
        sso->is_heap = true;
        sso->storage.heap.ptr = malloc(len + 1);
        strcpy(sso->storage.heap.ptr, str);
        sso->storage.heap.length = len;
        sso->storage.heap.capacity = len + 1;
        sso->storage.heap.refcount = 1;
    }
    
    return sso;
}

const char* sso_data(SSOString* str) {
    return str->is_heap ? str->storage.heap.ptr : str->storage.sso.data;
}

size_t sso_length(SSOString* str) {
    return str->is_heap ? str->storage.heap.length : str->storage.sso.length;
}

SSOString* sso_concat(SSOString* a, SSOString* b) {
    const char* str_a = sso_data(a);
    const char* str_b = sso_data(b);
    size_t len_a = sso_length(a);
    size_t len_b = sso_length(b);
    size_t total_len = len_a + len_b;
    
    SSOString* result = malloc(sizeof(SSOString));
    
    if (total_len < SSO_MAX_SIZE) {
        // Use SSO
        result->is_heap = false;
        strcpy(result->storage.sso.data, str_a);
        strcat(result->storage.sso.data, str_b);
        result->storage.sso.length = (uint8_t)total_len;
    } else {
        // Use heap
        result->is_heap = true;
        result->storage.heap.ptr = malloc(total_len + 1);
        strcpy(result->storage.heap.ptr, str_a);
        strcat(result->storage.heap.ptr, str_b);
        result->storage.heap.length = total_len;
        result->storage.heap.capacity = total_len + 1;
        result->storage.heap.refcount = 1;
    }
    
    return result;
}

void sso_free(SSOString* str) {
    if (str) {
        if (str->is_heap) {
            free(str->storage.heap.ptr);
        }
        free(str);
    }
}

void sso_retain(SSOString* str) {
    if (str && str->is_heap) {
        str->storage.heap.refcount++;
    }
}

void sso_release(SSOString* str) {
    if (str && str->is_heap && --str->storage.heap.refcount == 0) {
        sso_free(str);
    }
}

// ============================================================================
// Phase 3.4: Memory Management
// ============================================================================

static STOMemStats mem_stats = {0};

void sto_runtime_init(void) {
    memset(&mem_stats, 0, sizeof(STOMemStats));
}

void sto_runtime_cleanup(void) {
    // Cleanup would happen here if needed
}

STOMemStats sto_get_mem_stats(void) {
    return mem_stats;
}

// ============================================================================
// Phase 3.5: Array/List/Tuple Runtime Support
// ============================================================================

/**
 * Allocate a homogeneous array with fixed size elements
 * 
 * @param count Number of elements
 * @param elem_size Size of each element in bytes
 * @return Pointer to STOArray structure
 * 
 * Note: Returns raw pointer for direct assembly access.
 * Assembly code can store elements directly at base_ptr + (index * elem_size)
 */
void* sto_array_alloc(size_t count, size_t elem_size) {
    if (count == 0 || elem_size == 0) {
        return NULL;
    }
    
    STOArray* array = malloc(sizeof(STOArray));
    if (!array) {
        fprintf(stderr, "ERROR: Failed to allocate array structure\n");
        exit(1);
    }
    
    // Allocate contiguous memory for elements
    array->elements = calloc(count, elem_size);
    if (!array->elements) {
        fprintf(stderr, "ERROR: Failed to allocate array elements\n");
        free(array);
        exit(1);
    }
    
    array->count = count;
    array->elem_size = elem_size;
    array->refcount = 1;
    
    mem_stats.total_allocations++;
    
    // Return pointer to elements for direct access from assembly
    return array->elements;
}

/**
 * Set an array element (for bounds-checked access)
 * 
 * @param array Pointer to STOArray
 * @param index Element index
 * @param value Pointer to value to copy
 */
void sto_array_set(STOArray* array, size_t index, void* value) {
    if (!array || !value) return;
    if (index >= array->count) {
        fprintf(stderr, "ERROR: Array index out of bounds: %zu >= %zu\n", 
                index, array->count);
        exit(1);
    }
    
    void* dest = (char*)array->elements + (index * array->elem_size);
    memcpy(dest, value, array->elem_size);
}

/**
 * Get an array element
 * 
 * @param array Pointer to STOArray
 * @param index Element index
 * @return Pointer to element
 */
void* sto_array_get(STOArray* array, size_t index) {
    if (!array) return NULL;
    if (index >= array->count) {
        fprintf(stderr, "ERROR: Array index out of bounds: %zu >= %zu\n", 
                index, array->count);
        exit(1);
    }
    
    return (char*)array->elements + (index * array->elem_size);
}

/**
 * Free an array
 */
void sto_array_free(STOArray* array) {
    if (array) {
        free(array->elements);
        free(array);
    }
}

/**
 * Allocate a heterogeneous list (dynamic, type-tracked)
 * 
 * @param capacity Initial capacity
 * @return Pointer to STOList
 */
STOList* sto_list_alloc(size_t capacity) {
    if (capacity == 0) capacity = 4;  // Default capacity
    
    STOList* list = malloc(sizeof(STOList));
    if (!list) {
        fprintf(stderr, "ERROR: Failed to allocate list structure\n");
        exit(1);
    }
    
    list->elements = calloc(capacity, sizeof(void*));
    list->types = calloc(capacity, sizeof(uint8_t));
    
    if (!list->elements || !list->types) {
        fprintf(stderr, "ERROR: Failed to allocate list storage\n");
        free(list->elements);
        free(list->types);
        free(list);
        exit(1);
    }
    
    list->count = 0;
    list->capacity = capacity;
    list->refcount = 1;
    
    mem_stats.total_allocations++;
    
    return list;
}

/**
 * Set a list element at a specific index
 * 
 * @param list Pointer to STOList
 * @param index Element index
 * @param value Pointer to value (copied to heap)
 * @param type VarType of the element
 */
void sto_list_set(STOList* list, size_t index, void* value, uint8_t type) {
    if (!list || !value) return;
    
    // Grow if needed
    if (index >= list->capacity) {
        size_t new_capacity = (index + 1) * 2;
        void** new_elements = realloc(list->elements, new_capacity * sizeof(void*));
        uint8_t* new_types = realloc(list->types, new_capacity * sizeof(uint8_t));
        
        if (!new_elements || !new_types) {
            fprintf(stderr, "ERROR: Failed to grow list\n");
            exit(1);
        }
        
        // Zero out new slots
        memset(new_elements + list->capacity, 0, 
               (new_capacity - list->capacity) * sizeof(void*));
        memset(new_types + list->capacity, 0, 
               (new_capacity - list->capacity) * sizeof(uint8_t));
        
        list->elements = new_elements;
        list->types = new_types;
        list->capacity = new_capacity;
    }
    
    // Copy value to heap (8 bytes for int64/double)
    int64_t* heap_value = malloc(8);
    memcpy(heap_value, value, 8);
    
    list->elements[index] = heap_value;
    list->types[index] = type;
    
    if (index >= list->count) {
        list->count = index + 1;
    }
}

/**
 * Get a list element
 */
void* sto_list_get(STOList* list, size_t index) {
    if (!list || index >= list->count) {
        fprintf(stderr, "ERROR: List index out of bounds: %zu >= %zu\n", 
                index, list->count);
        exit(1);
    }
    
    return list->elements[index];
}

/**
 * Append to list
 */
void sto_list_append(STOList* list, void* value, uint8_t type) {
    sto_list_set(list, list->count, value, type);
}

/**
 * Free a list
 */
void sto_list_free(STOList* list) {
    if (list) {
        // Free all element data
        for (size_t i = 0; i < list->count; i++) {
            free(list->elements[i]);
        }
        free(list->elements);
        free(list->types);
        free(list);
    }
}

/**
 * Allocate a tuple (immutable, heterogeneous)
 */
STOTuple* sto_tuple_alloc(size_t count) {
    if (count == 0) return NULL;
    
    STOTuple* tuple = malloc(sizeof(STOTuple));
    if (!tuple) {
        fprintf(stderr, "ERROR: Failed to allocate tuple\n");
        exit(1);
    }
    
    tuple->elements = calloc(count, sizeof(void*));
    tuple->types = calloc(count, sizeof(uint8_t));
    
    if (!tuple->elements || !tuple->types) {
        fprintf(stderr, "ERROR: Failed to allocate tuple storage\n");
        free(tuple->elements);
        free(tuple->types);
        free(tuple);
        exit(1);
    }
    
    tuple->count = count;
    tuple->refcount = 1;
    
    mem_stats.total_allocations++;
    
    return tuple;
}

/**
 * Set a tuple element (only during initialization)
 */
void sto_tuple_set(STOTuple* tuple, size_t index, void* value, uint8_t type) {
    if (!tuple || !value || index >= tuple->count) return;
    
    // Copy value to heap
    int64_t* heap_value = malloc(8);
    memcpy(heap_value, value, 8);
    
    tuple->elements[index] = heap_value;
    tuple->types[index] = type;
}

/**
 * Get a tuple element
 */
void* sto_tuple_get(STOTuple* tuple, size_t index) {
    if (!tuple || index >= tuple->count) {
        fprintf(stderr, "ERROR: Tuple index out of bounds: %zu >= %zu\n", 
                index, tuple->count);
        exit(1);
    }
    
    return tuple->elements[index];
}

/**
 * Free a tuple
 */
void sto_tuple_free(STOTuple* tuple) {
    if (tuple) {
        // Free all element data
        for (size_t i = 0; i < tuple->count; i++) {
            free(tuple->elements[i]);
        }
        free(tuple->elements);
        free(tuple->types);
        free(tuple);
    }
}
