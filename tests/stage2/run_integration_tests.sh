#!/bin/bash
# MELP Stage 2 Bootstrap - Integration Test Suite
# Phase: 6.0 - Integration & Bootstrap Test
# Date: 15 Ocak 2026

set -e

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuration
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
COMPILER="$PROJECT_ROOT/compiler/stage2/stage2_bootstrap"
TEST_DIR="$SCRIPT_DIR/phase6"
TEMP_DIR="/tmp/melp_stage2_tests"

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Create temp directory
mkdir -p "$TEMP_DIR"
mkdir -p "$TEST_DIR"

echo "======================================"
echo "MELP Stage 2 Bootstrap - Integration Tests"
echo "Phase 6.0"
echo "======================================"
echo ""

# Helper function to run a test
run_test() {
    local test_name="$1"
    local input_file="$2"
    local should_pass="$3"  # "pass" or "fail"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Test $TOTAL_TESTS: $test_name ... "
    
    local output_file="$TEMP_DIR/test_$TOTAL_TESTS.ll"
    local error_file="$TEMP_DIR/test_$TOTAL_TESTS.err"
    
    if $COMPILER "$input_file" -o "$output_file" > "$error_file" 2>&1; then
        # Compilation succeeded
        if [ "$should_pass" = "pass" ]; then
            echo -e "${GREEN}✓ PASS${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗ FAIL (Expected error but succeeded)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        # Compilation failed
        if [ "$should_pass" = "fail" ]; then
            echo -e "${GREEN}✓ PASS (Expected error)${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗ FAIL${NC}"
            echo "  Error: $(cat $error_file | head -1)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
}

# ============================================================================
# Test Suite: Basic Features
# ============================================================================

echo "=== Category 1: Basic Features ==="

# Test 1: Simple return
cat > "$TEST_DIR/01_simple_return.mlp" << 'EOF'
function main() as numeric
    return 42
end_function
EOF
run_test "Simple return" "$TEST_DIR/01_simple_return.mlp" "pass"

# Test 2: Variable declaration
cat > "$TEST_DIR/02_variable.mlp" << 'EOF'
function main() as numeric
    numeric x = 10
    return x
end_function
EOF
run_test "Variable declaration" "$TEST_DIR/02_variable.mlp" "pass"

# Test 3: Arithmetic expression
cat > "$TEST_DIR/03_arithmetic.mlp" << 'EOF'
function main() as numeric
    numeric x = 10 + 20
    return x
end_function
EOF
run_test "Arithmetic expression" "$TEST_DIR/03_arithmetic.mlp" "pass"

# Test 4: If statement
cat > "$TEST_DIR/04_if_statement.mlp" << 'EOF'
function main() as numeric
    numeric x = 10
    if x > 5 then
        return 1
    end_if
    return 0
end_function
EOF
run_test "If statement" "$TEST_DIR/04_if_statement.mlp" "pass"

# Test 5: While loop
cat > "$TEST_DIR/05_while_loop.mlp" << 'EOF'
function main() as numeric
    numeric x = 0
    while x < 10
        x = x + 1
    end_while
    return x
end_function
EOF
run_test "While loop" "$TEST_DIR/05_while_loop.mlp" "pass"

# ============================================================================
# Test Suite: Multi-Function (Forward Declarations)
# ============================================================================

echo ""
echo "=== Category 2: Multi-Function Programs ==="

# Test 6: Two functions (forward call)
cat > "$TEST_DIR/06_two_functions.mlp" << 'EOF'
function add(numeric a; numeric b) as numeric
    return a + b
end_function

function main() as numeric
    numeric result = add(10; 20)
    return result
end_function
EOF
run_test "Two functions (forward call)" "$TEST_DIR/06_two_functions.mlp" "pass"

# Test 7: Three functions (chained calls)
cat > "$TEST_DIR/07_three_functions.mlp" << 'EOF'
function add(numeric a; numeric b) as numeric
    return a + b
end_function

function multiply(numeric a; numeric b) as numeric
    return a * b
end_function

function main() as numeric
    numeric sum = add(10; 20)
    numeric product = multiply(sum; 2)
    return product
end_function
EOF
run_test "Three functions (chained)" "$TEST_DIR/07_three_functions.mlp" "pass"

# Test 8: Recursive function
cat > "$TEST_DIR/08_recursive.mlp" << 'EOF'
function factorial(numeric n) as numeric
    if n <= 1 then
        return 1
    end_if
    return n * factorial(n - 1)
