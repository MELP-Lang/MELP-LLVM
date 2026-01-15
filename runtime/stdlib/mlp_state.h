// YZ_34: Phase 8 - State Manager
// User-controlled lifecycle, STO-optimized, persistent storage

#ifndef MLP_STATE_H
#define MLP_STATE_H

#include <stdint.h>

// Lifecycle Management
int64_t mlp_state_init(void);
int64_t mlp_state_close(void);

// Data Operations
int64_t mlp_state_set(const char* key, const char* value);
char* mlp_state_get(const char* key);
int64_t mlp_state_has(const char* key);
int64_t mlp_state_delete(const char* key);
int64_t mlp_state_clear(void);

// Configuration
int64_t mlp_state_config_set(const char* key, const char* value);

// Persistence
int64_t mlp_state_save(void);
int64_t mlp_state_load(void);

#endif // MLP_STATE_H
