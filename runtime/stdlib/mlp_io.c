/**
 * MLP Standard Library - I/O Functions
 * 
 * Architecture: Works with STO (Smart Type Optimization)
 * Handles int64, double, BigDecimal transparently
 */

#include "mlp_io.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

// For strdup on older systems
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
char* strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}
#endif

// External STO runtime functions (from libsto_runtime.a)
extern char* sto_bigdec_to_string(void* bigdec);

// ============================================================================
// Core STO-Aware Functions
// ============================================================================

// Print numeric value with newline (STO-aware)
void mlp_println_numeric(void* value, uint8_t sto_type) {
    if (!value) {
        printf("(null)\n");
        return;
    }
    
    switch (sto_type) {
        case INTERNAL_TYPE_INT64:
            printf("%" PRId64 "\n", *(int64_t*)value);
            break;
            
        case INTERNAL_TYPE_DOUBLE:
            printf("%g\n", *(double*)value);
            break;
            
        case INTERNAL_TYPE_BIGDECIMAL: {
            char* str = sto_bigdec_to_string(value);
            if (str) {
                printf("%s\n", str);
                free(str);
            } else {
                printf("(BigDecimal error)\n");
            }
            break;
        }
        
        default:
            printf("(unknown numeric type: %d)\n", sto_type);
    }
}

// Print without newline (STO-aware)
void mlp_print_numeric(void* value, uint8_t sto_type) {
    if (!value) {
        printf("(null)");
        return;
    }
    
    switch (sto_type) {
        case INTERNAL_TYPE_INT64:
            printf("%" PRId64, *(int64_t*)value);
            break;
            
        case INTERNAL_TYPE_DOUBLE:
            printf("%g", *(double*)value);
            break;
            
        case INTERNAL_TYPE_BIGDECIMAL: {
            char* str = sto_bigdec_to_string(value);
            if (str) {
                printf("%s", str);
                free(str);
            } else {
                printf("(BigDecimal error)");
            }
            break;
        }
        
        default:
            printf("(unknown numeric type: %d)", sto_type);
    }
}

// Convert numeric to string (STO-aware)
char* mlp_toString_numeric(void* value, uint8_t sto_type) {
    if (!value) return strdup("null");
    
    char buffer[64];
    switch (sto_type) {
        case INTERNAL_TYPE_INT64:
            snprintf(buffer, sizeof(buffer), "%" PRId64, *(int64_t*)value);
            return strdup(buffer);
            
        case INTERNAL_TYPE_DOUBLE:
            snprintf(buffer, sizeof(buffer), "%g", *(double*)value);
            return strdup(buffer);
            
        case INTERNAL_TYPE_BIGDECIMAL:
            return sto_bigdec_to_string(value);  // Already allocates
            
        default:
            return strdup("(unknown)");
    }
}

// Simple integer to string conversion (for Stage 0/Stage 1 bootstrap)
// BOOTSTRAP_YZ_01: Added for codegen_minimal.mlp - converts i64 to string
char* mlp_numeric_to_string(int64_t value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%" PRId64, value);
    return strdup(buffer);
}

// ============================================================================
// String & Boolean Functions (unchanged)
// ============================================================================

// Print string with newline
void mlp_println_string(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("(null)\n");
    }
}

// Print boolean with newline
void mlp_println_bool(int value) {
    printf("%s\n", value ? "true" : "false");
}

// Print without newline variants
void mlp_print_string(const char* str) {
    if (str) {
        printf("%s", str);
    }
}

void mlp_print_bool(int value) {
    printf("%s", value ? "true" : "false");
}

// Convert to string
char* mlp_toString_bool(int value) {
    return strdup(value ? "true" : "false");
}

// ============================================================================
// Input Functions (YZ_31 - Phase 6 completion)
// ============================================================================

// Read a line of text from stdin
// Returns: dynamically allocated string (caller must free)
char* mlp_input(void) {
    char buffer[1024];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return strdup("");  // EOF or error
    }
    
    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return strdup(buffer);
}

// Read a line with prompt
char* mlp_input_prompt(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    return mlp_input();
}

// Read a numeric value from stdin
int64_t mlp_input_numeric(void) {
    char buffer[64];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;  // EOF or error
    }
    
    // Parse as integer
    return atoll(buffer);
}

// Read a numeric with prompt
int64_t mlp_input_numeric_prompt(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    return mlp_input_numeric();
}

// ============================================================================
// File I/O Functions (YZ_33 - Phase 9)
// ============================================================================

// Read entire file content as string
char* mlp_read_file(const char* filename) {
    if (!filename) return strdup("");
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        return strdup("");  // File not found or permission denied
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer
    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return strdup("");
    }
    
    // Read entire file
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';
    
    fclose(file);
    return content;
}

// Write string content to file (overwrite)
int64_t mlp_write_file(const char* filename, const char* content) {
    if (!filename || !content) return 0;
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return 0;  // Permission denied or invalid path
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 1 : 0;
}

// Append string content to file
int64_t mlp_append_file(const char* filename, const char* content) {
    if (!filename || !content) return 0;
    
    FILE* file = fopen(filename, "a");
    if (!file) {
        return 0;  // Permission denied or invalid path
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 1 : 0;
}

// ============================================================================
// Simple Wrapper Functions (BOOTSTRAP_YZ_07 - Stage 2 Bootstrap)
// ============================================================================

// Simple wrapper for Stage 2 compiler (non-STO, direct i64)
// Stage 2 generates: call void @mlp_print_numeric(i64 %value)
// This wrapper converts to STO-aware call with INTERNAL_TYPE_INT64
void mlp_print_numeric_simple(int64_t value) {
    mlp_print_numeric(&value, INTERNAL_TYPE_INT64);
}

void mlp_println_numeric_simple(int64_t value) {
    mlp_println_numeric(&value, INTERNAL_TYPE_INT64);
}

void mlp_print_boolean_simple(int value) {
    mlp_print_bool(value);
}

void mlp_println_boolean_simple(int value) {
    mlp_println_bool(value);
}
