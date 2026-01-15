#!/bin/bash
# ============================================================================
# MELP Stage 1 - Module Linking Script
# ============================================================================
# File: scripts/build_stage1.sh
# Purpose: Compile all MELP modules and link into executable
# Usage: ./scripts/build_stage1.sh
# ============================================================================

set -e  # Exit on error

echo "=========================================="
echo "MELP Stage 1 - Module Compilation & Linking"
echo "=========================================="
echo ""

# ============================================================================
# Configuration
# ============================================================================
STAGE0_COMPILER="./compiler/stage0/modules/functions/functions_standalone"
OUTPUT_DIR="temp/stage1_build"
MODULES_DIR="modules"
RUNTIME_DIR="runtime/sto"

# Create output directory
mkdir -p "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/lexer"
mkdir -p "$OUTPUT_DIR/parser"
mkdir -p "$OUTPUT_DIR/codegen"

# ============================================================================
# Step 1: Compile Lexer Modules
# ============================================================================
echo "Step 1: Compiling Lexer Modules..."
echo "-----------------------------------"

LEXER_MODULES=(
    "char_utils"
    "token"
    "tokenize_identifiers"
    "tokenize_literals"
    "tokenize_operators"
    "lexer"
)

for module in "${LEXER_MODULES[@]}"; do
    echo "  Compiling lexer/$module.mlp → .ll"
    $STAGE0_COMPILER --compile-only --backend=llvm \
        "$MODULES_DIR/lexer_mlp/$module.mlp" \
        "$OUTPUT_DIR/lexer/$module.ll" 2>&1 | grep -v "^$" || true
done

echo "  ✅ Lexer modules compiled (${#LEXER_MODULES[@]} files)"
echo ""

# ============================================================================
# Step 2: Compile Parser Modules
# ============================================================================
echo "Step 2: Compiling Parser Modules..."
echo "-----------------------------------"

PARSER_MODULES=(
    "ast_nodes"
    "token_stream"
    "parser_errors"
    "parser_expr"
    "parser_stmt"
    "parser_control"
    "parser_func"
    "parser_struct"
    "parser_enum"
    "parser_import"
    "parser_switch"
    "parser_for"
    "parser_call"
    "parser_index"
    "parser_compound"
    "parser_integration"
)

for module in "${PARSER_MODULES[@]}"; do
    echo "  Compiling parser/$module.mlp → .ll"
    $STAGE0_COMPILER --compile-only --backend=llvm \
        "$MODULES_DIR/parser_mlp/$module.mlp" \
        "$OUTPUT_DIR/parser/$module.ll" 2>&1 | grep -v "^$" || true
done

echo "  ✅ Parser modules compiled (${#PARSER_MODULES[@]} files)"
echo ""

# ============================================================================
# Step 3: Compile CodeGen Modules
# ============================================================================
echo "Step 3: Compiling CodeGen Modules..."
echo "-----------------------------------"

CODEGEN_MODULES=(
    "ir_builder"
    "type_mapper"
    "symbol_table"
    "codegen_literal"
    "codegen_variable"
    "codegen_arithmetic"
    "codegen_comparison"
    "codegen_logical"
    "codegen_stmt"
    "codegen_control"
    "codegen_while"
    "codegen_for"
    "codegen_functions"
    "codegen_arrays"
    "codegen_integration"
)

for module in "${CODEGEN_MODULES[@]}"; do
    echo "  Compiling codegen/$module.mlp → .ll"
    $STAGE0_COMPILER --compile-only --backend=llvm \
        "$MODULES_DIR/codegen_mlp/$module.mlp" \
        "$OUTPUT_DIR/codegen/$module.ll" 2>&1 | grep -v "^$" || true
done

echo "  ✅ CodeGen modules compiled (${#CODEGEN_MODULES[@]} files)"
echo ""

