// ============================================================================
// BigDecimal - Arbitrary Precision Arithmetic
// ============================================================================
// Full implementation of arbitrary precision decimal numbers
// String-based representation for unlimited precision
//
// Architecture: Modular STO Runtime Component
// Author: MLP Compiler Team
// Date: 7 Aralık 2025

#define _POSIX_C_SOURCE 200809L  // For strdup

#include "runtime_sto.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// ============================================================================
// Helper Functions - String Arithmetic
// ============================================================================

// Reverse a string in place
static void reverse_string(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Remove leading zeros from string
static void remove_leading_zeros(char* str) {
    int i = 0;
    while (str[i] == '0' && str[i + 1] != '\0') {
        i++;
    }
    if (i > 0) {
        memmove(str, str + i, strlen(str) - i + 1);
    }
}

// Compare two positive number strings by magnitude
// Returns: -1 if a < b, 0 if a == b, 1 if a > b
static int compare_magnitude(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    
    // Different lengths - longer is bigger
    if (len_a > len_b) return 1;
    if (len_a < len_b) return -1;
    
    // Same length - lexicographic comparison
    return strcmp(a, b);
}

// Add two positive number strings
static char* add_positive(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int max_len = (len_a > len_b ? len_a : len_b) + 2;  // +1 for carry, +1 for null
    
    char* result = (char*)calloc(max_len, sizeof(char));
    if (!result) return NULL;
    
    int i = len_a - 1;
    int j = len_b - 1;
    int k = 0;
    int carry = 0;
    
    // Add digit by digit from right to left
    while (i >= 0 || j >= 0 || carry) {
        int digit_a = (i >= 0) ? (a[i] - '0') : 0;
        int digit_b = (j >= 0) ? (b[j] - '0') : 0;
        
        int sum = digit_a + digit_b + carry;
        result[k++] = (sum % 10) + '0';
        carry = sum / 10;
        
        i--;
        j--;
    }
    
    result[k] = '\0';
    reverse_string(result, k);
    remove_leading_zeros(result);
    
    return result;
}

// Subtract two positive number strings (assumes a >= b)
static char* subtract_positive(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int max_len = len_a + 1;  // +1 for null terminator
    
    char* result = (char*)calloc(max_len, sizeof(char));
    if (!result) return NULL;
    
    int i = len_a - 1;
    int j = len_b - 1;
    int k = 0;
    int borrow = 0;
    
    // Subtract digit by digit from right to left
    while (i >= 0) {
        int digit_a = a[i] - '0';
        int digit_b = (j >= 0) ? (b[j] - '0') : 0;
        
        int diff = digit_a - digit_b - borrow;
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result[k++] = diff + '0';
        
        i--;
        j--;
    }
    
    result[k] = '\0';
    reverse_string(result, k);
    remove_leading_zeros(result);
    
    // Handle case where result is "0"
    if (result[0] == '\0') {
        result[0] = '0';
        result[1] = '\0';
    }
    
    return result;
}

// Multiply two positive number strings
static char* multiply_positive(const char* a, const char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int result_len = len_a + len_b + 1;  // Maximum possible length
    
    int* result = (int*)calloc(result_len, sizeof(int));
    if (!result) return NULL;
    
    // Multiply digit by digit
    for (int i = len_a - 1; i >= 0; i--) {
        for (int j = len_b - 1; j >= 0; j--) {
            int digit_a = a[i] - '0';
            int digit_b = b[j] - '0';
            
            int product = digit_a * digit_b;
            int pos = (len_a - 1 - i) + (len_b - 1 - j);
            
            result[pos] += product;
            
            // Handle carry
            while (result[pos] >= 10) {
                result[pos + 1] += result[pos] / 10;
                result[pos] %= 10;
                pos++;
            }
        }
    }
    
    // Convert to string
    char* str_result = (char*)malloc(result_len);
    if (!str_result) {
        free(result);
        return NULL;
    }
    
    int k = 0;
    int i = result_len - 1;
    
    // Skip leading zeros
    while (i > 0 && result[i] == 0) {
        i--;
    }
    
    // Copy digits
    while (i >= 0) {
        str_result[k++] = result[i--] + '0';
    }
    
    str_result[k] = '\0';
    free(result);
    
    return str_result;
}

// ============================================================================
// Public BigDecimal Functions
// ============================================================================

BigDecimal* sto_bigdec_add(BigDecimal* a, BigDecimal* b) {
    if (!a || !b) return NULL;
    
    BigDecimal* result = (BigDecimal*)malloc(sizeof(BigDecimal));
    if (!result) return NULL;
    
    result->refcount = 1;
    
    // Same sign - add magnitudes
    if (a->negative == b->negative) {
        result->digits = add_positive(a->digits, b->digits);
        result->negative = a->negative;
    } else {
        // Different signs - subtract smaller from larger
        int cmp = compare_magnitude(a->digits, b->digits);
        
        if (cmp == 0) {
            // Same magnitude, different signs = 0
            result->digits = (char*)malloc(2);
            if (!result->digits) {
                free(result);
                return NULL;
            }
            result->digits[0] = '0';
            result->digits[1] = '\0';
            result->negative = false;
        } else if (cmp > 0) {
            // |a| > |b|
            result->digits = subtract_positive(a->digits, b->digits);
            result->negative = a->negative;
        } else {
            // |a| < |b|
            result->digits = subtract_positive(b->digits, a->digits);
            result->negative = b->negative;
        }
    }
    
    if (!result->digits) {
        free(result);
        return NULL;
    }
    
    result->length = strlen(result->digits);
    return result;
}

BigDecimal* sto_bigdec_sub(BigDecimal* a, BigDecimal* b) {
    if (!a || !b) return NULL;
    
    // a - b = a + (-b)
    BigDecimal temp_b = *b;
    temp_b.negative = !b->negative;
    
    return sto_bigdec_add(a, &temp_b);
}

BigDecimal* sto_bigdec_mul(BigDecimal* a, BigDecimal* b) {
    if (!a || !b) return NULL;
    
    BigDecimal* result = (BigDecimal*)malloc(sizeof(BigDecimal));
    if (!result) return NULL;
    
    result->refcount = 1;
    result->digits = multiply_positive(a->digits, b->digits);
    
    if (!result->digits) {
        free(result);
        return NULL;
    }
    
    // Sign: negative if exactly one operand is negative
    result->negative = (a->negative != b->negative);
    
    // Special case: 0 * anything = 0 (positive)
    if (strcmp(result->digits, "0") == 0) {
        result->negative = false;
    }
    
    result->length = strlen(result->digits);
    return result;
}

BigDecimal* sto_bigdec_div(BigDecimal* a, BigDecimal* b) {
    if (!a || !b) return NULL;
    
    // Check division by zero
    if (strcmp(b->digits, "0") == 0) {
        fprintf(stderr, "Error: Division by zero\n");
        return sto_bigdec_from_int64(0);
    }
    
    // TODO: Full arbitrary precision division
    // For now: Simple long division algorithm (integer division)
    
    // Placeholder: Convert to int64, divide, convert back
    // This loses precision for very large numbers!
    long long a_val = 0, b_val = 0;
    sscanf(a->digits, "%lld", &a_val);
    sscanf(b->digits, "%lld", &b_val);
    
    if (a->negative) a_val = -a_val;
    if (b->negative) b_val = -b_val;
    
    long long result_val = a_val / b_val;
    return sto_bigdec_from_int64(result_val);
}
