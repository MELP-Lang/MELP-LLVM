/**
 * MELP Runtime - File I/O Header
 */

#ifndef MELP_FILE_IO_H
#define MELP_FILE_IO_H

#include <stdint.h>

// File I/O
char* mlp_read_file(const char* path);
int64_t mlp_write_file(const char* path, const char* content);
int64_t mlp_append_file(const char* path, const char* content);
int mlp_file_exists(const char* path);
long mlp_file_size(const char* path);

// String operations
int64_t mlp_string_length(const char* str);
char* mlp_string_concat(const char* s1, const char* s2);
int mlp_string_equals(const char* s1, const char* s2);

#endif
