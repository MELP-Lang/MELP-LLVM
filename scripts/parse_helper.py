#!/usr/bin/env python3
"""
MELP Gen1 v2 - String Parser Helper
YZ_04: External helper for string parsing (bootstrap)

This script extracts function name and return value from simple MELP functions.
Later (YZ_05+) we'll move this logic into pure PMPL.
"""

import sys
import re

def extract_function_name(source):
    """
    Extract function name from MELP source.
    Input:  "function my_test() returns numeric return 77 end"
    Output: "my_test"
    """
    # Pattern: "function" + whitespace + name + "("
    match = re.search(r'function\s+(\w+)\s*\(', source)
    if match:
        return match.group(1)
    return "main"  # Default fallback

def extract_return_value(source):
    """
    Extract return value from simple function.
    Input:  "function test() returns numeric return 42 end"
    Output: 42
    """
    # Pattern: "return" + whitespace + number
    match = re.search(r'return\s+(\d+)', source)
    if match:
        return int(match.group(1))
    return 0  # Default fallback

def main():
    if len(sys.argv) != 2:
        print("Usage: parse_helper.py <source_file>", file=sys.stderr)
        sys.exit(1)
    
    source_file = sys.argv[1]
    
    # Read source
    with open(source_file, 'r') as f:
        source = f.read()
    
    # Extract info
    func_name = extract_function_name(source)
    return_val = extract_return_value(source)
    
    # Write to output files (for PMPL to read)
    with open('/tmp/parsed_func_name.txt', 'w') as f:
        f.write(func_name)
    
    with open('/tmp/parsed_return_val.txt', 'w') as f:
        f.write(str(return_val))
    
    print(f"✅ Parsed: {func_name}() returns {return_val}")

if __name__ == '__main__':
    main()
