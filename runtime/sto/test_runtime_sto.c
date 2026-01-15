#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "runtime_sto.h"

// Test colors
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void test_overflow_detection() {
    printf("\n=== Testing Overflow Detection ===\n");
    
    // Test 1: Normal addition (no overflow)
    printf("Test 1: 100 + 200 (no overflow)... ");
    assert(!sto_runtime_add_will_overflow(100, 200));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 2: Positive overflow
    printf("Test 2: INT64_MAX + 1 (overflow)... ");
    assert(sto_runtime_add_will_overflow(INT64_MAX, 1));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 3: Negative overflow
    printf("Test 3: INT64_MIN + (-1) (overflow)... ");
    assert(sto_runtime_add_will_overflow(INT64_MIN, -1));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 4: Large positive numbers
    printf("Test 4: INT64_MAX + INT64_MAX (overflow)... ");
    assert(sto_runtime_add_will_overflow(INT64_MAX, INT64_MAX));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 5: Normal subtraction
    printf("Test 5: 200 - 100 (no overflow)... ");
    assert(!sto_runtime_sub_will_overflow(200, 100));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 6: Subtraction overflow
    printf("Test 6: INT64_MIN - 1 (overflow)... ");
    assert(sto_runtime_sub_will_overflow(INT64_MIN, 1));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 7: Normal multiplication
    printf("Test 7: 100 * 200 (no overflow)... ");
    assert(!sto_runtime_mul_will_overflow(100, 200));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 8: Multiplication overflow
    printf("Test 8: INT64_MAX * 2 (overflow)... ");
    assert(sto_runtime_mul_will_overflow(INT64_MAX, 2));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 9: Multiply by zero
    printf("Test 9: INT64_MAX * 0 (no overflow)... ");
    assert(!sto_runtime_mul_will_overflow(INT64_MAX, 0));
    printf(GREEN "PASS" RESET "\n");
    
    // Test 10: Multiply by one
    printf("Test 10: INT64_MAX * 1 (no overflow)... ");
    assert(!sto_runtime_mul_will_overflow(INT64_MAX, 1));
    printf(GREEN "PASS" RESET "\n");
}

void test_safe_operations() {
    printf("\n=== Testing Safe Operations ===\n");
    
    int64_t result;
    bool overflow;
    
    // Test 1: Safe add (no overflow)
    printf("Test 1: safe_add(100, 200)... ");
    overflow = sto_runtime_safe_add(100, 200, &result);
    assert(!overflow && result == 300);
    printf(GREEN "PASS" RESET " (result: %lld)\n", (long long)result);
    
    // Test 2: Safe add (overflow)
    printf("Test 2: safe_add(INT64_MAX, 1)... ");
    overflow = sto_runtime_safe_add(INT64_MAX, 1, &result);
    assert(overflow);
    printf(GREEN "PASS" RESET " (overflow detected)\n");
    
    // Test 3: Safe sub (no overflow)
    printf("Test 3: safe_sub(200, 100)... ");
    overflow = sto_runtime_safe_sub(200, 100, &result);
    assert(!overflow && result == 100);
    printf(GREEN "PASS" RESET " (result: %lld)\n", (long long)result);
    
    // Test 4: Safe sub (overflow)
    printf("Test 4: safe_sub(INT64_MIN, 1)... ");
    overflow = sto_runtime_safe_sub(INT64_MIN, 1, &result);
    assert(overflow);
    printf(GREEN "PASS" RESET " (overflow detected)\n");
    
    // Test 5: Safe mul (no overflow)
    printf("Test 5: safe_mul(100, 200)... ");
    overflow = sto_runtime_safe_mul(100, 200, &result);
    assert(!overflow && result == 20000);
    printf(GREEN "PASS" RESET " (result: %lld)\n", (long long)result);
    
    // Test 6: Safe mul (overflow)
    printf("Test 6: safe_mul(INT64_MAX, 2)... ");
    overflow = sto_runtime_safe_mul(INT64_MAX, 2, &result);
    assert(overflow);
    printf(GREEN "PASS" RESET " (overflow detected)\n");
}

void test_bigdecimal_basic() {
    printf("\n=== Testing BigDecimal Basics ===\n");
    
    // Test 1: Create from int64
    printf("Test 1: Create BigDecimal from 12345... ");
    BigDecimal* bd1 = sto_bigdec_from_int64(12345);
    assert(bd1 != NULL);
    char* str1 = sto_bigdec_to_string(bd1);
    printf(GREEN "PASS" RESET " (value: %s)\n", str1);
    free(str1);
    sto_bigdec_free(bd1);
    
    // Test 2: Create from negative int64
    printf("Test 2: Create BigDecimal from -67890... ");
    BigDecimal* bd2 = sto_bigdec_from_int64(-67890);
    assert(bd2 != NULL);
    char* str2 = sto_bigdec_to_string(bd2);
    printf(GREEN "PASS" RESET " (value: %s)\n", str2);
    free(str2);
    sto_bigdec_free(bd2);
    
    // Test 3: Create from string
    printf("Test 3: Create BigDecimal from string \"999999999999999999\"... ");
    BigDecimal* bd3 = sto_bigdec_from_string("999999999999999999");
    assert(bd3 != NULL);
    char* str3 = sto_bigdec_to_string(bd3);
    printf(GREEN "PASS" RESET " (value: %s)\n", str3);
    free(str3);
    sto_bigdec_free(bd3);
}

