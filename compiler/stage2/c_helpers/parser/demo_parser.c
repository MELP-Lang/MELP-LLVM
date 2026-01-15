/* Quick parser demo */
#include "parser_impl.h"
#include <stdio.h>

int main(void) {
    const char* code = 
        "function factorial(numeric n) as numeric\n"
        "  if n <= 1 then\n"
        "    return 1\n"
        "  else\n"
        "    return n * factorial(n - 1)\n"
        "  end_if\n"
        "end_function\n"
        "\n"
        "function main() as numeric\n"
        "  numeric result = factorial(5)\n"
        "  return result\n"
        "end_function\n";
    
    printf("Parsing factorial program...\n");
    printf("Source:\n%s\n", code);
    printf("---\n");
    
    ASTNode* ast = parse(code);
    
    if (ast) {
        printf("✅ Parse successful!\n");
        printf("AST structure:\n");
        print_ast(ast, 0);
        free_ast(ast);
        return 0;
    } else {
        printf("❌ Parse failed: %s\n", get_parse_error());
        return 1;
    }
}
