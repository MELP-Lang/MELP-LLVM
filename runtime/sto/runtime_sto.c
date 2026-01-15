#include "runtime_sto.h"
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// ============================================================================
// Phase 3.1: Overflow Detection & Promotion
// ============================================================================

// Check if INT64 addition will overflow
bool sto_runtime_add_will_overflow(int64_t a, int64_t b) {
    // Overflow occurs when:
    // - Both positive and result wraps to negative
    // - Both negative and result wraps to positive
    
    if (b > 0 && a > INT64_MAX - b) {
        return true;  // Positive overflow
    }
    if (b < 0 && a < INT64_MIN - b) {
        return true;  // Negative overflow
    }
    return false;
}

// Check if INT64 subtraction will overflow
bool sto_runtime_sub_will_overflow(int64_t a, int64_t b) {
    // a - b == a + (-b)
    // Overflow occurs when:
    // - b is INT64_MIN (cannot negate)
    // - a + (-b) overflows
    
    if (b == INT64_MIN) {
        return a > 0;  // Cannot negate INT64_MIN
    }
    
    return sto_runtime_add_will_overflow(a, -b);
}

// Check if INT64 multiplication will overflow
bool sto_runtime_mul_will_overflow(int64_t a, int64_t b) {
    // Special cases
    if (a == 0 || b == 0) {
        return false;  // 0 * anything = 0
    }
    
    if (a == 1 || b == 1) {
        return false;  // 1 * x = x
    }
    
    if (a == -1) {
        return b == INT64_MIN;  // -1 * INT64_MIN overflows
    }
    
    if (b == -1) {
        return a == INT64_MIN;  // INT64_MIN * -1 overflows
    }
    
    // General case: check if a * b would overflow
    // Use division to check: if a * b / a != b, overflow occurred
    int64_t result = a * b;
    if (result / a != b) {
        return true;
    }
    
    return false;
}

// Safe addition with overflow check
// Returns true if overflow occurred (result invalid)
bool sto_runtime_safe_add(int64_t a, int64_t b, int64_t* result) {
    if (sto_runtime_add_will_overflow(a, b)) {
        *result = 0;  // Invalid result
        return true;  // Overflow occurred
    }
    
    *result = a + b;
    return false;  // No overflow
}

// Safe subtraction with overflow check
bool sto_runtime_safe_sub(int64_t a, int64_t b, int64_t* result) {
    if (sto_runtime_sub_will_overflow(a, b)) {
        *result = 0;  // Invalid result
        return true;  // Overflow occurred
    }
    
    *result = a - b;
    return false;  // No overflow
}

// Safe multiplication with overflow check
bool sto_runtime_safe_mul(int64_t a, int64_t b, int64_t* result) {
    if (sto_runtime_mul_will_overflow(a, b)) {
        *result = 0;  // Invalid result
        return true;  // Overflow occurred
    }
    
    *result = a * b;
    return false;  // No overflow
}

// ============================================================================
// Phase 3.2: BigDecimal Runtime
// ============================================================================
// Full implementation in bigdecimal.c
// This file only contains basic creation functions

// Create BigDecimal from INT64
BigDecimal* sto_bigdec_from_int64(int64_t value) {
    BigDecimal* bd = (BigDecimal*)malloc(sizeof(BigDecimal));
    if (!bd) return NULL;
    
    bd->negative = (value < 0);
    uint64_t abs_value = (value < 0) ? (uint64_t)(-(value + 1)) + 1 : (uint64_t)value;
    
    // Convert to string
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)abs_value);
    
    bd->length = strlen(buffer);
    bd->digits = (char*)malloc(bd->length + 1);
    strcpy(bd->digits, buffer);
    bd->refcount = 1;
    
    return bd;
}

// Create BigDecimal from string
BigDecimal* sto_bigdec_from_string(const char* str) {
    if (!str) return NULL;
    
    BigDecimal* bd = (BigDecimal*)malloc(sizeof(BigDecimal));
    if (!bd) return NULL;
    
    bd->negative = (str[0] == '-');
    const char* digits = bd->negative ? str + 1 : str;
    
    bd->length = strlen(digits);
    bd->digits = (char*)malloc(bd->length + 1);
    strcpy(bd->digits, digits);
    bd->refcount = 1;
    
    return bd;
}

// Convert BigDecimal to string
char* sto_bigdec_to_string(BigDecimal* bd) {
    if (!bd) return NULL;
    
    size_t len = bd->length + (bd->negative ? 2 : 1);
    char* str = (char*)malloc(len);
    if (!str) return NULL;
    
    if (bd->negative) {
        str[0] = '-';
        strcpy(str + 1, bd->digits);
    } else {
        strcpy(str, bd->digits);
    }
    
    return str;
}

