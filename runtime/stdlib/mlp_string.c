/**
 * MLP Standard Library - String Operations
 * 
 * Architecture: STO-compliant string operations
 * Works with both literal strings and heap-allocated strings
 * 
 * Created: 9 Aralık 2025 (YZ_06)
 */

#define _POSIX_C_SOURCE 200809L

#include "mlp_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

// ============================================================================
// String Concatenation
// ============================================================================

/**
 * Concatenate two strings
 * 
 * @param str1 First string (can be literal or heap)
 * @param str2 Second string (can be literal or heap)
 * @return Newly allocated concatenated string (caller must free)
 * 
 * Example:
 *   text greeting = "Hello"
 *   text target = "World"
 *   text message = greeting + " " + target  // "Hello World"
 */
char* mlp_string_concat(const char* str1, const char* str2) {
    // Null safety
    if (!str1 && !str2) return strdup("");
    if (!str1) return strdup(str2);
    if (!str2) return strdup(str1);
    
    // Calculate total length
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2;
    
    // Allocate result (heap)
    char* result = (char*)malloc(total_len + 1);
    if (!result) {
        fprintf(stderr, "Error: mlp_string_concat - malloc failed\n");
        return NULL;
    }
    
    // Copy strings
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    result[total_len] = '\0';
    
    return result;
}

/**
 * Concatenate three strings (optimization for multi-operand expressions)
 * 
 * @param str1 First string
 * @param str2 Second string
 * @param str3 Third string
 * @return Newly allocated concatenated string
 * 
 * Example:
 *   text a = "Hello"
 *   text b = " "
 *   text c = "World"
 *   text result = a + b + c  // Uses mlp_string_concat3 for efficiency
 */
char* mlp_string_concat3(const char* str1, const char* str2, const char* str3) {
    if (!str1) str1 = "";
    if (!str2) str2 = "";
    if (!str3) str3 = "";
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t total_len = len1 + len2 + len3;
    
    char* result = (char*)malloc(total_len + 1);
    if (!result) {
        fprintf(stderr, "Error: mlp_string_concat3 - malloc failed\n");
        return NULL;
    }
    
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    memcpy(result + len1 + len2, str3, len3);
    result[total_len] = '\0';
    
    return result;
}

// ============================================================================
// String Comparison
// ============================================================================

/**
 * Compare two strings lexicographically
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return < 0 if str1 < str2
 *         = 0 if str1 == str2
 *         > 0 if str1 > str2
 * 
 * Usage in MLP:
 *   if password == "admin"   → mlp_string_compare(password, "admin") == 0
 *   if name != "guest"       → mlp_string_compare(name, "guest") != 0
 *   if word < "banana"       → mlp_string_compare(word, "banana") < 0
 */
int mlp_string_compare(const char* str1, const char* str2) {
    // Null handling: NULL < any string, NULL == NULL
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;
    
    return strcmp(str1, str2);
}

/**
 * Check string equality
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return 1 if equal, 0 if not equal
 * 
 * Usage in MLP:
 *   if msg == "test"  → mlp_string_equals(msg, "test")
 */
int mlp_string_equals(const char* str1, const char* str2) {
    return mlp_string_compare(str1, str2) == 0;
}

/**
 * Check string inequality
 * 
 * @param str1 First string
 * @param str2 Second string
 * @return 1 if not equal, 0 if equal
 */
int mlp_string_not_equals(const char* str1, const char* str2) {
    return mlp_string_compare(str1, str2) != 0;
}

// ============================================================================
// Additional String Operations (Future expansion)
// ============================================================================

/**
 * Get string length
 * 
 * @param str String
 * @return Length of string (0 for NULL)
 */
size_t mlp_string_length(const char* str) {
    return str ? strlen(str) : 0;
}

/**
 * Check if string is empty
 * 
 * @param str String
 * @return 1 if empty or NULL, 0 otherwise
 */
int mlp_string_is_empty(const char* str) {
    return !str || str[0] == '\0';
}

/**
 * Duplicate a string
 * 
 * @param str Source string
 * @return New heap-allocated copy (caller must free)
 */