void test_bigdecimal_arithmetic() {
    printf("\n=== Testing BigDecimal Arithmetic ===\n");
    
    // Test 1: Addition
    printf("Test 1: 100 + 200 = 300... ");
    BigDecimal* a1 = sto_bigdec_from_int64(100);
    BigDecimal* b1 = sto_bigdec_from_int64(200);
    BigDecimal* r1 = sto_bigdec_add(a1, b1);
    char* s1 = sto_bigdec_to_string(r1);
    assert(strcmp(s1, "300") == 0);
    printf(GREEN "PASS" RESET " (result: %s)\n", s1);
    free(s1);
    sto_bigdec_free(a1);
    sto_bigdec_free(b1);
    sto_bigdec_free(r1);
    
    // Test 2: Subtraction
    printf("Test 2: 500 - 200 = 300... ");
    BigDecimal* a2 = sto_bigdec_from_int64(500);
    BigDecimal* b2 = sto_bigdec_from_int64(200);
    BigDecimal* r2 = sto_bigdec_sub(a2, b2);
    char* s2 = sto_bigdec_to_string(r2);
    assert(strcmp(s2, "300") == 0);
    printf(GREEN "PASS" RESET " (result: %s)\n", s2);
    free(s2);
    sto_bigdec_free(a2);
    sto_bigdec_free(b2);
    sto_bigdec_free(r2);
    
    // Test 3: Multiplication
    printf("Test 3: 25 * 4 = 100... ");
    BigDecimal* a3 = sto_bigdec_from_int64(25);
    BigDecimal* b3 = sto_bigdec_from_int64(4);
    BigDecimal* r3 = sto_bigdec_mul(a3, b3);
    char* s3 = sto_bigdec_to_string(r3);
    assert(strcmp(s3, "100") == 0);
    printf(GREEN "PASS" RESET " (result: %s)\n", s3);
    free(s3);
    sto_bigdec_free(a3);
    sto_bigdec_free(b3);
    sto_bigdec_free(r3);
    
    // Test 4: Division
    printf("Test 4: 100 / 4 = 25... ");
    BigDecimal* a4 = sto_bigdec_from_int64(100);
    BigDecimal* b4 = sto_bigdec_from_int64(4);
    BigDecimal* r4 = sto_bigdec_div(a4, b4);
    char* s4 = sto_bigdec_to_string(r4);
    assert(strcmp(s4, "25") == 0);
    printf(GREEN "PASS" RESET " (result: %s)\n", s4);
    free(s4);
    sto_bigdec_free(a4);
    sto_bigdec_free(b4);
    sto_bigdec_free(r4);
    
    // Test 5: Negative numbers
    printf("Test 5: (-50) + 30 = (-20)... ");
    BigDecimal* a5 = sto_bigdec_from_int64(-50);
    BigDecimal* b5 = sto_bigdec_from_int64(30);
    BigDecimal* r5 = sto_bigdec_add(a5, b5);
    char* s5 = sto_bigdec_to_string(r5);
    assert(strcmp(s5, "-20") == 0);
    printf(GREEN "PASS" RESET " (result: %s)\n", s5);
    free(s5);
    sto_bigdec_free(a5);
    sto_bigdec_free(b5);
    sto_bigdec_free(r5);
}