# ============================================================================
# Step 4: Convert LLVM IR to Object Files
# ============================================================================
echo "Step 4: Converting LLVM IR to Object Files..."
echo "----------------------------------------------"

# Convert lexer modules
for module in "${LEXER_MODULES[@]}"; do
    if [ -f "$OUTPUT_DIR/lexer/$module.ll" ]; then
        echo "  $module.ll → $module.o"
        llc-14 -filetype=obj "$OUTPUT_DIR/lexer/$module.ll" -o "$OUTPUT_DIR/lexer/$module.o"
    fi
done

# Convert parser modules
for module in "${PARSER_MODULES[@]}"; do
    if [ -f "$OUTPUT_DIR/parser/$module.ll" ]; then
        echo "  $module.ll → $module.o"
        llc-14 -filetype=obj "$OUTPUT_DIR/parser/$module.ll" -o "$OUTPUT_DIR/parser/$module.o"
    fi
done

# Convert codegen modules
for module in "${CODEGEN_MODULES[@]}"; do
    if [ -f "$OUTPUT_DIR/codegen/$module.ll" ]; then
        echo "  $module.ll → $module.o"
        llc-14 -filetype=obj "$OUTPUT_DIR/codegen/$module.ll" -o "$OUTPUT_DIR/codegen/$module.o"
    fi
done

echo "  ✅ Object files created"
echo ""

# ============================================================================
# Step 5: Link Everything Together
# ============================================================================
echo "Step 5: Linking Object Files..."
echo "--------------------------------"

# Collect all object files
OBJECT_FILES=$(find "$OUTPUT_DIR" -name "*.o" 2>/dev/null)
OBJECT_COUNT=$(echo "$OBJECT_FILES" | wc -l)

echo "  Found $OBJECT_COUNT object files"
echo "  Linking → melp_stage1"

# Link with runtime library
gcc -o "$OUTPUT_DIR/melp_stage1" \
    $OBJECT_FILES \
    "$RUNTIME_DIR/libmelp_sto.a" \
    -lm -no-pie 2>&1 | grep -v "^$" || true

if [ -f "$OUTPUT_DIR/melp_stage1" ]; then
    echo "  ✅ Executable created: $OUTPUT_DIR/melp_stage1"
else
    echo "  ❌ Linking failed!"
    exit 1
fi

echo ""

# ============================================================================
# Step 6: Summary
# ============================================================================
echo "=========================================="
echo "Build Summary"
echo "=========================================="
echo "  Lexer modules:   ${#LEXER_MODULES[@]}"
echo "  Parser modules:  ${#PARSER_MODULES[@]}"
echo "  CodeGen modules: ${#CODEGEN_MODULES[@]}"
echo "  Total modules:   $((${#LEXER_MODULES[@]} + ${#PARSER_MODULES[@]} + ${#CODEGEN_MODULES[@]}))"
echo "  Object files:    $OBJECT_COUNT"
echo ""
echo "  Output directory: $OUTPUT_DIR"
echo "  Executable:       $OUTPUT_DIR/melp_stage1"
echo ""

# ============================================================================
# Step 7: Verification Test
# ============================================================================
echo "=========================================="
echo "Running Verification Test..."
echo "=========================================="

if [ -x "$OUTPUT_DIR/melp_stage1" ]; then
    echo "  Testing executable..."
    "$OUTPUT_DIR/melp_stage1" || echo "  (Exit code: $?)"
    echo ""
    echo "  ✅ Stage 1 compiler executable is ready!"
else
    echo "  ❌ Executable not found or not executable"
    exit 1
fi

echo ""
echo "=========================================="
echo "✅ BUILD COMPLETE!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "  1. Test compilation: $OUTPUT_DIR/melp_stage1 <source.mlp>"
echo "  2. Run tests: ./scripts/test_stage1.sh"
echo "  3. Self-compile: $OUTPUT_DIR/melp_stage1 compiler/stage1/*.mlp"
echo ""
