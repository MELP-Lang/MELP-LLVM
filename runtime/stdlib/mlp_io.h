/**
 * MLP Standard Library - I/O Functions Header
 * 
 * Architecture: Works with STO (Smart Type Optimization)
 * User only sees: numeric, string, boolean
 * Runtime handles: int64/BigDecimal, SSO/heap internally
 */

#ifndef MLP_IO_H
#define MLP_IO_H

#include <stdint.h>
#include "../sto/sto_types.h"  // Use STO type definitions

// Legacy compatibility (deprecated - use INTERNAL_TYPE_* from sto_types.h)
#define STO_TYPE_INT64      INTERNAL_TYPE_INT64
#define STO_TYPE_DOUBLE     INTERNAL_TYPE_DOUBLE
#define STO_TYPE_BIGDECIMAL INTERNAL_TYPE_BIGDECIMAL
#define STO_TYPE_STRING     INTERNAL_TYPE_SSO_STRING
#define STO_TYPE_BOOLEAN    INTERNAL_TYPE_BOOLEAN

// ============================================================================
// Core I/O Functions (STO-aware)
// ============================================================================

// Print numeric value with newline
// value: pointer to numeric (int64*, double*, or BigDecimal*)
// sto_type: INTERNAL_TYPE_INT64, INTERNAL_TYPE_DOUBLE, or INTERNAL_TYPE_BIGDECIMAL
void mlp_println_numeric(void* value, uint8_t sto_type);

// Print string with newline
void mlp_println_string(const char* str);

// Print boolean with newline
void mlp_println_bool(int value);

// Print without newline
void mlp_print_numeric(void* value, uint8_t sto_type);
void mlp_print_string(const char* str);
void mlp_print_bool(int value);

// Convert numeric to string
// Returns: dynamically allocated string (caller must free)
char* mlp_toString_numeric(void* value, uint8_t sto_type);
char* mlp_toString_bool(int value);

// Simple integer to string (Stage 0/Stage 1 bootstrap)
// BOOTSTRAP_YZ_01: Added for codegen numeric to string conversion
char* mlp_numeric_to_string(int64_t value);

// ============================================================================
// Input Functions (YZ_31 - Phase 6 completion)
// ============================================================================

// Read a line of text from stdin
// Returns: dynamically allocated string (caller must free)
// If EOF or error, returns empty string ""
char* mlp_input(void);

// Read a line and prompt first
// prompt: Text to display before reading input
// Returns: dynamically allocated string (caller must free)
char* mlp_input_prompt(const char* prompt);

// Read a numeric value from stdin
// Returns: int64_t value (0 on error)
int64_t mlp_input_numeric(void);

// Read a numeric with prompt
// prompt: Text to display before reading input
// Returns: int64_t value (0 on error)
int64_t mlp_input_numeric_prompt(const char* prompt);

// ============================================================================
// File I/O Functions (YZ_33 - Phase 9)
// ============================================================================

// Read entire file content as string
// filename: Path to file to read
// Returns: dynamically allocated string with file content (caller must free)
//          Returns empty string "" on error (file not found, permission denied, etc.)
char* mlp_read_file(const char* filename);

// Write string content to file (overwrites existing file)
// filename: Path to file to write
// content: String content to write
// Returns: 1 on success, 0 on error
int64_t mlp_write_file(const char* filename, const char* content);

// Append string content to file (creates file if doesn't exist)
// filename: Path to file to append to
// content: String content to append
// Returns: 1 on success, 0 on error
int64_t mlp_append_file(const char* filename, const char* content);

// ============================================================================
// Simple Wrapper Functions (BOOTSTRAP_YZ_07 - Stage 2 Bootstrap)
// ============================================================================

// Simple wrappers for Stage 2 compiler (non-STO, direct i64)
// Stage 2 generates: call void @mlp_print_numeric(i64 %value)
// These wrappers convert to STO-aware calls with INTERNAL_TYPE_INT64

void mlp_print_numeric_simple(int64_t value);
void mlp_println_numeric_simple(int64_t value);
void mlp_print_boolean_simple(int value);
void mlp_println_boolean_simple(int value);

#endif
