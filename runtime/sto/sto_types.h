#ifndef STO_TYPES_H
#define STO_TYPES_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// STO (Smart Type Optimization) Type Definitions
// ============================================================================
// Shared between compiler and runtime

// Internal representation types for STO
typedef enum {
    INTERNAL_TYPE_UNKNOWN = 0,
    
    // Numeric types (user sees: numeric)
    INTERNAL_TYPE_INT64,        // -2^63 to 2^63-1 (8 bytes, register/stack)
    INTERNAL_TYPE_DOUBLE,       // IEEE 754 double (8 bytes, XMM register)
    INTERNAL_TYPE_BIGDECIMAL,   // Unlimited precision (heap allocated)
    
    // Text types (user sees: string)
    INTERNAL_TYPE_SSO_STRING,   // Small String Optimization (≤23 bytes, stack)
    INTERNAL_TYPE_HEAP_STRING,  // Heap allocated string (>23 bytes)
    INTERNAL_TYPE_RODATA_STRING,// Constant string in .rodata section
    
    // Other types
    INTERNAL_TYPE_BOOLEAN,
    INTERNAL_TYPE_ARRAY,
    INTERNAL_TYPE_STRUCT,
    INTERNAL_TYPE_FUNCTION
} InternalType;

// STO analysis result
typedef struct STOTypeInfo {
    InternalType type;          // Inferred internal type
    bool is_constant;           // Can be computed at compile time?
    bool needs_promotion;       // May need runtime promotion?
    
    // For numeric types
    union {
        int64_t int_value;      // For INT64
        double double_value;    // For DOUBLE
        char* string_value;     // For STRING types
    } const_value;
    
    // Memory location hint
    enum {
        MEM_REGISTER,           // CPU register (rax, xmm0, etc.)
        MEM_STACK,              // Stack frame
        MEM_HEAP,               // Heap allocated
        MEM_RODATA              // Read-only data section
    } mem_location;
} STOTypeInfo;

#endif
