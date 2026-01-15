// ============================================================================
// BigDecimal Test Program
// ============================================================================
// Comprehensive tests for arbitrary precision arithmetic
//
// Tests:
// - Creation from int64 and string
// - Addition, subtraction, multiplication
// - Large number handling (> int64 range)
// - Edge cases (zero, negative, overflow scenarios)

#include "runtime_sto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

// Helper: Assert BigDecimal value
void assert_bigdec_equals(BigDecimal* bd, const char* expected, const char* test_name) {
    char* result = sto_bigdec_to_string(bd);
    
    if (strcmp(result, expected) == 0) {
        printf("✅ %s: %s\n", test_name, result);
        tests_passed++;
    } else {
        printf("❌ %s: Expected %s, got %s\n", test_name, expected, result);
        tests_failed++;
    }
    
    free(result);
}

// Test 1: Creation from int64
void test_creation_from_int64() {
    printf("\n=== Test 1: Creation from int64 ===\n");
    
    BigDecimal* bd1 = sto_bigdec_from_int64(42);
    assert_bigdec_equals(bd1, "42", "Create 42");
    sto_bigdec_free(bd1);
    
    BigDecimal* bd2 = sto_bigdec_from_int64(-123);
    assert_bigdec_equals(bd2, "-123", "Create -123");
    sto_bigdec_free(bd2);
    
    BigDecimal* bd3 = sto_bigdec_from_int64(0);
    assert_bigdec_equals(bd3, "0", "Create 0");
    sto_bigdec_free(bd3);
    
    BigDecimal* bd4 = sto_bigdec_from_int64(9223372036854775807LL);  // INT64_MAX
    assert_bigdec_equals(bd4, "9223372036854775807", "Create INT64_MAX");
    sto_bigdec_free(bd4);
}

// Test 2: Creation from string
void test_creation_from_string() {
    printf("\n=== Test 2: Creation from string ===\n");
    
    BigDecimal* bd1 = sto_bigdec_from_string("12345678901234567890");
    assert_bigdec_equals(bd1, "12345678901234567890", "Create large number");
    sto_bigdec_free(bd1);
    
    BigDecimal* bd2 = sto_bigdec_from_string("-987654321");
    assert_bigdec_equals(bd2, "-987654321", "Create negative");
    sto_bigdec_free(bd2);
}

// Test 3: Addition
void test_addition() {
    printf("\n=== Test 3: Addition ===\n");
    
    // Positive + Positive
    BigDecimal* a = sto_bigdec_from_int64(123);
    BigDecimal* b = sto_bigdec_from_int64(456);
    BigDecimal* result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "579", "123 + 456");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Negative + Negative
    a = sto_bigdec_from_int64(-100);
    b = sto_bigdec_from_int64(-50);
    result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "-150", "-100 + -50");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Positive + Negative (larger positive)
    a = sto_bigdec_from_int64(100);
    b = sto_bigdec_from_int64(-30);
    result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "70", "100 + -30");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Positive + Negative (larger negative)
    a = sto_bigdec_from_int64(30);
    b = sto_bigdec_from_int64(-100);
    result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "-70", "30 + -100");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Same magnitude, opposite signs
    a = sto_bigdec_from_int64(50);
    b = sto_bigdec_from_int64(-50);
    result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "0", "50 + -50");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Large number addition
    a = sto_bigdec_from_string("999999999999999999");
    b = sto_bigdec_from_string("1");
    result = sto_bigdec_add(a, b);
    assert_bigdec_equals(result, "1000000000000000000", "Large: 999...999 + 1");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
}

// Test 4: Subtraction
void test_subtraction() {
    printf("\n=== Test 4: Subtraction ===\n");
    
    // Positive - Positive (larger - smaller)
    BigDecimal* a = sto_bigdec_from_int64(100);
    BigDecimal* b = sto_bigdec_from_int64(30);
    BigDecimal* result = sto_bigdec_sub(a, b);
    assert_bigdec_equals(result, "70", "100 - 30");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Positive - Positive (smaller - larger)
    a = sto_bigdec_from_int64(30);
    b = sto_bigdec_from_int64(100);
    result = sto_bigdec_sub(a, b);
    assert_bigdec_equals(result, "-70", "30 - 100");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Positive - Negative
    a = sto_bigdec_from_int64(50);
    b = sto_bigdec_from_int64(-20);
    result = sto_bigdec_sub(a, b);
    assert_bigdec_equals(result, "70", "50 - -20");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Same values
    a = sto_bigdec_from_int64(42);
    b = sto_bigdec_from_int64(42);
    result = sto_bigdec_sub(a, b);
    assert_bigdec_equals(result, "0", "42 - 42");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
}

