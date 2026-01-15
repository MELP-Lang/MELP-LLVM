#!/bin/bash
# MELP Stage 2 Bootstrap - Build Script
# Phase: 6.0 - Integration & Bootstrap Test
# Purpose: Build unified stage2_bootstrap compiler from modular components

set -e  # Exit on error

echo "🔨 Building MELP Stage 2 Bootstrap Compiler (Phase 6.0)..."

# Configuration
STAGE2_DIR="$(cd "$(dirname "$0")" && pwd)"
C_HELPERS="$STAGE2_DIR/c_helpers"
OUTPUT_BINARY="$STAGE2_DIR/stage2_bootstrap"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Step 1: Compile modular components
echo -e "${BLUE}📦 Step 1/3: Compiling modular components...${NC}"

# Common
gcc -c "$C_HELPERS/common/token.c" -o "$C_HELPERS/common/token.o" -O2 -Wall -I"$STAGE2_DIR"
gcc -c "$C_HELPERS/common/ast.c" -o "$C_HELPERS/common/ast.o" -O2 -Wall -I"$STAGE2_DIR"

# Lexer
gcc -c "$C_HELPERS/lexer/lexer_impl.c" -o "$C_HELPERS/lexer/lexer_impl.o" -O2 -Wall -I"$STAGE2_DIR"

# Parser
gcc -c "$C_HELPERS/parser/parser_impl.c" -o "$C_HELPERS/parser/parser_impl.o" -O2 -Wall -I"$STAGE2_DIR"

# Semantic
gcc -c "$C_HELPERS/semantic/symbol_table.c" -o "$C_HELPERS/semantic/symbol_table.o" -O2 -Wall -I"$STAGE2_DIR"
gcc -c "$C_HELPERS/semantic/type_checker.c" -o "$C_HELPERS/semantic/type_checker.o" -O2 -Wall -I"$STAGE2_DIR"
gcc -c "$C_HELPERS/semantic/semantic_analyzer.c" -o "$C_HELPERS/semantic/semantic_analyzer.o" -O2 -Wall -I"$STAGE2_DIR" 2>&1 | grep -v "strncpy.*truncation" || true

# Codegen
gcc -c "$C_HELPERS/codegen/codegen.c" -o "$C_HELPERS/codegen/codegen.o" -O2 -Wall -I"$STAGE2_DIR" 2>&1 | grep -v "strncpy.*truncation" || true

echo -e "${GREEN}✅ All components compiled${NC}"

# Step 2: Link unified compiler
echo -e "${BLUE}🔗 Step 2/3: Linking unified compiler...${NC}"

gcc -o "$OUTPUT_BINARY" \
    "$STAGE2_DIR/stage2_bootstrap.c" \
    "$C_HELPERS/common/token.o" \
    "$C_HELPERS/common/ast.o" \
    "$C_HELPERS/lexer/lexer_impl.o" \
    "$C_HELPERS/parser/parser_impl.o" \
    "$C_HELPERS/semantic/symbol_table.o" \
    "$C_HELPERS/semantic/type_checker.o" \
    "$C_HELPERS/semantic/semantic_analyzer.o" \
    "$C_HELPERS/codegen/codegen.o" \
    -O2 -Wall -I"$STAGE2_DIR"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Unified compiler linked successfully${NC}"
else
    echo -e "${RED}❌ Linking failed${NC}"
    exit 1
fi

# Step 3: Verify binary
echo -e "${BLUE}🔍 Step 3/3: Verifying binary...${NC}"

if [ -x "$OUTPUT_BINARY" ]; then
    SIZE=$(ls -lh "$OUTPUT_BINARY" | awk '{print $5}')
    echo -e "${GREEN}✅ Binary created: $OUTPUT_BINARY ($SIZE)${NC}"
    echo ""
    echo "Testing binary..."
    $OUTPUT_BINARY --version
    echo ""
else
    echo -e "${RED}❌ Binary not executable${NC}"
    exit 1
fi

# Cleanup object files (optional)
echo -e "${BLUE}🧹 Cleaning up object files...${NC}"
find "$C_HELPERS" -name "*.o" -delete
echo -e "${GREEN}✅ Cleanup complete${NC}"

echo ""
echo "🎉 Build complete!"
echo "   Binary: $OUTPUT_BINARY"
echo "   Usage:  $OUTPUT_BINARY <input.mlp> [-o output.ll] [-v]"
echo ""
echo "Features (Phase 6.0):"
echo "  ✓ Forward declarations"
echo "  ✓ Multi-function programs"
echo "  ✓ Modular architecture"
echo "  ✓ Clean error reporting"
echo ""
