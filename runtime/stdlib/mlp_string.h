/**
 * MLP Standard Library - String Operations Header
 * 
 * STO-compliant string operations for MELP compiler
 * 
 * Created: 9 Aralık 2025 (YZ_06)
 */

#ifndef MLP_STRING_H
#define MLP_STRING_H

#include <stddef.h>  // size_t

// String concatenation
char* mlp_string_concat(const char* str1, const char* str2);
char* mlp_string_concat3(const char* str1, const char* str2, const char* str3);

// String comparison
int mlp_string_compare(const char* str1, const char* str2);
int mlp_string_equals(const char* str1, const char* str2);
int mlp_string_not_equals(const char* str1, const char* str2);

// String properties
size_t mlp_string_length(const char* str);
int mlp_string_is_empty(const char* str);

// String manipulation
char* mlp_string_duplicate(const char* str);
char* mlp_string_substring(const char* str, size_t start, size_t length);  // YZ_22
int mlp_string_indexOf(const char* str, const char* substr);  // YZ_22
char* mlp_string_char_at(const char* str, size_t index);  // Task 0.2: Character access

// YZ_90: Number to string conversion (for string interpolation)
char* mlp_number_to_string(long long num);
char* mlp_double_to_string(double num);

// Phase 5: Case Conversion & Trimming (YZ_29)
char* mlp_string_toUpperCase(const char* str);
char* mlp_string_toLowerCase(const char* str);
char* mlp_string_trim(const char* str);
char* mlp_string_trimStart(const char* str);
char* mlp_string_trimEnd(const char* str);

// YZ_31: Replace & Split
char* mlp_string_replace(const char* str, const char* old_substr, const char* new_substr);
char* mlp_string_replaceAll(const char* str, const char* old_substr, const char* new_substr);
char** mlp_string_split(const char* str, const char* delimiter, int* count);
void mlp_string_split_free(char** parts, int count);

// Memory management
void mlp_string_free(char* str);

#endif // MLP_STRING_H
