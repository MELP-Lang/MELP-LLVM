/**
 * MLP Math Functions Implementation
 * YZ_94: Basic math operations for MELP language
 */

#include "mlp_math.h"

/**
 * Return the minimum of two numbers
 */
int64_t min(int64_t a, int64_t b) {
    return (a < b) ? a : b;
}

/**
 * Return the maximum of two numbers
 */
int64_t max(int64_t a, int64_t b) {
    return (a > b) ? a : b;
}

/**
 * Return the absolute value
 * Note: We use mlp_abs to avoid conflict with stdlib abs()
 * The compiler generates calls to 'abs' which links to stdlib
 */
int64_t mlp_abs(int64_t x) {
    return (x < 0) ? -x : x;
}