char* mlp_string_duplicate(const char* str) {
    return str ? strdup(str) : NULL;
}

/**
 * YZ_22: Extract substring
 * 
 * @param str Source string
 * @param start Starting index (0-based)
 * @param length Number of characters to extract
 * @return New heap-allocated substring (caller must free)
 */
char* mlp_string_substring(const char* str, size_t start, size_t length) {
    if (!str) return NULL;
    
    size_t str_len = strlen(str);
    if (start >= str_len) return strdup("");  // Out of bounds
    
    // Adjust length if it goes beyond string end
    if (start + length > str_len) {
        length = str_len - start;
    }
    
    // Allocate memory for substring + null terminator
    char* result = malloc(length + 1);
    if (!result) return NULL;
    
    // Copy substring
    memcpy(result, str + start, length);
    result[length] = '\0';
    
    return result;
}

/**
 * YZ_22: Find first occurrence of substring
 * 
 * @param str String to search in
 * @param substr Substring to find
 * @return Index of first occurrence, or -1 if not found
 */
int mlp_string_indexOf(const char* str, const char* substr) {
    if (!str || !substr) return -1;
    
    const char* found = strstr(str, substr);
    if (!found) return -1;
    
    return (int)(found - str);  // Calculate index
}

/**
 * Task 0.2: Get character at index (returns single-character string)
 * 
 * @param str Source string
 * @param index Position (0-based)
 * @return New heap-allocated single-character string (caller must free)
 *         Empty string if index out of bounds
 * 
 * Example:
 *   string s = "MELP"
 *   string ch = char_at(s; 0)  // "M"
 */
char* mlp_string_char_at(const char* str, size_t index) {
    if (!str) return strdup("");
    
    size_t len = strlen(str);
    if (index >= len) return strdup("");  // Out of bounds
    
    // Allocate 2 bytes: 1 char + null terminator
    char* result = (char*)malloc(2);
    if (!result) {
        fprintf(stderr, "Error: mlp_string_char_at - malloc failed\n");
        return NULL;
    }
    
    result[0] = str[index];
    result[1] = '\0';
    
    return result;
}

// ============================================================================
// Memory Management Helpers
// ============================================================================

/**
 * Free a string allocated by MLP stdlib
 * 
 * @param str String to free
 * 
 * Note: Do NOT free string literals (.rodata)!
 * Only free heap-allocated strings returned by concat/duplicate.
 */
void mlp_string_free(char* str) {
    if (str) {
        free(str);
    }
}

// ============================================================================
// YZ_90: Number to String Conversion (for string interpolation)
// ============================================================================

/**
 * Convert integer to string
 * 
 * @param num Integer number
 * @return New heap-allocated string (caller must free)
 * 
 * Example:
 *   numeric age = 25
 *   string msg = "Age: ${age}"  // Uses mlp_number_to_string internally
 */
char* mlp_number_to_string(long long num) {
    // Allocate buffer (max 64-bit int = 20 digits + sign + null)
    char* buffer = malloc(22);
    if (!buffer) {
        fprintf(stderr, "Error: mlp_number_to_string - malloc failed\n");
        exit(1);
    }
    
    snprintf(buffer, 22, "%lld", num);
    return buffer;
}

/**
 * Convert double to string
 * 
 * @param num Floating point number
 * @return New heap-allocated string (caller must free)
 * 
 * Example:
 *   numeric pi = 3.14159
 *   string msg = "Pi: ${pi}"  // Uses mlp_double_to_string internally
 */
char* mlp_double_to_string(double num) {
    // Allocate buffer (enough for most double representations)
    char* buffer = malloc(32);
    if (!buffer) {
        fprintf(stderr, "Error: mlp_double_to_string - malloc failed\n");
        exit(1);
    }
    
    // Use %.6f format (6 decimal places, remove trailing zeros)
    snprintf(buffer, 32, "%.6f", num);
    
    // Remove trailing zeros after decimal point
    char* dot = strchr(buffer, '.');
    if (dot) {
        char* end = buffer + strlen(buffer) - 1;
        while (end > dot && *end == '0') {
            *end = '\0';
            end--;
        }
        // Remove trailing dot if no decimals left
        if (end == dot) {
            *end = '\0';
        }
    }
    
    return buffer;
}