void test_bigdecimal_compare() {
    printf("\n=== Testing BigDecimal Comparison ===\n");
    
    // Test 1: Equal
    printf("Test 1: 100 == 100... ");
    BigDecimal* a1 = sto_bigdec_from_int64(100);
    BigDecimal* b1 = sto_bigdec_from_int64(100);
    assert(sto_bigdec_compare(a1, b1) == 0);
    printf(GREEN "PASS" RESET "\n");
    sto_bigdec_free(a1);
    sto_bigdec_free(b1);
    
    // Test 2: Less than
    printf("Test 2: 50 < 100... ");
    BigDecimal* a2 = sto_bigdec_from_int64(50);
    BigDecimal* b2 = sto_bigdec_from_int64(100);
    assert(sto_bigdec_compare(a2, b2) == -1);
    printf(GREEN "PASS" RESET "\n");
    sto_bigdec_free(a2);
    sto_bigdec_free(b2);
    
    // Test 3: Greater than
    printf("Test 3: 200 > 100... ");
    BigDecimal* a3 = sto_bigdec_from_int64(200);
    BigDecimal* b3 = sto_bigdec_from_int64(100);
    assert(sto_bigdec_compare(a3, b3) == 1);
    printf(GREEN "PASS" RESET "\n");
    sto_bigdec_free(a3);
    sto_bigdec_free(b3);
    
    // Test 4: Negative comparison
    printf("Test 4: (-100) < 50... ");
    BigDecimal* a4 = sto_bigdec_from_int64(-100);
    BigDecimal* b4 = sto_bigdec_from_int64(50);
    assert(sto_bigdec_compare(a4, b4) == -1);
    printf(GREEN "PASS" RESET "\n");
    sto_bigdec_free(a4);
    sto_bigdec_free(b4);
    
    // Test 5: Both negative
    printf("Test 5: (-50) > (-100)... ");
    BigDecimal* a5 = sto_bigdec_from_int64(-50);
    BigDecimal* b5 = sto_bigdec_from_int64(-100);
    assert(sto_bigdec_compare(a5, b5) == 1);
    printf(GREEN "PASS" RESET "\n");
    sto_bigdec_free(a5);
    sto_bigdec_free(b5);
}

void test_sso_string() {
    printf("\n=== Testing SSO String ===\n");
    
    // Test 1: Short string (SSO)
    printf("Test 1: Create SSO string \"Hello\"... ");
    SSOString* sso1 = sto_sso_create("Hello");
    assert(sso1 != NULL);
    const char* data1 = sto_sso_data(sso1);
    assert(strcmp(data1, "Hello") == 0);
    printf(GREEN "PASS" RESET " (stored inline, flags: %d)\n", sso1->flags);
    sto_sso_free(sso1);
    
    // Test 2: Long string (heap)
    printf("Test 2: Create SSO string (long, 50 chars)... ");
    const char* long_str = "This is a very long string that exceeds 23 bytes!";
    SSOString* sso2 = sto_sso_create(long_str);
    assert(sso2 != NULL);
    const char* data2 = sto_sso_data(sso2);
    assert(strcmp(data2, long_str) == 0);
    printf(GREEN "PASS" RESET " (stored on heap, flags: %d)\n", sso2->flags);
    sto_sso_free(sso2);
    
    // Test 3: Edge case - exactly 23 bytes
    printf("Test 3: Create SSO string (exactly 23 bytes)... ");
    const char* edge_str = "12345678901234567890123";  // 23 chars
    SSOString* sso3 = sto_sso_create(edge_str);
    assert(sso3 != NULL);
    const char* data3 = sto_sso_data(sso3);
    assert(strcmp(data3, edge_str) == 0);
    printf(GREEN "PASS" RESET " (stored inline, flags: %d)\n", sso3->flags);
    sto_sso_free(sso3);
}

void test_edge_cases() {
    printf("\n=== Testing Edge Cases ===\n");
    
    int64_t result;
    
    // Test 1: INT64_MAX edge
    printf("Test 1: INT64_MAX = %lld... ", (long long)INT64_MAX);
    bool overflow = sto_runtime_safe_add(INT64_MAX, 0, &result);
    assert(!overflow && result == INT64_MAX);
    printf(GREEN "PASS" RESET "\n");
    
    // Test 2: INT64_MIN edge
    printf("Test 2: INT64_MIN = %lld... ", (long long)INT64_MIN);
    overflow = sto_runtime_safe_add(INT64_MIN, 0, &result);
    assert(!overflow && result == INT64_MIN);
    printf(GREEN "PASS" RESET "\n");
    
    // Test 3: Zero operations
    printf("Test 3: 0 + 0 = 0... ");
    overflow = sto_runtime_safe_add(0, 0, &result);
    assert(!overflow && result == 0);
    printf(GREEN "PASS" RESET "\n");
    
    // Test 4: Large multiplication
    printf("Test 4: 1000000 * 1000000... ");
    overflow = sto_runtime_safe_mul(1000000LL, 1000000LL, &result);
    assert(!overflow && result == 1000000000000LL);
    printf(GREEN "PASS" RESET " (result: %lld)\n", (long long)result);
    
    // Test 5: Negative multiplication
    printf("Test 5: (-1000) * 2000... ");
    overflow = sto_runtime_safe_mul(-1000, 2000, &result);
    assert(!overflow && result == -2000000);
    printf(GREEN "PASS" RESET " (result: %lld)\n", (long long)result);
}

int main() {
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║   STO Runtime - Phase 3.1 Test Suite            ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    test_overflow_detection();
    test_safe_operations();
    test_bigdecimal_basic();
    test_bigdecimal_arithmetic();
    test_bigdecimal_compare();
    test_sso_string();
    test_edge_cases();
    
    printf("\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║              " GREEN "ALL TESTS PASSED!" RESET "                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    
    return 0;
}
