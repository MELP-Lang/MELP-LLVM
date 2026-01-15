/*
 * MELP Stage 2 Bootstrap - Unified Compiler Executable
 * Phase: 6.0 - Integration & Bootstrap Test
 * Date: 15 Ocak 2026
 * 
 * Purpose: Unified compiler that orchestrates lexer, parser, semantic, codegen
 * Architecture: Glue code that calls modular components (c_helpers/)
 * NOT an orchestrator: Simple sequential pipeline
 * 
 * Pipeline:
 *   Source → Parser (includes Lexer) → Semantic → Codegen → LLVM IR
 * 
 * AUTONOMOUS Compliance:
 *   - Minimal glue code (imports from c_helpers)
 *   - No business logic duplication
 *   - Error propagation from each phase
 *   - Clean resource management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Import modular components
#include "c_helpers/parser/parser_impl.h"
#include "c_helpers/semantic/semantic_analyzer.h"
#include "c_helpers/codegen/codegen.h"

/* ============================================================================
 * FILE I/O UTILITIES
 * ============================================================================ */

/* Read source file into string */
static char* read_source_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer
    char* buffer = malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    // Read file
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    
    fclose(file);
    return buffer;
}

/* ============================================================================
 * COMPILATION PIPELINE
 * ============================================================================ */

/* Compile source to LLVM IR
 * Returns: true on success, false on error
 */
static bool compile(const char* input_file, const char* output_file, bool verbose) {
    // Step 1: Read source file
    if (verbose) {
        printf("Step 1/4: Reading source file '%s'...\n", input_file);
    }
    
    char* source = read_source_file(input_file);
    if (!source) {
        return false;
    }
    
    // Step 2: Parse (includes lexing)
    if (verbose) {
        printf("Step 2/4: Parsing (lexing + syntax analysis)...\n");
    }
    
    ASTNode* ast = parse(source);  // parse() does tokenize internally
    
    if (!ast) {
        fprintf(stderr, "Error: Parse failed\n");
        const char* err = get_parse_error();
        if (err) {
            fprintf(stderr, "%s\n", err);
        }
        free(source);
        return false;
    }
    
    if (verbose) {
        if (ast->type == AST_PROGRAM) {
            printf("  ✓ AST generated (%d functions)\n", ast->data.program.function_count);
        } else {
            printf("  ✓ AST generated\n");
        }
    }
    
    // Step 3: Semantic analysis
    if (verbose) {
        printf("Step 3/4: Semantic analysis...\n");
    }
    
    if (!analyze_program(ast)) {
        fprintf(stderr, "Error: Semantic analysis failed\n");
        const char* err = get_semantic_error();
        if (err) {
            fprintf(stderr, "%s\n", err);
        }
        free_ast(ast);
        free(source);
        return false;
    }
    
    if (verbose) {
        printf("  ✓ Semantic validation complete\n");
    }
    
    // Step 4: Code generation
    if (verbose) {
        printf("Step 4/4: Code generation (LLVM IR)...\n");
    }
    
    if (!generate_code(ast, output_file)) {
        fprintf(stderr, "Error: Code generation failed\n");
        const char* err = get_codegen_error();
        if (err) {
            fprintf(stderr, "%s\n", err);
        }
        free_ast(ast);
        free(source);
        return false;
    }
    
    if (verbose) {
        printf("  ✓ LLVM IR written to '%s'\n", output_file);
    }
    
    // Cleanup
    free_ast(ast);
    free(source);
    
    return true;
}

/* ============================================================================
 * MAIN ENTRY POINT
 * ============================================================================ */

int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc < 2) {
        fprintf(stderr, "MELP Stage 2 Bootstrap Compiler\n");
        fprintf(stderr, "Phase 6.0 - Integration & Bootstrap Test\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Usage: %s <input.mlp> [-o <output.ll>] [-v]\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -o FILE    Output LLVM IR to FILE (default: output.ll)\n");
        fprintf(stderr, "  -v         Verbose mode (show compilation steps)\n");
        fprintf(stderr, "  --version  Show version information\n");
        fprintf(stderr, "  --help     Show this help message\n");
        return 1;
    }
    
    // Handle version flag
    if (strcmp(argv[1], "--version") == 0) {
        printf("MELP Stage 2 Bootstrap Compiler\n");
        printf("Version: 2.1.0 (Phase 6.0)\n");
        printf("Build Date: 15 Ocak 2026\n");
        printf("Features: Forward declarations, multi-function support\n");
        return 0;
    }
    
    // Handle help flag
    if (strcmp(argv[1], "--help") == 0) {
        printf("MELP Stage 2 Bootstrap Compiler\n");
        printf("Phase 6.0 - Integration & Bootstrap Test\n");
        printf("\n");
        printf("Usage: %s <input.mlp> [-o <output.ll>] [-v]\n", argv[0]);
        printf("\n");
        printf("Description:\n");
        printf("  Compiles MELP source code to LLVM IR.\n");
        printf("  Supports multi-function programs with forward declarations.\n");
        printf("\n");
        printf("Options:\n");
        printf("  -o FILE    Output LLVM IR to FILE (default: output.ll)\n");
        printf("  -v         Verbose mode (show compilation steps)\n");
        printf("  --version  Show version information\n");
        printf("  --help     Show this help message\n");
        printf("\n");
        printf("Examples:\n");
        printf("  %s program.mlp                  # Compile to output.ll\n", argv[0]);
        printf("  %s program.mlp -o program.ll    # Compile to program.ll\n", argv[0]);
        printf("  %s program.mlp -o program.ll -v # Verbose compilation\n", argv[0]);
        return 0;
    }
    
    // Parse arguments
    const char* input_file = argv[1];
    const char* output_file = "output.ll";  // Default output
    bool verbose = false;
    
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
    }
    
    // Run compilation
    if (verbose) {
        printf("=== MELP Stage 2 Bootstrap Compiler ===\n");
        printf("Input:  %s\n", input_file);
        printf("Output: %s\n\n", output_file);
    }
    
    bool success = compile(input_file, output_file, verbose);
    
    if (success) {
        if (verbose) {
            printf("\n✅ Compilation successful!\n");
        }
        return 0;
    } else {
        if (verbose) {
            printf("\n❌ Compilation failed\n");
        }
        return 1;
    }
}
