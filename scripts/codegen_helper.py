#!/usr/bin/env python3
"""
MELP Gen1 v3 - Template-based Code Generator
YZ_04: Generate LLVM IR using templates + Python substitution
"""

import sys
import re

def generate_llvm_ir(func_name, return_val, template_file, output_file):
    """
    Generate LLVM IR by substituting template placeholders.
    """
    # Read template
    with open(template_file, 'r') as f:
        template = f.read()
    
    # Substitute placeholders
    ir = template.replace('__FUNC_NAME__', func_name)
    ir = ir.replace('__RETURN_VAL__', str(return_val))
    
    # Write output
    with open(output_file, 'w') as f:
        f.write(ir)
    
    print(f"✅ Generated: {output_file}")
    print(f"   Function: {func_name}() returns {return_val}")

def main():
    if len(sys.argv) != 3:
        print("Usage: codegen_helper.py <func_name_file> <return_val_file>", file=sys.stderr)
        sys.exit(1)
    
    func_name_file = sys.argv[1]
    return_val_file = sys.argv[2]
    
    # Read parsed data
    with open(func_name_file, 'r') as f:
        func_name = f.read().strip()
    
    with open(return_val_file, 'r') as f:
        return_val = int(f.read().strip())
    
    # Generate
    generate_llvm_ir(
        func_name,
        return_val,
        'templates/function_template.ll',
        'test.ll'
    )

if __name__ == '__main__':
    main()
