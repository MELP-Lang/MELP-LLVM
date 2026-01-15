#!/bin/bash
# MELP Architecture Validation Script
# Prevents architecture violations before commit

echo "🔍 Validating MELP Architecture..."
echo ""

ERRORS=0
WARNINGS=0

# ============================================================================
# 1. Check main.c size
# ============================================================================
echo "📏 Checking main.c size..."
MAIN_LINES=$(wc -l < compiler/stage0/main.c 2>/dev/null || echo "0")
if [ "$MAIN_LINES" -gt 300 ]; then
    echo "❌ CRITICAL: main.c is $MAIN_LINES lines (MAX 300)"
    echo "   → MUST refactor before committing!"
    ((ERRORS++))
elif [ "$MAIN_LINES" -gt 250 ]; then
    echo "⚠️  WARNING: main.c is $MAIN_LINES lines (approaching limit)"
    ((WARNINGS++))
else
    echo "✅ main.c size OK: $MAIN_LINES lines"
fi

# ============================================================================
# 2. Check for version-suffixed binaries
# ============================================================================
echo ""
echo "🔍 Checking binary names..."
VERSIONED_BINS=$(find compiler/stage0 -maxdepth 1 -type f -name "melpc_*" 2>/dev/null)
if [ -n "$VERSIONED_BINS" ]; then
    echo "❌ Found versioned binaries:"
    echo "$VERSIONED_BINS"
    echo "   → Binary should be named 'melpc' only!"
    ((ERRORS++))
else
    echo "✅ No versioned binaries found"
fi

# ============================================================================
# 3. Check for forbidden central files
# ============================================================================
echo ""
echo "🔍 Checking for forbidden files..."
FORBIDDEN=("core.c" "utils.c" "compiler.c" "all.c" "monolith.c")
for file in "${FORBIDDEN[@]}"; do
    if find compiler/stage0 -maxdepth 1 -name "$file" 2>/dev/null | grep -q .; then
        echo "❌ Forbidden central file found: $file"
        echo "   → Use modular structure instead!"
        ((ERRORS++))
    fi
done
echo "✅ No forbidden files found"

# ============================================================================
# 4. Check for large files (potential monoliths)
# ============================================================================
echo ""
echo "🔍 Checking for large files..."
LARGE_FILES=$(find compiler/stage0 -maxdepth 1 -name "*.c" -exec sh -c '
    lines=$(wc -l < "$1")
    if [ $lines -gt 1000 ]; then
        echo "$1: $lines lines"
    fi
' _ {} \;)

if [ -n "$LARGE_FILES" ]; then
    echo "⚠️  Large files found (>1000 lines):"
    echo "$LARGE_FILES"
    echo "   → Consider splitting into modules"
    ((WARNINGS++))
else
    echo "✅ No large files found"
fi

# ============================================================================
# 5. Check module structure
# ============================================================================
echo ""
echo "🔍 Checking module structure..."
if [ ! -d "compiler/stage0/modules" ]; then
    echo "❌ modules/ directory not found!"
    ((ERRORS++))
else
    MODULE_COUNT=$(find compiler/stage0/modules -mindepth 1 -maxdepth 1 -type d | wc -l)
    echo "✅ Found $MODULE_COUNT modules"
fi

# ============================================================================
# 6. Check ARCHITECTURE.md exists
# ============================================================================
echo ""
echo "🔍 Checking for ARCHITECTURE.md..."
if [ ! -f "ARCHITECTURE.md" ]; then
    echo "⚠️  ARCHITECTURE.md not found!"
    echo "   → Create it to document rules"
    ((WARNINGS++))
else
    echo "✅ ARCHITECTURE.md exists"
fi

# ============================================================================
# SUMMARY
# ============================================================================
echo ""
echo "═══════════════════════════════════════"
if [ $ERRORS -gt 0 ]; then
    echo "❌ VALIDATION FAILED!"
    echo "   Errors: $ERRORS"
    echo "   Warnings: $WARNINGS"
    echo ""
    echo "Fix errors before committing!"
    echo "See ARCHITECTURE.md for rules."
    exit 1
elif [ $WARNINGS -gt 0 ]; then
    echo "⚠️  VALIDATION PASSED WITH WARNINGS"
    echo "   Warnings: $WARNINGS"
    echo ""
    echo "Consider addressing warnings."
    exit 0
else
    echo "✅ VALIDATION PASSED!"
    echo "   Architecture is clean."
    exit 0
fi
