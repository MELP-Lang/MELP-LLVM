// YZ_34: Phase 8 - State Manager Implementation
// STO-optimized (SSO + Heap), File I/O based persistence

#include "mlp_state.h"
#include "mlp_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _GNU_SOURCE
char* strdup(const char* s);
#endif

// STO: Small String Optimization size
#define STATE_SSO_SIZE 24

// State Entry (STO-optimized)
typedef struct StateEntry {
    char* key;
    
    // STO: Value representation
    union {
        char sso_data[STATE_SSO_SIZE];  // ≤23 bytes (inline)
        char* heap_ptr;                  // >23 bytes (heap)
    } value;
    
    size_t value_len;
    uint8_t is_heap;  // 0 = SSO, 1 = heap
    
    struct StateEntry* next;  // Linked list
} StateEntry;

// State Manager
typedef struct {
    StateEntry* entries;
    int count;
    
    // Configuration
    int auto_persist;       // 0 = memory only, 1 = auto-save
    char* persist_file;     // Default: .melp_state.json
    
    // Statistics (for debugging)
    size_t total_sso_count;
    size_t total_heap_count;
    size_t total_heap_bytes;
} StateManager;

// Global instance
static StateManager* g_state_manager = NULL;

// Forward declarations
static StateEntry* find_entry(const char* key);
static StateEntry* create_entry(const char* key);
static void free_entry(StateEntry* entry);

// ============================================================================
// Lifecycle Management
// ============================================================================

int64_t mlp_state_init(void) {
    if (g_state_manager != NULL) {
        fprintf(stderr, "Warning: state_init() called twice! Ignoring.\n");
        return 0;
    }
    
    g_state_manager = (StateManager*)malloc(sizeof(StateManager));
    if (!g_state_manager) {
        fprintf(stderr, "Error: Failed to allocate state manager\n");
        return 0;
    }
    
    g_state_manager->entries = NULL;
    g_state_manager->count = 0;
    g_state_manager->auto_persist = 0;
    g_state_manager->persist_file = strdup(".melp_state.json");
    g_state_manager->total_sso_count = 0;
    g_state_manager->total_heap_count = 0;
    g_state_manager->total_heap_bytes = 0;
    
    return 1;
}

int64_t mlp_state_close(void) {
    if (g_state_manager == NULL) {
        return 0;
    }
    
    // Free all entries
    StateEntry* entry = g_state_manager->entries;
    while (entry) {
        StateEntry* next = entry->next;
        free_entry(entry);
        entry = next;
    }
    
    free(g_state_manager->persist_file);
    free(g_state_manager);
    g_state_manager = NULL;
    
    return 1;
}

// Auto-cleanup at program exit (GCC/Clang)
#ifndef _MSC_VER
__attribute__((destructor))
#endif
void mlp_state_auto_cleanup(void) {
    if (g_state_manager != NULL) {
        fprintf(stderr, "[State Manager] Auto-cleanup triggered\n");
        
        // Auto-save if enabled
        if (g_state_manager->auto_persist) {
            mlp_state_save();
        }
        
        mlp_state_close();
    }
}

// ============================================================================
// Data Operations
// ============================================================================

int64_t mlp_state_set(const char* key, const char* value) {
    if (g_state_manager == NULL) {
        fprintf(stderr, "Error: State manager not initialized. Call state_init() first!\n");
        return 0;
    }
    
    if (!key || !value) return 0;
    
    size_t len = strlen(value);
    StateEntry* entry = find_entry(key);
    
    // Create new entry if not found
    if (!entry) {
        entry = create_entry(key);
        if (!entry) return 0;
        
        // Add to list
        entry->next = g_state_manager->entries;
        g_state_manager->entries = entry;
        g_state_manager->count++;
    } else {
        // Free old value if heap
        if (entry->is_heap && entry->value.heap_ptr) {
            free(entry->value.heap_ptr);
            entry->value.heap_ptr = NULL;
        }
    }
    
    // STO Decision: SSO vs Heap
    if (len < STATE_SSO_SIZE) {
        // Small String Optimization (stack inline)
        memcpy(entry->value.sso_data, value, len + 1);
        entry->is_heap = 0;
        g_state_manager->total_sso_count++;
    } else {
        // Heap allocation (large data)
        entry->value.heap_ptr = (char*)malloc(len + 1);
        if (!entry->value.heap_ptr) {
            fprintf(stderr, "Error: Failed to allocate heap for large value\n");
            return 0;
        }
        strcpy(entry->value.heap_ptr, value);
        entry->is_heap = 1;
        g_state_manager->total_heap_count++;
        g_state_manager->total_heap_bytes += len;
    }
    
    entry->value_len = len;
    
    // Auto-persist if enabled
    if (g_state_manager->auto_persist) {
        mlp_state_save();
    }
    
    return 1;
}

char* mlp_state_get(const char* key) {
    if (g_state_manager == NULL) {
        fprintf(stderr, "Error: State manager not initialized\n");
        return strdup("");
    }
    
    if (!key) return strdup("");
    
    StateEntry* entry = find_entry(key);
    if (!entry) {
        return strdup("");  // Not found
    }
    
    // Return copy of value
    if (entry->is_heap) {
        return strdup(entry->value.heap_ptr);
    } else {
        return strdup(entry->value.sso_data);
    }
}

