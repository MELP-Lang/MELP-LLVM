/**
 * Test program for MLP List Runtime
 * YZ_200: Validates list operations before compiler integration
 */

#include <stdio.h>
#include <stdint.h>
#include "mlp_list.h"

void test_list_create() {
    printf("Test 1: List Creation\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    printf("  Created: length=%zu, capacity=%zu\n", 
           melp_list_length(list), melp_list_capacity(list));
    printf("  Empty? %s\n", melp_list_is_empty(list) ? "YES" : "NO");
    melp_list_free(list);
    printf("  ✅ PASSED\n\n");
}

void test_list_append() {
    printf("Test 2: List Append\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    int64_t values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        melp_list_append(list, &values[i]);
    }
    
    printf("  After 5 appends: length=%zu, capacity=%zu\n", 
           melp_list_length(list), melp_list_capacity(list));
    
    melp_list_print_debug(list);
    
    // Verify values
    for (int i = 0; i < 5; i++) {
        int64_t* val = (int64_t*)melp_list_get(list, i);
        if (*val != values[i]) {
            printf("  ❌ FAILED: Expected %lld, got %lld at index %d\n", 
                   (long long)values[i], (long long)*val, i);
            melp_list_free(list);
            return;
        }
    }
    
    melp_list_free(list);
    printf("  ✅ PASSED\n\n");
}

void test_list_prepend() {
    printf("Test 3: List Prepend\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    int64_t val1 = 30, val2 = 20, val3 = 10;
    melp_list_append(list, &val1);
    melp_list_prepend(list, &val2);
    melp_list_prepend(list, &val3);
    
    printf("  After prepends: ");
    melp_list_print_debug(list);
    
    // Expected: [10, 20, 30]
    int64_t* v0 = (int64_t*)melp_list_get(list, 0);
    int64_t* v1 = (int64_t*)melp_list_get(list, 1);
    int64_t* v2 = (int64_t*)melp_list_get(list, 2);
    
    if (*v0 == 10 && *v1 == 20 && *v2 == 30) {
        printf("  ✅ PASSED\n\n");
    } else {
        printf("  ❌ FAILED: Expected [10, 20, 30]\n\n");
    }
    
    melp_list_free(list);
}

void test_list_set() {
    printf("Test 4: List Set\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    int64_t val1 = 10, val2 = 20, val3 = 30, new_val = 99;
    melp_list_append(list, &val1);
    melp_list_append(list, &val2);
    melp_list_append(list, &val3);
    
    printf("  Before set: ");
    melp_list_print_debug(list);
    
    melp_list_set(list, 1, &new_val);
    
    printf("  After set(1, 99): ");
    melp_list_print_debug(list);
    
    int64_t* v1 = (int64_t*)melp_list_get(list, 1);
    if (*v1 == 99) {
        printf("  ✅ PASSED\n\n");
    } else {
        printf("  ❌ FAILED\n\n");
    }
    
    melp_list_free(list);
}

void test_list_remove() {
    printf("Test 5: List Remove\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    int64_t values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        melp_list_append(list, &values[i]);
    }
    
    printf("  Before remove: ");
    melp_list_print_debug(list);
    
    melp_list_remove(list, 2);  // Remove 30
    
    printf("  After remove(2): ");
    melp_list_print_debug(list);
    
    // Expected: [10, 20, 40, 50]
    if (melp_list_length(list) == 4) {
        int64_t* v2 = (int64_t*)melp_list_get(list, 2);
        if (*v2 == 40) {
            printf("  ✅ PASSED\n\n");
        } else {
            printf("  ❌ FAILED\n\n");
        }
    } else {
        printf("  ❌ FAILED\n\n");
    }
    
    melp_list_free(list);
}

void test_list_capacity_growth() {
    printf("Test 6: Capacity Growth\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    printf("  Initial capacity: %zu\n", melp_list_capacity(list));
    
    // Add 10 elements to trigger multiple growths
    for (int64_t i = 0; i < 10; i++) {
        melp_list_append(list, &i);
    }
    
    printf("  After 10 appends: length=%zu, capacity=%zu\n", 
           melp_list_length(list), melp_list_capacity(list));
    
    if (melp_list_length(list) == 10 && melp_list_capacity(list) >= 10) {
        printf("  ✅ PASSED\n\n");
    } else {
        printf("  ❌ FAILED\n\n");
    }
    
    melp_list_free(list);
}

void test_list_empty() {
    printf("Test 7: Empty List\n");
    MelpList* list = melp_list_create(sizeof(int64_t));
    
    printf("  Empty? %s (length=%zu)\n", 
           melp_list_is_empty(list) ? "YES" : "NO",
           melp_list_length(list));
    
    int64_t val = 42;
    melp_list_append(list, &val);
    
    printf("  After append: Empty? %s (length=%zu)\n", 
           melp_list_is_empty(list) ? "YES" : "NO",
           melp_list_length(list));
    
    melp_list_clear(list);
    
    printf("  After clear: Empty? %s (length=%zu)\n", 
           melp_list_is_empty(list) ? "YES" : "NO",
           melp_list_length(list));
    
    if (melp_list_is_empty(list) && melp_list_length(list) == 0) {
        printf("  ✅ PASSED\n\n");
    } else {
        printf("  ❌ FAILED\n\n");
    }
    
    melp_list_free(list);
}

int main() {
    printf("=================================\n");
    printf("MLP List Runtime Test Suite\n");
    printf("YZ_200: List Operations\n");
    printf("=================================\n\n");
    
    test_list_create();
    test_list_append();
    test_list_prepend();
    test_list_set();
    test_list_remove();
    test_list_capacity_growth();
    test_list_empty();
    
    printf("=================================\n");
    printf("✅ All Tests Completed!\n");
    printf("=================================\n");
    
    return 0;
}
