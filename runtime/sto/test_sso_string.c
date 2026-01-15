// ============================================================================
// SSO String Test Program
// ============================================================================
// Comprehensive tests for Small String Optimization
//
// Tests:
// - SSO vs Heap storage (≤23 bytes vs >23 bytes)
// - String concatenation
// - String comparison and equality
// - Substring operations
// - String search (find)
// - Conversions (int64 ↔ string)
// - Prefix/suffix checks

#include "runtime_sto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test counters
static int tests_passed = 0;
static int tests_failed = 0;

// Helper: Assert string equals
void assert_sso_equals(SSOString* sso, const char* expected, const char* test_name) {
    const char* result = sto_sso_data(sso);
    
    if (result && strcmp(result, expected) == 0) {
        printf("✅ %s: '%s'\n", test_name, result);
        tests_passed++;
    } else {
        printf("❌ %s: Expected '%s', got '%s'\n", test_name, expected, result ? result : "NULL");
        tests_failed++;
    }
}

// Test 1: SSO storage (≤23 bytes)
void test_sso_storage() {
    printf("\n=== Test 1: SSO Storage (≤23 bytes) ===\n");
    
    // Very short string (SSO)
    SSOString* s1 = sto_sso_create("Hello");
    assert_sso_equals(s1, "Hello", "Short string");
    printf("   Flags: 0x%02x (bit 0 = %d, SSO)\n", s1->flags, s1->flags & 1);
    sto_sso_free(s1);
    
    // Exactly 23 bytes (max SSO)
    SSOString* s2 = sto_sso_create("12345678901234567890123");  // 23 chars
    assert_sso_equals(s2, "12345678901234567890123", "Exactly 23 bytes");
    printf("   Flags: 0x%02x (bit 0 = %d, SSO)\n", s2->flags, s2->flags & 1);
    sto_sso_free(s2);
    
    // 24 bytes (needs heap)
    SSOString* s3 = sto_sso_create("123456789012345678901234");  // 24 chars
    assert_sso_equals(s3, "123456789012345678901234", "24 bytes (heap)");
    printf("   Flags: 0x%02x (bit 0 = %d, Heap)\n", s3->flags, s3->flags & 1);
    sto_sso_free(s3);
}

// Test 2: String length
void test_string_length() {
    printf("\n=== Test 2: String Length ===\n");
    
    SSOString* s1 = sto_sso_create("Hello");
    size_t len1 = sto_sso_length(s1);
    printf("%s Length of 'Hello': %zu (expected 5)\n", len1 == 5 ? "✅" : "❌", len1);
    if (len1 == 5) tests_passed++; else tests_failed++;
    sto_sso_free(s1);
    
    SSOString* s2 = sto_sso_create("");
    size_t len2 = sto_sso_length(s2);
    printf("%s Length of '': %zu (expected 0)\n", len2 == 0 ? "✅" : "❌", len2);
    if (len2 == 0) tests_passed++; else tests_failed++;
    sto_sso_free(s2);
    
    SSOString* s3 = sto_sso_create("Very long string that exceeds SSO limit");
    size_t len3 = sto_sso_length(s3);
    printf("%s Length of long string: %zu (expected 39)\n", len3 == 39 ? "✅" : "❌", len3);
    if (len3 == 39) tests_passed++; else tests_failed++;
    sto_sso_free(s3);
}

// Test 3: String concatenation
void test_concatenation() {
    printf("\n=== Test 3: String Concatenation ===\n");
    
    // SSO + SSO = SSO
    SSOString* a = sto_sso_create("Hello");
    SSOString* b = sto_sso_create(" World");
    SSOString* result = sto_sso_concat(a, b);
    assert_sso_equals(result, "Hello World", "SSO + SSO");
    printf("   Result flags: 0x%02x (SSO = %d)\n", result->flags, (result->flags & 1) == 0);
    sto_sso_free(a);
    sto_sso_free(b);
    sto_sso_free(result);
    
    // SSO + SSO = Heap (exceeds 23 bytes)
    a = sto_sso_create("This is a longer");
    b = sto_sso_create(" string example");
    result = sto_sso_concat(a, b);
    assert_sso_equals(result, "This is a longer string example", "SSO + SSO → Heap");
    printf("   Result flags: 0x%02x (Heap = %d)\n", result->flags, (result->flags & 1) == 1);
    sto_sso_free(a);
    sto_sso_free(b);
    sto_sso_free(result);
}

// Test 4: String comparison
void test_comparison() {
    printf("\n=== Test 4: String Comparison ===\n");
    
    SSOString* s1 = sto_sso_create("apple");
    SSOString* s2 = sto_sso_create("banana");
    SSOString* s3 = sto_sso_create("apple");
    
    int cmp1 = sto_sso_compare(s1, s2);
    printf("%s 'apple' < 'banana': %d\n", cmp1 < 0 ? "✅" : "❌", cmp1);
    if (cmp1 < 0) tests_passed++; else tests_failed++;
    
    int cmp2 = sto_sso_compare(s2, s1);
    printf("%s 'banana' > 'apple': %d\n", cmp2 > 0 ? "✅" : "❌", cmp2);
    if (cmp2 > 0) tests_passed++; else tests_failed++;
    
    bool eq = sto_sso_equals(s1, s3);
    printf("%s 'apple' == 'apple': %d\n", eq ? "✅" : "❌", eq);
    if (eq) tests_passed++; else tests_failed++;
    
    sto_sso_free(s1);
    sto_sso_free(s2);
    sto_sso_free(s3);
}