// ============================================================================
// Phase 5: String Case Conversion & Trimming (YZ_29)
// ============================================================================

/**
 * Convert string to uppercase
 * 
 * @param str Source string
 * @return New heap-allocated uppercase string (caller must free)
 * 
 * Example:
 *   string upper = toUpperCase("hello")  -- "HELLO"
 */
char* mlp_string_toUpperCase(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c >= 'a' && c <= 'z') {
            result[i] = c - 32;  // 'a' - 'A' = 32
        } else {
            result[i] = c;
        }
    }
    result[len] = '\0';
    
    return result;
}

/**
 * Convert string to lowercase
 * 
 * @param str Source string
 * @return New heap-allocated lowercase string (caller must free)
 * 
 * Example:
 *   string lower = toLowerCase("HELLO")  -- "hello"
 */
char* mlp_string_toLowerCase(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c >= 'A' && c <= 'Z') {
            result[i] = c + 32;  // 'A' + 32 = 'a'
        } else {
            result[i] = c;
        }
    }
    result[len] = '\0';
    
    return result;
}

/**
 * Trim whitespace from both ends of string
 * 
 * @param str Source string
 * @return New heap-allocated trimmed string (caller must free)
 * 
 * Example:
 *   string clean = trim("  hello  ")  -- "hello"
 */
char* mlp_string_trim(const char* str) {
    if (!str) return NULL;
    
    // Find start (skip leading whitespace)
    const char* start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // If string is all whitespace
    if (*start == '\0') {
        return strdup("");
    }
    
    // Find end (skip trailing whitespace)
    const char* end = str + strlen(str) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // Calculate length and copy
    size_t len = end - start + 1;
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    memcpy(result, start, len);
    result[len] = '\0';
    
    return result;
}

/**
 * Trim whitespace from start of string
 * 
 * @param str Source string
 * @return New heap-allocated trimmed string (caller must free)
 * 
 * Example:
 *   string clean = trimStart("  hello  ")  -- "hello  "
 */
char* mlp_string_trimStart(const char* str) {
    if (!str) return NULL;
    
    // Skip leading whitespace
    const char* start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    return strdup(start);
}

/**
 * Trim whitespace from end of string
 * 
 * @param str Source string
 * @return New heap-allocated trimmed string (caller must free)
 * 
 * Example:
 *   string clean = trimEnd("  hello  ")  -- "  hello"
 */
