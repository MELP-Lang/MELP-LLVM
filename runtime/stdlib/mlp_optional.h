#ifndef MLP_OPTIONAL_H
#define MLP_OPTIONAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// MELP Runtime - Optional Type Support (YZ_202)
// ============================================================================
// Nullable types: numeric?, string?, boolean?
// Null safety: null coalescing (??), safe navigation (?.), null assertion (!)
// ============================================================================

// Optional state (None or Some)
typedef enum {
    MELP_OPT_NONE = 0,  // No value (null)
    MELP_OPT_SOME = 1   // Has value
} MelpOptionalState;

// Generic optional container
// Used for numeric?, string?, boolean?, etc.
typedef struct {
    MelpOptionalState state;  // NONE or SOME
    void* value;              // Pointer to actual value (NULL if NONE)
    size_t value_size;        // Size of value in bytes
} MelpOptional;

// ============================================================================
// Optional Creation
// ============================================================================

// Create optional with value (Some)
MelpOptional* melp_optional_some(void* value, size_t value_size);

// Create optional without value (None/null)
MelpOptional* melp_optional_none(void);

// Create optional numeric
MelpOptional* melp_optional_numeric(double value);

// Create optional string
MelpOptional* melp_optional_string(const char* value);

// Create optional boolean
MelpOptional* melp_optional_boolean(bool value);

// ============================================================================
// Optional Operations
// ============================================================================

// Check if optional has a value
bool melp_optional_has_value(MelpOptional* opt);

// Check if optional is null/none
bool melp_optional_is_null(MelpOptional* opt);

// Get value (panics if None)
void* melp_optional_get(MelpOptional* opt);

// Get numeric value (panics if None)
double melp_optional_get_numeric(MelpOptional* opt);

// Get string value (panics if None)
const char* melp_optional_get_string(MelpOptional* opt);

// Get boolean value (panics if None)
bool melp_optional_get_boolean(MelpOptional* opt);

// Get value or default (returns default if None)
void* melp_optional_get_or(MelpOptional* opt, void* default_value);

// Get numeric or default
double melp_optional_get_numeric_or(MelpOptional* opt, double default_value);

// Get string or default
const char* melp_optional_get_string_or(MelpOptional* opt, const char* default_value);

// Get boolean or default
bool melp_optional_get_boolean_or(MelpOptional* opt, bool default_value);

// ============================================================================
// Null Coalescing (a ?? b)
// ============================================================================

// Returns left if not null, otherwise right
void* melp_optional_coalesce(void* left, void* right);

// Numeric coalescing
double melp_optional_coalesce_numeric(MelpOptional* opt, double default_value);

// String coalescing
const char* melp_optional_coalesce_string(MelpOptional* opt, const char* default_value);

// Boolean coalescing
bool melp_optional_coalesce_boolean(MelpOptional* opt, bool default_value);

// ============================================================================
// Null Assertion (value!)
// ============================================================================

// Assert value is not null (panics with custom message if null)
void* melp_optional_assert(MelpOptional* opt, const char* message);

// ============================================================================
// Memory Management
// ============================================================================

// Free optional (but not the contained value)
void melp_optional_free(MelpOptional* opt);

// Free optional and contained value
void melp_optional_free_deep(MelpOptional* opt);

#endif // MLP_OPTIONAL_H