// Test 5: Substring
void test_substring() {
    printf("\n=== Test 5: Substring ===\n");
    
    SSOString* str = sto_sso_create("Hello World");
    
    SSOString* sub1 = sto_sso_substring(str, 0, 5);
    assert_sso_equals(sub1, "Hello", "Substring [0:5]");
    sto_sso_free(sub1);
    
    SSOString* sub2 = sto_sso_substring(str, 6, 5);
    assert_sso_equals(sub2, "World", "Substring [6:11]");
    sto_sso_free(sub2);
    
    SSOString* sub3 = sto_sso_substring(str, 0, 100);  // Exceeds length
    assert_sso_equals(sub3, "Hello World", "Substring [0:100] (clamped)");
    sto_sso_free(sub3);
    
    sto_sso_free(str);
}

// Test 6: Integer conversions
void test_int_conversions() {
    printf("\n=== Test 6: Integer Conversions ===\n");
    
    SSOString* s1 = sto_sso_from_int64(42);
    assert_sso_equals(s1, "42", "int64_t 42 → string");
    sto_sso_free(s1);
    
    SSOString* s2 = sto_sso_from_int64(-123);
    assert_sso_equals(s2, "-123", "int64_t -123 → string");
    sto_sso_free(s2);
    
    SSOString* s3 = sto_sso_create("999");
    int64_t val = sto_sso_to_int64(s3);
    printf("%s String '999' → int64_t: %lld\n", val == 999 ? "✅" : "❌", (long long)val);
    if (val == 999) tests_passed++; else tests_failed++;
    sto_sso_free(s3);
}

// Test 7: String search
void test_search() {
    printf("\n=== Test 7: String Search ===\n");
    
    SSOString* str = sto_sso_create("Hello World, Hello Universe");
    
    int pos1 = sto_sso_find(str, "World");
    printf("%s Find 'World': %d (expected 6)\n", pos1 == 6 ? "✅" : "❌", pos1);
    if (pos1 == 6) tests_passed++; else tests_failed++;
    
    int pos2 = sto_sso_find(str, "Hello");
    printf("%s Find 'Hello' (first): %d (expected 0)\n", pos2 == 0 ? "✅" : "❌", pos2);
    if (pos2 == 0) tests_passed++; else tests_failed++;
    
    int pos3 = sto_sso_find(str, "NotFound");
    printf("%s Find 'NotFound': %d (expected -1)\n", pos3 == -1 ? "✅" : "❌", pos3);
    if (pos3 == -1) tests_passed++; else tests_failed++;
    
    sto_sso_free(str);
}

// Test 8: Prefix/Suffix checks
void test_prefix_suffix() {
    printf("\n=== Test 8: Prefix/Suffix Checks ===\n");
    
    SSOString* str = sto_sso_create("filename.txt");
    
    bool starts = sto_sso_starts_with(str, "file");
    printf("%s Starts with 'file': %d\n", starts ? "✅" : "❌", starts);
    if (starts) tests_passed++; else tests_failed++;
    
    bool ends = sto_sso_ends_with(str, ".txt");
    printf("%s Ends with '.txt': %d\n", ends ? "✅" : "❌", ends);
    if (ends) tests_passed++; else tests_failed++;
    
    bool not_starts = sto_sso_starts_with(str, "data");
    printf("%s Does NOT start with 'data': %d\n", !not_starts ? "✅" : "❌", not_starts);
    if (!not_starts) tests_passed++; else tests_failed++;
    
    sto_sso_free(str);
}

// Test 9: String copy
void test_copy() {
    printf("\n=== Test 9: String Copy ===\n");
    
    SSOString* original = sto_sso_create("Original");
    SSOString* copy = sto_sso_copy(original);
    
    assert_sso_equals(copy, "Original", "Copy equals original");
    
    // Verify they're independent (different memory)
    bool independent = (original != copy);
    printf("%s Copy is independent: %d\n", independent ? "✅" : "❌", independent);
    if (independent) tests_passed++; else tests_failed++;
    
    sto_sso_free(original);
    sto_sso_free(copy);
}

int main() {
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║       SSO String Test Suite - STO Runtime            ║\n");
    printf("║       Small String Optimization                      ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    
    test_sso_storage();
    test_string_length();
    test_concatenation();
    test_comparison();
    test_substring();
    test_int_conversions();
    test_search();
    test_prefix_suffix();
    test_copy();
    
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                   Test Results                        ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    printf("║  ✅ Passed: %3d                                       ║\n", tests_passed);
    printf("║  ❌ Failed: %3d                                       ║\n", tests_failed);
    printf("║  📊 Total:  %3d                                       ║\n", tests_passed + tests_failed);
    printf("╚═══════════════════════════════════════════════════════╝\n");
    
    return tests_failed == 0 ? 0 : 1;
}