char* mlp_string_trimEnd(const char* str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return strdup("");
    
    // Find end (skip trailing whitespace)
    const char* end = str + len - 1;
    while (end >= str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // Calculate length and copy
    size_t new_len = end - str + 1;
    char* result = malloc(new_len + 1);
    if (!result) return NULL;
    
    memcpy(result, str, new_len);
    result[new_len] = '\0';
    
    return result;
}

// ============================================================================
// YZ_31: Replace & Split Functions
// ============================================================================

/**
 * Replace first occurrence of substring
 * 
 * @param str Source string
 * @param old_substr Substring to find and replace
 * @param new_substr Replacement string
 * @return New heap-allocated string with replacement (caller must free)
 * 
 * Example:
 *   string result = replace("hello world", "world", "MELP")  -- "hello MELP"
 */
char* mlp_string_replace(const char* str, const char* old_substr, const char* new_substr) {
    if (!str) return NULL;
    if (!old_substr || old_substr[0] == '\0') return strdup(str);
    if (!new_substr) new_substr = "";
    
    // Find first occurrence
    const char* found = strstr(str, old_substr);
    if (!found) return strdup(str);  // Not found, return copy
    
    size_t old_len = strlen(old_substr);
    size_t new_len = strlen(new_substr);
    size_t str_len = strlen(str);
    
    // Calculate new string length
    size_t result_len = str_len - old_len + new_len;
    char* result = malloc(result_len + 1);
    if (!result) return NULL;
    
    // Copy: prefix + new_substr + suffix
    size_t prefix_len = found - str;
    memcpy(result, str, prefix_len);
    memcpy(result + prefix_len, new_substr, new_len);
    memcpy(result + prefix_len + new_len, found + old_len, str_len - prefix_len - old_len);
    result[result_len] = '\0';
    
    return result;
}

/**
 * Replace all occurrences of substring
 * 
 * @param str Source string
 * @param old_substr Substring to find and replace
 * @param new_substr Replacement string
 * @return New heap-allocated string with all replacements (caller must free)
 * 
 * Example:
 *   string result = replaceAll("a-b-c", "-", "_")  -- "a_b_c"
 */
char* mlp_string_replaceAll(const char* str, const char* old_substr, const char* new_substr) {
    if (!str) return NULL;
    if (!old_substr || old_substr[0] == '\0') return strdup(str);
    if (!new_substr) new_substr = "";
    
    size_t old_len = strlen(old_substr);
    size_t new_len = strlen(new_substr);
    size_t str_len = strlen(str);
    
    // Count occurrences first
    int count = 0;
    const char* p = str;
    while ((p = strstr(p, old_substr)) != NULL) {
        count++;
        p += old_len;
    }
    
    if (count == 0) return strdup(str);  // Not found
    
    // Calculate new string length
    size_t result_len = str_len + count * (new_len - old_len);
    char* result = malloc(result_len + 1);
    if (!result) return NULL;
    
    // Build result string
    char* dest = result;
    const char* src = str;
    while ((p = strstr(src, old_substr)) != NULL) {
        // Copy prefix
        size_t prefix_len = p - src;
        memcpy(dest, src, prefix_len);
        dest += prefix_len;
        
        // Copy replacement
        memcpy(dest, new_substr, new_len);
        dest += new_len;
        
        src = p + old_len;
    }
    
    // Copy remainder
    strcpy(dest, src);
    
    return result;
}

/**
 * Split string by delimiter
 * 
 * @param str Source string
 * @param delimiter Delimiter to split by
 * @param count Output: number of parts
 * @return Array of heap-allocated strings (caller must free with mlp_string_split_free)
 * 
 * Example:
 *   int count;
 *   char** parts = split("a,b,c", ",", &count);  -- ["a", "b", "c"], count=3
 */
char** mlp_string_split(const char* str, const char* delimiter, int* count) {
    if (!str || !delimiter || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    *count = 0;
    size_t delim_len = strlen(delimiter);
    
    // Handle empty delimiter - return whole string as single element
    if (delim_len == 0) {
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = strdup(str);
        *count = 1;
        return result;
    }
    
    // Count parts first
    int parts = 1;
    const char* p = str;
    while ((p = strstr(p, delimiter)) != NULL) {
        parts++;
        p += delim_len;
    }
    
    // Allocate array
    char** result = malloc(parts * sizeof(char*));
    if (!result) return NULL;
    
    // Split the string
    int idx = 0;
    const char* start = str;
    p = str;
    
    while ((p = strstr(start, delimiter)) != NULL) {
        size_t part_len = p - start;
        result[idx] = malloc(part_len + 1);
        if (!result[idx]) {
            // Cleanup on failure
            for (int i = 0; i < idx; i++) free(result[i]);
            free(result);
            *count = 0;
            return NULL;
        }
        memcpy(result[idx], start, part_len);
        result[idx][part_len] = '\0';
        idx++;
        start = p + delim_len;
    }
    
    // Last part (after final delimiter)
    result[idx] = strdup(start);
    if (!result[idx]) {
        for (int i = 0; i < idx; i++) free(result[i]);
        free(result);
        *count = 0;
        return NULL;
    }
    
    *count = parts;
    return result;
}

/**
 * Free split result
 * 
 * @param parts Array returned by mlp_string_split
 * @param count Number of parts
 */
void mlp_string_split_free(char** parts, int count) {
    if (!parts) return;
    for (int i = 0; i < count; i++) {
        free(parts[i]);
    }
    free(parts);
}