end_function

function main() as numeric
    return factorial(5)
end_function
EOF
run_test "Recursive function" "$TEST_DIR/08_recursive.mlp" "pass"

# Test 9: Mutual recursion
cat > "$TEST_DIR/09_mutual_recursion.mlp" << 'EOF'
function is_even(numeric n) as numeric
    if n == 0 then
        return 1
    end_if
    return is_odd(n - 1)
end_function

function is_odd(numeric n) as numeric
    if n == 0 then
        return 0
    end_if
    return is_even(n - 1)
end_function

function main() as numeric
    return is_even(4)
end_function
EOF
run_test "Mutual recursion" "$TEST_DIR/09_mutual_recursion.mlp" "pass"

# Test 10: Multiple parameters
cat > "$TEST_DIR/10_multi_params.mlp" << 'EOF'
function sum_four(numeric a; numeric b; numeric c; numeric d) as numeric
    return a + b + c + d
end_function

function main() as numeric
    return sum_four(1; 2; 3; 4)
end_function
EOF
run_test "Multiple parameters (4 params)" "$TEST_DIR/10_multi_params.mlp" "pass"

# ============================================================================
# Test Suite: Complex Expressions
# ============================================================================

echo ""
echo "=== Category 3: Complex Expressions ==="

# Test 11: Nested arithmetic
cat > "$TEST_DIR/11_nested_arithmetic.mlp" << 'EOF'
function main() as numeric
    return (10 + 20) * (30 - 5) / 5
end_function
EOF
run_test "Nested arithmetic" "$TEST_DIR/11_nested_arithmetic.mlp" "pass"

# Test 12: Boolean logic
cat > "$TEST_DIR/12_boolean_logic.mlp" << 'EOF'
function main() as numeric
    boolean a = true
    boolean b = false
    if a and not b then
        return 1
    end_if
    return 0
end_function
EOF
run_test "Boolean logic" "$TEST_DIR/12_boolean_logic.mlp" "pass"

# Test 13: Comparison operators
cat > "$TEST_DIR/13_comparison.mlp" << 'EOF'
function main() as numeric
    numeric x = 10
    if x >= 5 and x <= 15 then
        return 1
    end_if
    return 0
end_function
EOF
run_test "Comparison operators" "$TEST_DIR/13_comparison.mlp" "pass"

# Test 14: Nested if-else
cat > "$TEST_DIR/14_nested_if.mlp" << 'EOF'
function main() as numeric
    numeric x = 10
    if x > 0 then
        if x > 5 then
            return 2
        end_if
        return 1
    else
        return 0
    end_if
end_function
EOF
run_test "Nested if-else" "$TEST_DIR/14_nested_if.mlp" "pass"

# Test 15: Nested while
cat > "$TEST_DIR/15_nested_while.mlp" << 'EOF'
function main() as numeric
    numeric x = 0
    numeric y = 0
    while x < 3
        y = 0
        while y < 3
            y = y + 1
        end_while
        x = x + 1
    end_while
    return x
end_function
EOF
run_test "Nested while loops" "$TEST_DIR/15_nested_while.mlp" "pass"

# ============================================================================
# Test Suite: Error Detection
# ============================================================================

echo ""
echo "=== Category 4: Error Detection ==="

# Test 16: Undefined function
cat > "$TEST_DIR/16_undefined_func.mlp" << 'EOF'
function main() as numeric
    return foo()
end_function
EOF
run_test "Undefined function (should fail)" "$TEST_DIR/16_undefined_func.mlp" "fail"

# Test 17: Undefined variable
cat > "$TEST_DIR/17_undefined_var.mlp" << 'EOF'
function main() as numeric
    return x
end_function
EOF
run_test "Undefined variable (should fail)" "$TEST_DIR/17_undefined_var.mlp" "fail"

# Test 18: Type mismatch
cat > "$TEST_DIR/18_type_mismatch.mlp" << 'EOF'
function main() as numeric
    numeric x = true
    return x
end_function
EOF
run_test "Type mismatch (should fail)" "$TEST_DIR/18_type_mismatch.mlp" "fail"

# ============================================================================
# Results Summary
# ============================================================================

echo ""
echo "======================================"
echo "Test Results Summary"
echo "======================================"
echo "Total tests:  $TOTAL_TESTS"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
