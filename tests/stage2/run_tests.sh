#!/bin/bash
# MELP Stage 2 - Comprehensive Test Suite
# Purpose: Validate lexer, parser, semantic, and codegen

# Change to workspace root
cd "$(dirname "$0")/../.."

COMPILER="./compiler/stage2/stage2_v1"
COMPILE_SCRIPT="./compiler/stage2/compile.sh"
TEST_DIR="tests/stage2"
TEMP_DIR="/tmp/melp_stage2_tests"
RUNTIME_LIB="./runtime/stdlib/libmlp_stage2.a"
STO_LIB="./runtime/sto/libsto_runtime.a"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Setup
mkdir -p "$TEMP_DIR"
PASSED=0
FAILED=0

echo "🧪 MELP Stage 2 - Comprehensive Test Suite"
echo "=========================================="
echo ""

# Test helper function
run_test() {
    local test_name=$1
    local test_file=$2
    local expected_exit=$3
    
    echo -n "Testing: $test_name ... "
    
    # Compile to LLVM IR (stdout)
    if ! $COMPILER "$test_file" > "$TEMP_DIR/test.ll" 2>/dev/null; then
        echo -e "${RED}FAILED${NC} (compilation error)"
        FAILED=$((FAILED + 1))
        return
    fi
    
    # Compile LLVM IR to object
    if ! llc -filetype=obj "$TEMP_DIR/test.ll" -o "$TEMP_DIR/test.o" 2>/dev/null; then
        echo -e "${RED}FAILED${NC} (llc error)"
        FAILED=$((FAILED + 1))
        return
    fi
    
    # Link to executable with runtime libraries (BOOTSTRAP_YZ_07 update)
    if ! gcc -no-pie "$TEMP_DIR/test.o" "$RUNTIME_LIB" "$STO_LIB" -o "$TEMP_DIR/test" 2>/dev/null; then
        echo -e "${RED}FAILED${NC} (link error)"
        FAILED=$((FAILED + 1))
        return
    fi
    
    # Execute and check exit code
    set +e
    "$TEMP_DIR/test" 2>/dev/null
    local actual_exit=$?
    set -e
    
    if [ "$actual_exit" -eq "$expected_exit" ]; then
        echo -e "${GREEN}PASSED${NC} (exit code: $actual_exit)"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAILED${NC} (expected: $expected_exit, got: $actual_exit)"
        FAILED=$((FAILED + 1))
    fi
}

# Run tests
echo "📋 Basic Tests"
echo "--------------"
run_test "Simple return" "$TEST_DIR/test_simple_return.mlp" 42
run_test "Return only" "$TEST_DIR/test_only_return.mlp" 7

echo ""
echo "📋 Variable Tests"
echo "-----------------"
run_test "Simple variable" "$TEST_DIR/test_simple_var.mlp" 5
run_test "Variable expressions" "$TEST_DIR/test_variables.mlp" 50

echo ""
echo "📋 Control Flow Tests"
echo "---------------------"
run_test "If statement" "$TEST_DIR/test_if_statement.mlp" 1
run_test "While loop" "$TEST_DIR/test_while_loop.mlp" 45

echo ""
echo "📋 Function Tests"
echo "----------------"
run_test "Function call" "$TEST_DIR/test_function_call.mlp" 30

echo ""
echo "=========================================="
echo "📊 Test Summary"
echo "=========================================="
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo "Total:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
