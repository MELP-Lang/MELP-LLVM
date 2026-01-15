#ifndef MLP_PANIC_H
#define MLP_PANIC_H

// Array bounds checking
void mlp_panic_array_bounds(long index, long length, const char* array_name);

// Phase 6: Generic runtime error handler
void mlp_runtime_error(const char* message);

// Division by zero specific handler
void mlp_panic_division_by_zero(void);

#endif
