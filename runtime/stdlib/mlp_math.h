/**
 * MLP Math Functions
 * YZ_94: Basic math operations for MELP language
 */

#ifndef MLP_MATH_H
#define MLP_MATH_H

#include <stdint.h>

/**
 * Return the minimum of two numbers
 */
int64_t min(int64_t a, int64_t b);

/**
 * Return the maximum of two numbers
 */
int64_t max(int64_t a, int64_t b);

/**
 * Return the absolute value (long version to match assembly calling convention)
 * Note: Standard abs() returns int, we need int64_t for MELP numeric type
 */
int64_t mlp_abs(int64_t x);

#endif /* MLP_MATH_H */
