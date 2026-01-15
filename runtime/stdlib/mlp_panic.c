#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// ANSI Colors for error output
// ============================================================================
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BOLD_RED "\033[1;31m"

void mlp_panic_array_bounds(long index, long length, const char* array_name) {
    fprintf(stderr, "\n");
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "%sRUNTIME ERROR: Array Index Out of Bounds%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "Array: %s\n", array_name ? array_name : "(unknown)");
    fprintf(stderr, "Index: %ld\n", index);
    fprintf(stderr, "Valid range: 0 to %ld\n", length - 1);
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "\n");
    exit(42);  // Exit code 42 = bounds check failure
}

// ============================================================================
// Phase 6: Generic runtime error handler
// ============================================================================
void mlp_runtime_error(const char* message) {
    fprintf(stderr, "\n");
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "%sRUNTIME ERROR%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "%s\n", message ? message : "Unknown error");
    fprintf(stderr, "%s==========================================================%s\n", COLOR_BOLD_RED, COLOR_RESET);
    fprintf(stderr, "\n");
    exit(43);  // Exit code 43 = runtime error
}

// Division by zero specific handler
void mlp_panic_division_by_zero(void) {
    mlp_runtime_error("Division by zero is not allowed!");
}
