#ifndef RUNTIME_STO_H
#define RUNTIME_STO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sto_types.h"

// ============================================================================
// STO Runtime Support - Phase 3
// ============================================================================
// Runtime functions for STO optimization:
// - Overflow detection and promotion
// - BigDecimal operations
// - SSO string management
// - Memory management

// ============================================================================
// Phase 3.1: Overflow Detection & Promotion
// ============================================================================

// Check if INT64 addition will overflow
bool sto_runtime_add_will_overflow(int64_t a, int64_t b);

// Check if INT64 subtraction will overflow
bool sto_runtime_sub_will_overflow(int64_t a, int64_t b);

// Check if INT64 multiplication will overflow
bool sto_runtime_mul_will_overflow(int64_t a, int64_t b);

// Safe addition with overflow check (returns true if promoted to BigDecimal)
bool sto_runtime_safe_add(int64_t a, int64_t b, int64_t* result);

// Safe subtraction with overflow check
bool sto_runtime_safe_sub(int64_t a, int64_t b, int64_t* result);

// Safe multiplication with overflow check
bool sto_runtime_safe_mul(int64_t a, int64_t b, int64_t* result);

// ============================================================================
// Phase 3.5: Print Functions
// ============================================================================

// Print INT64 to stdout with newline
void sto_print_int64(int64_t value);

// Print double to stdout with newline
void sto_print_double(double value);

// ============================================================================
// Phase 3.2: BigDecimal Runtime
// ============================================================================

// BigDecimal structure - arbitrary precision decimal numbers
struct BigDecimal {
    char* digits;      // String representation of digits
    int length;        // Number of digits
    bool negative;     // Sign flag
    int refcount;      // Reference counting for memory management
};

typedef struct BigDecimal BigDecimal;

// Create BigDecimal from INT64
BigDecimal* sto_bigdec_from_int64(int64_t value);

// Create BigDecimal from string
BigDecimal* sto_bigdec_from_string(const char* str);

// BigDecimal operations (to be implemented)
BigDecimal* sto_bigdec_add(BigDecimal* a, BigDecimal* b);
BigDecimal* sto_bigdec_sub(BigDecimal* a, BigDecimal* b);
BigDecimal* sto_bigdec_mul(BigDecimal* a, BigDecimal* b);
BigDecimal* sto_bigdec_div(BigDecimal* a, BigDecimal* b);

// Convert BigDecimal to string
char* sto_bigdec_to_string(BigDecimal* bd);

// Compare two BigDecimals (-1: a<b, 0: a==b, 1: a>b)
int sto_bigdec_compare(BigDecimal* a, BigDecimal* b);

// Free BigDecimal
void sto_bigdec_free(BigDecimal* bd);

// ============================================================================
// Phase 3.3: SSO String (Placeholder)
// ============================================================================

// SSO String structure (23 bytes inline + 1 byte flag)
typedef struct {
    union {
        char inline_data[24];  // Small string storage (≤23 bytes + null)
        struct {
            char* heap_ptr;    // Heap pointer for large strings
            size_t length;     // String length
            size_t capacity;   // Allocated capacity
        } heap;
    } data;
    uint8_t flags;  // Bit 0: is_heap, Bit 1-7: inline_length
} SSOString;

// Create SSO string from C string
SSOString* sto_sso_create(const char* str);

// Get string data (works for both SSO and heap)
const char* sto_sso_data(SSOString* sso);

// Get string length
size_t sto_sso_length(SSOString* sso);

// String concatenation
SSOString* sto_sso_concat(SSOString* a, SSOString* b);

// String comparison
int sto_sso_compare(SSOString* a, SSOString* b);

// String equality
bool sto_sso_equals(SSOString* a, SSOString* b);

// Substring extraction
SSOString* sto_sso_substring(SSOString* str, size_t start, size_t length);

// String/Integer conversions
int64_t sto_sso_to_int64(SSOString* str);
SSOString* sto_sso_from_int64(int64_t value);

// String search
int sto_sso_find(SSOString* haystack, const char* needle);

// String copy
SSOString* sto_sso_copy(SSOString* str);

// Convert to C string (caller must free)
char* sto_sso_to_cstring(SSOString* str);

// String prefix/suffix checks
bool sto_sso_starts_with(SSOString* str, const char* prefix);
bool sto_sso_ends_with(SSOString* str, const char* suffix);

// Free SSO string
void sto_sso_free(SSOString* sso);

// ============================================================================
// Phase 3.4: Type Inference
// ============================================================================

// Infer numeric type from literal string (INT64 or BigDecimal)
// Returns: 0 for INT64, 1 for BigDecimal
int sto_infer_numeric_type(const char* literal);

// Infer string storage type (SSO or heap)
// Returns: 0 for SSO (≤23 bytes), 1 for heap (>23 bytes)
int sto_infer_string_type(const char* literal);

#endif
