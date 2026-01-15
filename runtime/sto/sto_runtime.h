#ifndef STO_RUNTIME_H
#define STO_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sto_types.h"

// ============================================================================
// STO Runtime Support - Phase 3
// ============================================================================
// Provides runtime support for STO optimizations:
// - Overflow detection and promotion (INT64 → BIGDECIMAL)
// - BigDecimal operations
// - SSO string management
// - Memory management hooks

// ============================================================================
// Phase 3.1: Overflow Detection & Promotion
// ============================================================================

// Check if addition would overflow
bool sto_would_overflow_add(int64_t a, int64_t b);

// Check if subtraction would overflow
bool sto_would_overflow_sub(int64_t a, int64_t b);

// Check if multiplication would overflow
bool sto_would_overflow_mul(int64_t a, int64_t b);

// Safe addition with overflow detection
// Returns true if overflow occurred, result promoted to BigDecimal
bool sto_safe_add_i64(int64_t a, int64_t b, int64_t* result);

// Safe subtraction with overflow detection
bool sto_safe_sub_i64(int64_t a, int64_t b, int64_t* result);

// Safe multiplication with overflow detection
bool sto_safe_mul_i64(int64_t a, int64_t b, int64_t* result);

// ============================================================================
// Phase 3.2: BigDecimal Runtime Library
// ============================================================================

// BigDecimal structure (arbitrary precision)
typedef struct {
    char* digits;      // String of digits
    int length;        // Number of digits
    int scale;         // Decimal point position (from right)
    bool negative;     // Sign
    int refcount;      // Reference count for GC
} BigDecimal;

// Create BigDecimal from int64
BigDecimal* bigdec_from_i64(int64_t value);

// Create BigDecimal from double
BigDecimal* bigdec_from_double(double value);

// Create BigDecimal from string
BigDecimal* bigdec_from_string(const char* str);

// BigDecimal operations
BigDecimal* bigdec_add(BigDecimal* a, BigDecimal* b);
BigDecimal* bigdec_sub(BigDecimal* a, BigDecimal* b);
BigDecimal* bigdec_mul(BigDecimal* a, BigDecimal* b);
BigDecimal* bigdec_div(BigDecimal* a, BigDecimal* b);

// BigDecimal comparison
int bigdec_compare(BigDecimal* a, BigDecimal* b);  // Returns: -1, 0, 1

// Convert BigDecimal to string
char* bigdec_to_string(BigDecimal* bd);

// Convert BigDecimal to int64 (may lose precision)
int64_t bigdec_to_i64(BigDecimal* bd);

// Convert BigDecimal to double (may lose precision)
double bigdec_to_double(BigDecimal* bd);

// Free BigDecimal
void bigdec_free(BigDecimal* bd);

// Reference counting
void bigdec_retain(BigDecimal* bd);
void bigdec_release(BigDecimal* bd);

// ============================================================================
// Phase 3.3: SSO (Small String Optimization)
// ============================================================================

#define SSO_MAX_SIZE 23

// SSO String structure (fits in 24 bytes)
typedef struct {
    union {
        struct {
            char data[SSO_MAX_SIZE];
            uint8_t length;  // Length stored in last byte
        } sso;
        struct {
            char* ptr;
            size_t length;
            size_t capacity;
            int refcount;
        } heap;
    } storage;
    bool is_heap;  // Flag to distinguish SSO vs heap
} SSOString;

// Create SSO string from C string
SSOString* sso_create(const char* str);

// Get string data (works for both SSO and heap)
const char* sso_data(SSOString* str);

// Get string length
size_t sso_length(SSOString* str);

// Concatenate strings (may promote to heap)
SSOString* sso_concat(SSOString* a, SSOString* b);

// Free SSO string
void sso_free(SSOString* str);

// Reference counting for heap strings
void sso_retain(SSOString* str);
void sso_release(SSOString* str);

// ============================================================================
// Phase 3.4: Memory Management
// ============================================================================

// Initialize STO runtime (call once at program start)
void sto_runtime_init(void);

// Cleanup STO runtime (call once at program end)
void sto_runtime_cleanup(void);

// Get memory statistics
typedef struct {
    size_t bigdecimal_count;
    size_t bigdecimal_bytes;
    size_t heap_string_count;
    size_t heap_string_bytes;
    size_t total_allocations;
} STOMemStats;

STOMemStats sto_get_mem_stats(void);

// ============================================================================
// Phase 3.5: Array/List/Tuple Runtime Support
// ============================================================================

// Array structure (homogeneous, fixed size)
typedef struct {
    void* elements;       // Element data
    size_t count;         // Number of elements
    size_t elem_size;     // Size of each element (in bytes)
    int refcount;         // Reference count for GC
} STOArray;

// List structure (heterogeneous, dynamic)
typedef struct {
    void** elements;      // Array of pointers to elements
    uint8_t* types;       // Array of element types (VarType)
    size_t count;         // Current number of elements
    size_t capacity;      // Allocated capacity
    int refcount;         // Reference count for GC
} STOList;

// Tuple structure (heterogeneous, immutable)
typedef struct {
    void** elements;      // Array of pointers to elements
    uint8_t* types;       // Array of element types (VarType)
    size_t count;         // Number of elements (fixed)
    int refcount;         // Reference count for GC
} STOTuple;

// Array operations
void* sto_array_alloc(size_t count, size_t elem_size);
void sto_array_set(STOArray* array, size_t index, void* value);
void* sto_array_get(STOArray* array, size_t index);
void sto_array_free(STOArray* array);

// List operations
STOList* sto_list_alloc(size_t capacity);
void sto_list_set(STOList* list, size_t index, void* value, uint8_t type);
void* sto_list_get(STOList* list, size_t index);
void sto_list_append(STOList* list, void* value, uint8_t type);
void sto_list_free(STOList* list);

// Tuple operations
STOTuple* sto_tuple_alloc(size_t count);
void sto_tuple_set(STOTuple* tuple, size_t index, void* value, uint8_t type);
void* sto_tuple_get(STOTuple* tuple, size_t index);
void sto_tuple_free(STOTuple* tuple);

#endif