// Compare two BigDecimals
// Returns: -1 if a < b, 0 if a == b, 1 if a > b
int sto_bigdec_compare(BigDecimal* a, BigDecimal* b) {
    if (!a || !b) return 0;
    
    // Handle sign differences
    if (a->negative && !b->negative) return -1;  // negative < positive
    if (!a->negative && b->negative) return 1;   // positive > negative
    
    // Both same sign - compare magnitudes
    int len_cmp = a->length - b->length;
    
    if (len_cmp != 0) {
        // Different lengths
        if (a->negative) {
            // Both negative: longer magnitude = smaller value
            return len_cmp > 0 ? -1 : 1;
        } else {
            // Both positive: longer magnitude = larger value
            return len_cmp > 0 ? 1 : -1;
        }
    }
    
    // Same length - compare digit by digit
    int digit_cmp = strcmp(a->digits, b->digits);
    
    if (a->negative) {
        // Both negative: reverse comparison
        return digit_cmp > 0 ? -1 : (digit_cmp < 0 ? 1 : 0);
    } else {
        // Both positive: normal comparison
        return digit_cmp > 0 ? 1 : (digit_cmp < 0 ? -1 : 0);
    }
}

// Free BigDecimal
void sto_bigdec_free(BigDecimal* bd) {
    if (!bd) return;
    
    bd->refcount--;
    if (bd->refcount <= 0) {
        free(bd->digits);
        free(bd);
    }
}
// ============================================================================

// Create SSO string from C string
SSOString* sto_sso_create(const char* str) {
    if (!str) return NULL;
    
    SSOString* sso = (SSOString*)malloc(sizeof(SSOString));
    if (!sso) return NULL;
    
    size_t len = strlen(str);
    
    if (len <= 23) {
        // Use inline storage (SSO)
        memset(sso->data.inline_data, 0, 24);  // Clear all bytes
        strcpy(sso->data.inline_data, str);
        sso->flags = (len << 1);  // Shift left 1, bit 0 = 0 (not heap)
    } else {
        // Use heap storage
        sso->data.heap.heap_ptr = (char*)malloc(len + 1);
        if (!sso->data.heap.heap_ptr) {
            free(sso);
            return NULL;
        }
        strcpy(sso->data.heap.heap_ptr, str);
        sso->data.heap.length = len;
        sso->data.heap.capacity = len + 1;
        sso->flags = 1;  // Bit 0 = 1 (is heap)
    }
    
    return sso;
}

// Get string data (works for both SSO and heap)
const char* sto_sso_data(SSOString* sso) {
    if (!sso) return NULL;
    
    if (sso->flags & 1) {
        // Heap string
        return sso->data.heap.heap_ptr;
    } else {
        // SSO string
        return sso->data.inline_data;
    }
}

// Free SSO string
void sto_sso_free(SSOString* sso) {
    if (!sso) return;
    
    if (sso->flags & 1) {
        // Heap string - free the pointer
        free(sso->data.heap.heap_ptr);
    }
    // SSO string - nothing to free (inline storage)
    
    free(sso);
}

// ============================================================================
// Phase 3.4: Type Inference
// ============================================================================

// Infer numeric type from literal string
// Returns: 0 for INT64, 1 for BigDecimal
int sto_infer_numeric_type(const char* literal) {
    if (!literal) return 0;
    
    // Check for very long numbers (>19 digits = potential overflow)
    size_t len = strlen(literal);
    size_t digit_count = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isdigit(literal[i])) {
            digit_count++;
        }
    }
    
    // INT64_MAX = 9223372036854775807 (19 digits)
    // If more than 19 digits, definitely BigDecimal
    if (digit_count > 19) {
        return 1;  // BigDecimal
    }
    
    // If exactly 19 digits, check if > INT64_MAX
    if (digit_count == 19) {
        // Parse and compare
        int64_t value = 0;
        bool negative = (literal[0] == '-');
        const char* start = negative ? literal + 1 : literal;
        
        for (size_t i = 0; i < strlen(start); i++) {
            if (isdigit(start[i])) {
                int64_t digit = start[i] - '0';
                
                // Check for overflow before multiplying
                if (value > INT64_MAX / 10) {
                    return 1;  // Will overflow, use BigDecimal
                }
                value *= 10;
                
                // Check for overflow before adding
                if (value > INT64_MAX - digit) {
                    return 1;  // Will overflow, use BigDecimal
                }
                value += digit;
            }
        }
    }
    
    return 0;  // INT64 is sufficient
}

// Infer string storage type
// Returns: 0 for SSO (≤23 bytes), 1 for heap (>23 bytes)
int sto_infer_string_type(const char* literal) {
    if (!literal) return 0;
    
    size_t len = strlen(literal);
    return (len > 23) ? 1 : 0;
}

// ============================================================================
// Phase 3.5: Print Functions
// ============================================================================

// Print INT64 to stdout with newline
void sto_print_int64(int64_t value) {
    printf("%lld\n", (long long)value);
}

// Print double to stdout with newline
void sto_print_double(double value) {
    printf("%g\n", value);
}
