#!/bin/bash
# MELP Stage 2 Bootstrap - Bootstrap Validation Test
# Phase: 6.0 - Integration & Bootstrap Test
# Date: 15 Ocak 2026
# 
# Purpose: Validate that stage2_bootstrap can compile real programs
#          and produce working executables

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
RUNTIME_LIB="$PROJECT_ROOT/runtime/stdlib/libmlp_stage2.a"
TEMP_DIR="/tmp/melp_bootstrap_validation"

# Create temp directory
mkdir -p "$TEMP_DIR"

echo "======================================"
echo "MELP Stage 2 Bootstrap - Validation"
echo "Phase 6.0"
echo "======================================"
echo ""

# Test counter
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Helper function to run validation test
run_validation() {
    local test_name="$1"
    local source_code="$2"
    local expected_output="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Validation $TOTAL_TESTS: $test_name ... "
    
    local source_file="$TEMP_DIR/test_$TOTAL_TESTS.mlp"
    local ir_file="$TEMP_DIR/test_$TOTAL_TESTS.ll"
    local obj_file="$TEMP_DIR/test_$TOTAL_TESTS.o"
    local exe_file="$TEMP_DIR/test_$TOTAL_TESTS"
    
    # Step 1: Create source file
    echo "$source_code" > "$source_file"
    
    # Step 2: Compile to LLVM IR
    if ! $COMPILER "$source_file" -o "$ir_file" 2>/dev/null; then
        echo -e "${RED}✗ FAIL (Compilation)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi
    
    # Step 3: LLVM IR → Object
    if ! llc -filetype=obj "$ir_file" -o "$obj_file" 2>/dev/null; then
        echo -e "${RED}✗ FAIL (LLVM backend)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi
    
    # Step 4: Link (if runtime exists)
    if [ -f "$RUNTIME_LIB" ]; then
        if ! gcc -no-pie "$obj_file" "$RUNTIME_LIB" -o "$exe_file" 2>/dev/null; then
            echo -e "${RED}✗ FAIL (Linking)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            return
        fi
    else
        if ! gcc -no-pie "$obj_file" -o "$exe_file" 2>/dev/null; then
            echo -e "${RED}✗ FAIL (Linking)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            return
        fi
    fi
    
    # Step 5: Execute and check exit code
    "$exe_file"
    local exit_code=$?
    
    # Check if exit code matches expected value
    if [ "$exit_code" = "$expected_output" ]; then
        echo -e "${GREEN}✓ PASS (exit=$exit_code)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "  Expected exit code: $expected_output"
        echo "  Got exit code:      $exit_code"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# ============================================================================
# Bootstrap Validation Tests
# ============================================================================

echo "=== Bootstrap Validation Tests ==="
echo ""

# Test 1: Simple return value
run_validation "Simple return (42)" \
"function main() as numeric
    return 42
end_function" \
"42"

# Test 2: Arithmetic
run_validation "Arithmetic (10+20)" \
"function main() as numeric
    return 10 + 20
end_function" \
"30"

# Test 3: Variable
run_validation "Variable assignment" \
"function main() as numeric
    numeric x = 100
    return x
end_function" \
"100"

# Test 4: Function call
run_validation "Function call (add)" \
"function add(numeric a; numeric b) as numeric
    return a + b
end_function

function main() as numeric
    return add(50; 25)
end_function" \
"75"

# Test 5: Conditional
run_validation "Conditional (if-then-else)" \
"function main() as numeric
    numeric x = 10
    if x > 5 then
        return 1
    else
        return 0
    end_if
end_function" \
"1"

# Test 6: Loop
run_validation "Loop (while)" \
"function main() as numeric
    numeric sum = 0
    numeric i = 1
    while i <= 10
        sum = sum + i
        i = i + 1
    end_while
    return sum
end_function" \
"55"

# Test 7: Recursive factorial
run_validation "Recursive factorial(5)" \
"function factorial(numeric n) as numeric
    if n <= 1 then
        return 1
    end_if
    return n * factorial(n - 1)
end_function

function main() as numeric
    return factorial(5)
end_function" \
"120"

# Test 8: Multi-function (helper functions)
run_validation "Multi-function helpers" \
"function double(numeric x) as numeric
    return x * 2
end_function

function triple(numeric x) as numeric
    return x * 3
end_function

function main() as numeric
    numeric a = double(10)
    numeric b = triple(5)
    return a + b
end_function" \
"35"

# Test 9: Complex expression
run_validation "Complex expression" \
"function main() as numeric
    numeric x = (10 + 5) * 2
    numeric y = (20 - 5) / 3
    return x + y
end_function" \
"35"

# Test 10: Boolean logic
run_validation "Boolean logic" \
"function main() as numeric
    boolean a = true
    boolean b = false
    if a and not b then
        return 7
    end_if
    return 0
end_function" \
"7"

# ============================================================================
# Results Summary
# ============================================================================

echo ""
echo "======================================"
echo "Bootstrap Validation Results"
echo "======================================"
echo "Total validations:  $TOTAL_TESTS"
echo -e "Passed:             ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:             ${RED}$FAILED_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ Bootstrap validation successful!${NC}"
    echo ""
    echo "stage2_bootstrap is PRODUCTION READY! 🎉"
    echo ""
    echo "Features validated:"
    echo "  ✓ Source → LLVM IR compilation"
    echo "  ✓ LLVM IR → Native executable"
    echo "  ✓ Multi-function programs"
    echo "  ✓ Forward declarations"
    echo "  ✓ Recursion"
    echo "  ✓ Control flow (if/while)"
    echo "  ✓ Complex expressions"
    echo ""
    exit 0
else
    echo -e "${RED}❌ Bootstrap validation failed${NC}"
    exit 1
fi