// Test 5: Multiplication
void test_multiplication() {
    printf("\n=== Test 5: Multiplication ===\n");
    
    // Positive * Positive
    BigDecimal* a = sto_bigdec_from_int64(12);
    BigDecimal* b = sto_bigdec_from_int64(34);
    BigDecimal* result = sto_bigdec_mul(a, b);
    assert_bigdec_equals(result, "408", "12 * 34");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Positive * Negative
    a = sto_bigdec_from_int64(15);
    b = sto_bigdec_from_int64(-3);
    result = sto_bigdec_mul(a, b);
    assert_bigdec_equals(result, "-45", "15 * -3");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Negative * Negative
    a = sto_bigdec_from_int64(-7);
    b = sto_bigdec_from_int64(-8);
    result = sto_bigdec_mul(a, b);
    assert_bigdec_equals(result, "56", "-7 * -8");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Multiply by zero
    a = sto_bigdec_from_int64(999);
    b = sto_bigdec_from_int64(0);
    result = sto_bigdec_mul(a, b);
    assert_bigdec_equals(result, "0", "999 * 0");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
    
    // Large multiplication
    a = sto_bigdec_from_string("123456789");
    b = sto_bigdec_from_string("987654321");
    result = sto_bigdec_mul(a, b);
    assert_bigdec_equals(result, "121932631112635269", "Large: 123456789 * 987654321");
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    sto_bigdec_free(result);
}

// Test 6: Comparison
void test_comparison() {
    printf("\n=== Test 6: Comparison ===\n");
    
    BigDecimal* a = sto_bigdec_from_int64(100);
    BigDecimal* b = sto_bigdec_from_int64(50);
    
    int cmp = sto_bigdec_compare(a, b);
    printf("%s 100 > 50: %d\n", cmp > 0 ? "✅" : "❌", cmp);
    if (cmp > 0) tests_passed++; else tests_failed++;
    
    cmp = sto_bigdec_compare(b, a);
    printf("%s 50 < 100: %d\n", cmp < 0 ? "✅" : "❌", cmp);
    if (cmp < 0) tests_passed++; else tests_failed++;
    
    cmp = sto_bigdec_compare(a, a);
    printf("%s 100 == 100: %d\n", cmp == 0 ? "✅" : "❌", cmp);
    if (cmp == 0) tests_passed++; else tests_failed++;
    
    sto_bigdec_free(a);
    sto_bigdec_free(b);
    
    // Negative comparisons
    a = sto_bigdec_from_int64(-50);
    b = sto_bigdec_from_int64(-100);
    
    cmp = sto_bigdec_compare(a, b);
    printf("%s -50 > -100: %d\n", cmp > 0 ? "✅" : "❌", cmp);
    if (cmp > 0) tests_passed++; else tests_failed++;
    
    sto_bigdec_free(a);
    sto_bigdec_free(b);
}

// Test 7: Overflow scenario
void test_overflow_scenario() {
    printf("\n=== Test 7: INT64 Overflow → BigDecimal ===\n");
    
    // INT64_MAX + 1 should work in BigDecimal
    BigDecimal* max_int = sto_bigdec_from_int64(9223372036854775807LL);
    BigDecimal* one = sto_bigdec_from_int64(1);
    BigDecimal* result = sto_bigdec_add(max_int, one);
    
    assert_bigdec_equals(result, "9223372036854775808", "INT64_MAX + 1");
    
    sto_bigdec_free(max_int);
    sto_bigdec_free(one);
    sto_bigdec_free(result);
}

int main() {
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║       BigDecimal Test Suite - STO Runtime            ║\n");
    printf("║       Arbitrary Precision Arithmetic                 ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    
    test_creation_from_int64();
    test_creation_from_string();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_comparison();
    test_overflow_scenario();
    
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                   Test Results                        ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    printf("║  ✅ Passed: %3d                                       ║\n", tests_passed);
    printf("║  ❌ Failed: %3d                                       ║\n", tests_failed);
    printf("║  📊 Total:  %3d                                       ║\n", tests_passed + tests_failed);
    printf("╚═══════════════════════════════════════════════════════╝\n");
    
    return tests_failed == 0 ? 0 : 1;
}