int64_t mlp_state_has(const char* key) {
    if (g_state_manager == NULL || !key) return 0;
    return find_entry(key) != NULL ? 1 : 0;
}

int64_t mlp_state_delete(const char* key) {
    if (g_state_manager == NULL || !key) return 0;
    
    StateEntry* prev = NULL;
    StateEntry* curr = g_state_manager->entries;
    
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            // Found - remove from list
            if (prev) {
                prev->next = curr->next;
            } else {
                g_state_manager->entries = curr->next;
            }
            
            free_entry(curr);
            g_state_manager->count--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    
    return 0;  // Not found
}

int64_t mlp_state_clear(void) {
    if (g_state_manager == NULL) return 0;
    
    StateEntry* entry = g_state_manager->entries;
    while (entry) {
        StateEntry* next = entry->next;
        free_entry(entry);
        entry = next;
    }
    
    g_state_manager->entries = NULL;
    g_state_manager->count = 0;
    g_state_manager->total_sso_count = 0;
    g_state_manager->total_heap_count = 0;
    g_state_manager->total_heap_bytes = 0;
    
    return 1;
}

// ============================================================================
// Configuration
// ============================================================================

int64_t mlp_state_config_set(const char* key, const char* value) {
    if (g_state_manager == NULL) {
        fprintf(stderr, "Error: State manager not initialized\n");
        return 0;
    }
    
    if (!key || !value) return 0;
    
    if (strcmp(key, "auto_persist") == 0) {
        g_state_manager->auto_persist = atoi(value);
        return 1;
    }
    
    if (strcmp(key, "persist_file") == 0) {
        free(g_state_manager->persist_file);
        g_state_manager->persist_file = strdup(value);
        return 1;
    }
    
    fprintf(stderr, "Warning: Unknown config key: %s\n", key);
    return 0;
}

// ============================================================================
// Persistence (Simple JSON)
// ============================================================================

int64_t mlp_state_save(void) {
    if (g_state_manager == NULL) return 0;
    
    // Build simple JSON
    char* json = (char*)malloc(1024 * 1024);  // 1MB buffer
    if (!json) return 0;
    
    strcpy(json, "{\n");
    
    StateEntry* entry = g_state_manager->entries;
    int first = 1;
    
    while (entry) {
        if (!first) strcat(json, ",\n");
        first = 0;
        
        strcat(json, "  \"");
        strcat(json, entry->key);
        strcat(json, "\": \"");
        
        // Get value
        const char* val = entry->is_heap ? entry->value.heap_ptr : entry->value.sso_data;
        
        // Escape special characters
        size_t current_len = strlen(json);
        const char* p = val;
        while (*p) {
            if (*p == '"' || *p == '\\') {
                json[current_len++] = '\\';
            }
            json[current_len++] = *p++;
        }
        json[current_len] = '\0';
        
        strcat(json, "\"");
        
        entry = entry->next;
    }
    
    strcat(json, "\n}\n");
    
    // Write to file
    int64_t result = mlp_write_file(g_state_manager->persist_file, json);
    free(json);
    
    return result;
}

int64_t mlp_state_load(void) {
    if (g_state_manager == NULL) {
        fprintf(stderr, "Error: State manager not initialized\n");
        return 0;
    }
    
    // Read file
    char* json = mlp_read_file(g_state_manager->persist_file);
    if (!json || strlen(json) == 0) {
        if (json) free(json);
        return 0;  // File not found or empty
    }
    
    // Simple JSON parser (key-value pairs only)
    char* p = json;
    
    while (*p) {
        // Skip whitespace and {
        while (*p && (*p == ' ' || *p == '\n' || *p == '\t' || *p == '{' || *p == ',')) p++;
        if (*p == '}' || *p == '\0') break;
        
        // Parse key
        if (*p != '"') break;
        p++;  // Skip opening "
        
        char key[256] = {0};
        int i = 0;
        while (*p && *p != '"' && i < 255) {
            key[i++] = *p++;
        }
        if (*p != '"') break;
        p++;  // Skip closing "
        
        // Skip : and whitespace
        while (*p && (*p == ':' || *p == ' ' || *p == '\t' || *p == '\n')) p++;
        
        // Parse value
        if (*p != '"') break;
        p++;  // Skip opening "
        
        char value[4096] = {0};
        i = 0;
        while (*p && *p != '"' && i < 4095) {
            if (*p == '\\' && *(p+1)) {
                p++;  // Skip escape char
            }
            value[i++] = *p++;
        }
        if (*p != '"') break;
        p++;  // Skip closing "
        
        // Set state
        mlp_state_set(key, value);
    }
    
    free(json);
    return 1;
}

// ============================================================================
// Helper Functions
// ============================================================================

static StateEntry* find_entry(const char* key) {
    StateEntry* entry = g_state_manager->entries;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static StateEntry* create_entry(const char* key) {
    StateEntry* entry = (StateEntry*)malloc(sizeof(StateEntry));
    if (!entry) return NULL;
    
    entry->key = strdup(key);
    entry->value_len = 0;
    entry->is_heap = 0;
    entry->next = NULL;
    
    return entry;
}

static void free_entry(StateEntry* entry) {
    if (!entry) return;
    
    if (entry->key) free(entry->key);
    if (entry->is_heap && entry->value.heap_ptr) {
        free(entry->value.heap_ptr);
    }
    
    free(entry);
}
