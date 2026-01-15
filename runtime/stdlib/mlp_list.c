/**
 * MLP Standard Library - List (Dynamic Array) Operations Implementation
 * 
 * STO-compliant list operations for MELP compiler
 * YZ_200: Full list implementation with runtime allocation
 * 
 * Created: 21 Aralık 2025 (YZ_200)
 */

#include "mlp_list.h"
#include "mlp_panic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initial capacity for new lists
#define INITIAL_CAPACITY 4

// Growth factor (capacity doubles each time)
#define GROWTH_FACTOR 2

// -----------------------------------------------------------------------------
// Core List Operations
// -----------------------------------------------------------------------------

MelpList* melp_list_create(size_t element_size) {
    if (element_size == 0) {
        return NULL;
    }
    
    MelpList* list = (MelpList*)malloc(sizeof(MelpList));
    if (!list) {
        return NULL;
    }
    
    list->elements = (void**)malloc(sizeof(void*) * INITIAL_CAPACITY);
    if (!list->elements) {
        free(list);
        return NULL;
    }
    
    list->length = 0;
    list->capacity = INITIAL_CAPACITY;
    list->element_size = element_size;
    
    return list;
}

void melp_list_free(MelpList* list) {
    if (!list) {
        return;
    }
    
    // Free each element
    for (size_t i = 0; i < list->length; i++) {
        if (list->elements[i]) {
            free(list->elements[i]);
        }
    }
    
    // Free the element array
    free(list->elements);
    
    // Free the list structure
    free(list);
}

size_t melp_list_length(MelpList* list) {
    if (!list) {
        return 0;
    }
    return list->length;
}

int melp_list_is_empty(MelpList* list) {
    return (!list || list->length == 0) ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Element Access
// -----------------------------------------------------------------------------

void* melp_list_get(MelpList* list, size_t index) {
    if (!list || index >= list->length) {
        // Runtime bounds check failure
        mlp_runtime_error("List index out of bounds");
        return NULL;
    }
    
    return list->elements[index];
}

int melp_list_set(MelpList* list, size_t index, void* element) {
    if (!list || index >= list->length) {
        return -1;  // Index out of bounds
    }
    
    if (!element) {
        return -1;  // Null element
    }
    
    // Free old element
    if (list->elements[index]) {
        free(list->elements[index]);
    }
    
    // Allocate and copy new element
    list->elements[index] = malloc(list->element_size);
    if (!list->elements[index]) {
        return -1;  // Memory allocation failure
    }
    
    memcpy(list->elements[index], element, list->element_size);
    
    return 0;
}

// -----------------------------------------------------------------------------
// Internal Helper: Grow capacity
// -----------------------------------------------------------------------------

static int melp_list_grow(MelpList* list) {
    size_t new_capacity = list->capacity * GROWTH_FACTOR;
    
    void** new_elements = (void**)realloc(list->elements, sizeof(void*) * new_capacity);
    if (!new_elements) {
        return -1;  // Memory allocation failure
    }
    
    list->elements = new_elements;
    list->capacity = new_capacity;
    
    return 0;
}

// -----------------------------------------------------------------------------
// Modification Operations
// -----------------------------------------------------------------------------

int melp_list_append(MelpList* list, void* element) {
    if (!list || !element) {
        return -1;
    }
    
    // Grow if needed
    if (list->length >= list->capacity) {
        if (melp_list_grow(list) != 0) {
            return -1;
        }
    }
    
    // Allocate and copy element
    list->elements[list->length] = malloc(list->element_size);
    if (!list->elements[list->length]) {
        return -1;
    }
    
    memcpy(list->elements[list->length], element, list->element_size);
    list->length++;
    
    return 0;
}

int melp_list_prepend(MelpList* list, void* element) {
    if (!list || !element) {
        return -1;
    }
    
    // Grow if needed
    if (list->length >= list->capacity) {
        if (melp_list_grow(list) != 0) {
            return -1;
        }
    }
    
    // Shift all elements right by one
    for (size_t i = list->length; i > 0; i--) {
        list->elements[i] = list->elements[i - 1];
    }
    
    // Allocate and copy new element at index 0
    list->elements[0] = malloc(list->element_size);
    if (!list->elements[0]) {
        return -1;
    }
    
    memcpy(list->elements[0], element, list->element_size);
    list->length++;
    
    return 0;
}

int melp_list_remove(MelpList* list, size_t index) {
    if (!list || index >= list->length) {
        return -1;  // Index out of bounds
    }
    
    // Free the element
    if (list->elements[index]) {
        free(list->elements[index]);
    }
    
    // Shift all elements left by one
    for (size_t i = index; i < list->length - 1; i++) {
        list->elements[i] = list->elements[i + 1];
    }
    
    list->length--;
    
    return 0;
}

void melp_list_clear(MelpList* list) {
    if (!list) {
        return;
    }
    
    // Free all elements
    for (size_t i = 0; i < list->length; i++) {
        if (list->elements[i]) {
            free(list->elements[i]);
            list->elements[i] = NULL;
        }
    }
    
    list->length = 0;
}

// -----------------------------------------------------------------------------
// Utility Operations
// -----------------------------------------------------------------------------

MelpList* melp_list_clone(MelpList* list) {
    if (!list) {
        return NULL;
    }
    
    MelpList* new_list = melp_list_create(list->element_size);
    if (!new_list) {
        return NULL;
    }
    
    // Reserve capacity to match original
    if (melp_list_reserve(new_list, list->length) != 0) {
        melp_list_free(new_list);
        return NULL;
    }
    
    // Copy all elements
    for (size_t i = 0; i < list->length; i++) {
        if (melp_list_append(new_list, list->elements[i]) != 0) {
            melp_list_free(new_list);
            return NULL;
        }
    }
    
    return new_list;
}

void melp_list_reverse(MelpList* list) {
    if (!list || list->length <= 1) {
        return;
    }
    
    size_t left = 0;
    size_t right = list->length - 1;
    
    while (left < right) {
        // Swap elements
        void* temp = list->elements[left];
        list->elements[left] = list->elements[right];
        list->elements[right] = temp;
        
        left++;
        right--;
    }
}

size_t melp_list_capacity(MelpList* list) {
    if (!list) {
        return 0;
    }
    return list->capacity;
}

int melp_list_reserve(MelpList* list, size_t new_capacity) {
    if (!list || new_capacity <= list->capacity) {
        return 0;  // No need to grow
    }
    
    void** new_elements = (void**)realloc(list->elements, sizeof(void*) * new_capacity);
    if (!new_elements) {
        return -1;
    }
    
    list->elements = new_elements;
    list->capacity = new_capacity;
    
    return 0;
}

// -----------------------------------------------------------------------------
// Debug & Introspection
// -----------------------------------------------------------------------------

void melp_list_print_debug(MelpList* list) {
    if (!list) {
        printf("List: NULL\n");
        return;
    }
    
    printf("List[%zu/%zu]: [", list->length, list->capacity);
    
    for (size_t i = 0; i < list->length; i++) {
        if (list->element_size == sizeof(int64_t)) {
            int64_t* val = (int64_t*)list->elements[i];
            printf("%lld", (long long)*val);
        } else {
            printf("%p", list->elements[i]);
        }
        
        if (i < list->length - 1) {
            printf(", ");
        }
    }
    
    printf("]\n");
}
