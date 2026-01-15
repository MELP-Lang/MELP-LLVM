/**
 * MELP Runtime - File I/O Implementation
 * 
 * Based on guvenacar/mlp runtime/runtime.c (proven, 22 Nov 2024)
 * Adapted for MELP self-hosting requirements
 * 
 * Functions:
 *   - mlp_read_file(path) -> char* (caller must free)
 *   - mlp_write_file(path, content) -> int64 (0 success, -1 fail)
 *   - mlp_file_exists(path) -> int (1/0)
 *   - mlp_file_size(path) -> long
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Read entire file into memory
 * @param path - File path to read
 * @return char* - File contents (caller must free), NULL on error
 */
char* mlp_read_file(const char* path) {
    if (!path) {
        return NULL;
    }
    
    FILE* file = fopen(path, "r");
    if (!file) {
        // Silent fail - no println in self-hosting
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    // Allocate buffer
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // Read content
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';  // Null-terminate

    fclose(file);
    return buffer;
}

/**
 * Write content to file
 * @param path - File path to write
 * @param content - String content to write
 * @return int64_t - 0 on success, -1 on error
 */
int64_t mlp_write_file(const char* path, const char* content) {
    if (!path || !content) {
        return -1;
    }
    
    FILE* file = fopen(path, "w");
    if (!file) {
        return -1;
    }

    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);

    fclose(file);
    return (written == len) ? 0 : -1;
}

/**
 * Append content to file
 * @param path - File path to append
 * @param content - String content to append
 * @return int64_t - 0 on success, -1 on error
 */
int64_t mlp_append_file(const char* path, const char* content) {
    if (!path || !content) {
        return -1;
    }
    
    FILE* file = fopen(path, "a");
    if (!file) {
        return -1;
    }

    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);

    fclose(file);
    return (written == len) ? 0 : -1;
}

/**
 * Check if file exists
 * @param path - File path to check
 * @return int - 1 if exists, 0 otherwise
 */
int mlp_file_exists(const char* path) {
    if (!path) {
        return 0;
    }
    
    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * Get file size
 * @param path - File path
 * @return long - File size in bytes, -1 on error
 */
long mlp_file_size(const char* path) {
    if (!path) {
        return -1;
    }
    
    FILE* file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

/**
 * String length (for LLVM IR compatibility)
 */
int64_t mlp_string_length(const char* str) {
    if (!str) return 0;
    return (int64_t)strlen(str);
}

/**
 * String concatenation (allocates new string)
 */
char* mlp_string_concat(const char* s1, const char* s2) {
    if (!s1 && !s2) return NULL;
    if (!s1) return strdup(s2);
    if (!s2) return strdup(s1);
    
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*)malloc(len1 + len2 + 1);
    if (!result) return NULL;
    
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * String comparison
 */
int mlp_string_equals(const char* s1, const char* s2) {
    if (s1 == s2) return 1;
    if (!s1 || !s2) return 0;
    return strcmp(s1, s2) == 0 ? 1 : 0;
}
