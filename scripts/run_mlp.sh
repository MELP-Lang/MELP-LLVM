#!/bin/bash
# MELP programlarını derle ve çalıştır
# Kullanım: ./scripts/run_mlp.sh input.mlp [output_name]

set -e

MLP_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
INPUT="$1"
OUTPUT="${2:-/tmp/mlp_program}"

if [ -z "$INPUT" ]; then
    echo "Kullanım: $0 input.mlp [output_name]"
    exit 1
fi

echo "🔧 Derleniyor: $INPUT"

# 1. LLVM IR üret
"$MLP_ROOT/compiler/stage0/modules/functions/functions_compiler" "$INPUT" "${OUTPUT}.ll"

# 2. Executable oluştur
clang "${OUTPUT}.ll" \
    "$MLP_ROOT/runtime/stdlib/libmlp_stdlib.a" \
    "$MLP_ROOT/runtime/sto/bigdecimal.o" \
    "$MLP_ROOT/runtime/sto/sso_string.o" \
    "$MLP_ROOT/runtime/sto/runtime_sto.o" \
    "$MLP_ROOT/runtime/sto/sto_runtime.o" \
    -lm -o "${OUTPUT}" 2>/dev/null

echo "✅ Derlendi: ${OUTPUT}"
echo "🚀 Çalıştırılıyor..."
echo "---"

# 3. Çalıştır
"${OUTPUT}"
