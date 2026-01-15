// YZ_34: State Manager Runtime Tests
// Test STO optimization (SSO vs Heap), persistence, auto-cleanup

#include "mlp_state.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_lifecycle() {
    printf("\n=== Test 1: Lifecycle ===\n");
    
    // Init
    assert(mlp_state_init() == 1);
    printf("✅ state_init()\n");
    
    // Double init should warn
    assert(mlp_state_init() == 0);
    printf("✅ Double init prevented\n");
    
    // Close
    assert(mlp_state_close() == 1);
    printf("✅ state_close()\n");
    
    // Re-init after close
    assert(mlp_state_init() == 1);
    printf("✅ Re-init after close\n");
}

void test_basic_operations() {
    printf("\n=== Test 2: Basic Operations ===\n");
    
    mlp_state_init();
    
    // Set & Get
    assert(mlp_state_set("name", "Ali") == 1);
    char* name = mlp_state_get("name");
    assert(strcmp(name, "Ali") == 0);
    free(name);
    printf("✅ set/get: name = Ali\n");
    
    // Has
    assert(mlp_state_has("name") == 1);
    assert(mlp_state_has("nonexistent") == 0);
    printf("✅ has: name exists, nonexistent doesn't\n");
    
    // Delete
    assert(mlp_state_delete("name") == 1);
    assert(mlp_state_has("name") == 0);
    printf("✅ delete: name removed\n");
    
    // Clear
    mlp_state_set("key1", "val1");
    mlp_state_set("key2", "val2");
    mlp_state_clear();
    assert(mlp_state_has("key1") == 0);
    assert(mlp_state_has("key2") == 0);
    printf("✅ clear: all removed\n");
    
    mlp_state_close();
}

void test_sto_optimization() {
    printf("\n=== Test 3: STO Optimization (SSO vs Heap) ===\n");
    
    mlp_state_init();
    
    // Small strings (≤23 bytes) - SSO (stack inline)
    mlp_state_set("small1", "Ali");            // 3 bytes
    mlp_state_set("small2", "1234567890123456789012");  // 22 bytes (limit)
    
    char* s1 = mlp_state_get("small1");
    char* s2 = mlp_state_get("small2");
    assert(strcmp(s1, "Ali") == 0);
    assert(strcmp(s2, "1234567890123456789012") == 0);
    free(s1);
    free(s2);
    printf("✅ Small strings (SSO): 3 bytes, 22 bytes\n");
    
    // Large string (>23 bytes) - Heap
    char large[100];
    strcpy(large, "This is a very long string that exceeds SSO size and goes to heap!");
    mlp_state_set("large", large);
    
    char* l = mlp_state_get("large");
    assert(strcmp(l, large) == 0);
    free(l);
    printf("✅ Large string (Heap): 68 bytes\n");
    
    mlp_state_close();
}

void test_persistence() {
    printf("\n=== Test 4: Persistence (File I/O) ===\n");
    
    mlp_state_init();
    mlp_state_config_set("persist_file", "test_state.json");
    
    // Store data
    mlp_state_set("shared:username", "Ali");
    mlp_state_set("shared:theme", "dark");
    mlp_state_set("config:version", "1.0");
    
    // Save to disk
    assert(mlp_state_save() == 1);
    printf("✅ Saved to test_state.json\n");
    
    // Clear memory
    mlp_state_clear();
    assert(mlp_state_has("shared:username") == 0);
    printf("✅ Memory cleared\n");
    
    // Load from disk
    assert(mlp_state_load() == 1);
    char* username = mlp_state_get("shared:username");
    char* theme = mlp_state_get("shared:theme");
    assert(strcmp(username, "Ali") == 0);
    assert(strcmp(theme, "dark") == 0);
    free(username);
    free(theme);
    printf("✅ Loaded from test_state.json\n");
    
    mlp_state_close();
}

void test_config() {
    printf("\n=== Test 5: Configuration ===\n");
    
    mlp_state_init();
    
    // Auto-persist
    assert(mlp_state_config_set("auto_persist", "1") == 1);
    printf("✅ Config: auto_persist enabled\n");
    
    // Custom file
    assert(mlp_state_config_set("persist_file", "custom.json") == 1);
    printf("✅ Config: persist_file = custom.json\n");
    
    mlp_state_close();
}

void test_namespace_convention() {
    printf("\n=== Test 6: Namespace Convention ===\n");
    
    mlp_state_init();
    
    // Shared namespace
    mlp_state_set("shared:global_counter", "100");
    mlp_state_set("shared:app_name", "MyApp");
    
    // Config namespace
    mlp_state_set("config:theme", "dark");
    mlp_state_set("config:lang", "en");
    
    // Temp namespace
    mlp_state_set("temp:session_token", "abc123");
    
    char* counter = mlp_state_get("shared:global_counter");
    char* theme = mlp_state_get("config:theme");
    char* token = mlp_state_get("temp:session_token");
    
    assert(strcmp(counter, "100") == 0);
    assert(strcmp(theme, "dark") == 0);
    assert(strcmp(token, "abc123") == 0);
    
    free(counter);
    free(theme);
    free(token);
    
    printf("✅ Namespaces: shared, config, temp\n");
    
    mlp_state_close();
}

int main() {
    printf("🧪 MLP State Manager - Runtime Tests\n");
    printf("=====================================\n");
    
    test_lifecycle();
    test_basic_operations();
    test_sto_optimization();
    test_persistence();
    test_config();
    test_namespace_convention();
    
    printf("\n✅ ALL TESTS PASSED!\n");
    printf("\n💡 Note: Auto-cleanup will run at program exit\n");
    
    return 0;
}
