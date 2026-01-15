/**
 * MLP Standard Library - Stage 2 Bootstrap Wrappers
 * 
 * Purpose: Simple wrappers for Stage 2 compiler (non-STO, direct i64)
 * Author: BOOTSTRAP_YZ_07
 * Date: 15 Ocak 2026
 * 
 * Stage 2 generates simple signatures:
 *   declare void @mlp_print_numeric(i64)
 *   declare void @mlp_print_string(i8*)
 *   declare void @mlp_print_boolean(i1)
 * 
 * These wrappers match those signatures and convert to STO-aware calls.
 */

#include <stdint.h>

// STO type constants (from sto_types.h)
#define INTERNAL_TYPE_INT64 1

// Forward declarations of STO-aware functions (from mlp_io.c)
// Note: In libmlp_stage2.a, mlp_print_numeric (STO-aware) is renamed to mlp_print_numeric_sto
// to avoid conflict with our Stage 2 wrapper
extern void mlp_print_numeric_sto(void* value, uint8_t sto_type);
extern void mlp_print_bool(int value);

// ============================================================================
// Stage 2 Bootstrap Wrappers (match Stage 2 generated signatures)
// ============================================================================

// Print numeric (i64) - Stage 2 compatible signature
// Stage 2 generates: call void @mlp_print_numeric(i64 %value)
// 
// This function will be renamed to mlp_print_numeric by objcopy in Makefile
void mlp_print_numeric_s2(int64_t value) {
    // Call the STO-aware version (renamed to mlp_print_numeric_sto) with INTERNAL_TYPE_INT64
    mlp_print_numeric_sto(&value, INTERNAL_TYPE_INT64);
}

// Print boolean (i1) - Stage 2 compatible signature
// Stage 2 generates: call void @mlp_print_boolean(i1 %value)
void mlp_print_boolean(int value) {
    mlp_print_bool(value);
}

// Note: mlp_print_string already has correct signature in mlp_io.c
// declare void @mlp_print_string(i8*)
// No wrapper needed!
