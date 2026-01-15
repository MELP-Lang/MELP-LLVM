// Define _POSIX_C_SOURCE for strdup
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "mlp_optional.h"
#include "mlp_panic.h"
#include <stdlib.h>
#include <string.h>

// ============================================================================
// MELP Runtime - Optional Type Implementation (YZ_202)
// ============================================================================

// ============================================================================
// Optional Creation
// ============================================================================

MelpOptional* melp_optional_some(void* value, size_t value_size) {
    MelpOptional* opt = (MelpOptional*)malloc(sizeof(MelpOptional));
    if (!opt) {
        melp_runtime_error("Failed to allocate memory for optional");
    }
    
    opt->state = MELP_OPT_SOME;
    opt->value = value;
    opt->value_size = value_size;
    return opt;
}

MelpOptional* melp_optional_none(void) {
    MelpOptional* opt = (MelpOptional*)malloc(sizeof(MelpOptional));
    if (!opt) {
        melp_runtime_error("Failed to allocate memory for optional");
    }
    
    opt->state = MELP_OPT_NONE;
    opt->value = NULL;
    opt->value_size = 0;
    return opt;
}

MelpOptional* melp_optional_numeric(double value) {
    double* heap_value = (double*)malloc(sizeof(double));
    if (!heap_value) {
        melp_runtime_error("Failed to allocate memory for optional numeric");
    }
    *heap_value = value;
    return melp_optional_some(heap_value, sizeof(double));
}

MelpOptional* melp_optional_string(const char* value) {
    if (!value) {
        return melp_optional_none();
    }
    
    char* heap_value = strdup(value);
    if (!heap_value) {
        melp_runtime_error("Failed to allocate memory for optional string");
    }
    return melp_optional_some(heap_value, strlen(value) + 1);
}

MelpOptional* melp_optional_boolean(bool value) {
    bool* heap_value = (bool*)malloc(sizeof(bool));
    if (!heap_value) {
        melp_runtime_error("Failed to allocate memory for optional boolean");
    }
    *heap_value = value;
    return melp_optional_some(heap_value, sizeof(bool));
}

// ============================================================================
// Optional Operations
// ============================================================================

bool melp_optional_has_value(MelpOptional* opt) {
    if (!opt) return false;
    return opt->state == MELP_OPT_SOME;
}

bool melp_optional_is_null(MelpOptional* opt) {
    return !melp_optional_has_value(opt);
}

void* melp_optional_get(MelpOptional* opt) {
    if (!opt) {
        melp_runtime_error("optional_get() called on NULL pointer");
    }
    if (opt->state == MELP_OPT_NONE) {
        melp_runtime_error("optional_get() called on None value");
    }
    return opt->value;
}

double melp_optional_get_numeric(MelpOptional* opt) {
    void* value = melp_optional_get(opt);
    return *(double*)value;
}

const char* melp_optional_get_string(MelpOptional* opt) {
    void* value = melp_optional_get(opt);
    return (const char*)value;
}

bool melp_optional_get_boolean(MelpOptional* opt) {
    void* value = melp_optional_get(opt);
    return *(bool*)value;
}

void* melp_optional_get_or(MelpOptional* opt, void* default_value) {
    if (!opt || opt->state == MELP_OPT_NONE) {
        return default_value;
    }
    return opt->value;
}

double melp_optional_get_numeric_or(MelpOptional* opt, double default_value) {
    if (!opt || opt->state == MELP_OPT_NONE) {
        return default_value;
    }
    return *(double*)opt->value;
}

const char* melp_optional_get_string_or(MelpOptional* opt, const char* default_value) {
    if (!opt || opt->state == MELP_OPT_NONE) {
        return default_value;
    }
    return (const char*)opt->value;
}

bool melp_optional_get_boolean_or(MelpOptional* opt, bool default_value) {
    if (!opt || opt->state == MELP_OPT_NONE) {
        return default_value;
    }
    return *(bool*)opt->value;
}

// ============================================================================
// Null Coalescing (a ?? b)
// ============================================================================

void* melp_optional_coalesce(void* left, void* right) {
    return left ? left : right;
}

double melp_optional_coalesce_numeric(MelpOptional* opt, double default_value) {
    return melp_optional_get_numeric_or(opt, default_value);
}

const char* melp_optional_coalesce_string(MelpOptional* opt, const char* default_value) {
    return melp_optional_get_string_or(opt, default_value);
}

bool melp_optional_coalesce_boolean(MelpOptional* opt, bool default_value) {
    return melp_optional_get_boolean_or(opt, default_value);
}

// ============================================================================
// Null Assertion (value!)
// ============================================================================

void* melp_optional_assert(MelpOptional* opt, const char* message) {
    if (!opt || opt->state == MELP_OPT_NONE) {
        if (message) {
            melp_runtime_error(message);
        } else {
            melp_runtime_error("Null assertion failed: value is None");
        }
    }
    return opt->value;
}

// ============================================================================
// Memory Management
// ============================================================================

void melp_optional_free(MelpOptional* opt) {
    if (opt) {
        free(opt);
    }
}

void melp_optional_free_deep(MelpOptional* opt) {
    if (opt) {
        if (opt->value) {
            free(opt->value);
        }
        free(opt);
    }
}
