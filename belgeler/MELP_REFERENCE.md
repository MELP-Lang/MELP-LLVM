# MELP - Comprehensive AI Reference Guide

**Last Updated:** 12 Aralık 2025  
**Version:** 2.0  
**Purpose:** Single source of truth for all AI sessions (YZ series)

---

## 🚀 Quick Start (Read This First!)

### What is MELP?
**MELP** (Multi-Language Programming / Melting Pot) is a self-hosting programming language with:
- **Multi-syntax support** - Write in Python-style, C-style, or custom syntax
- **Multi-language** - Code in Turkish, English, Hindi, Chinese, or your own language
- **STO (Smart Type Optimization)** - User sees 2 types (numeric, string), compiler optimizes behind scenes
- **Stateless by default** - All variables are function-local unless explicitly stateful
- **Self-hosting goal** - MELP compiler written in MELP itself

### Project Status
- **Stage 0:** ~98% complete (bootstrap compiler in C)
- **Phase 11:** Module system + incremental compilation ✅
- **Phase 12:** TTO→STO refactoring in progress (Parts 1-3 done, 4-5 pending)
- **Latest:** YZ_52 completed handoff & documentation sync

### Critical Files Location
```
/mnt/project/
├── MELP_REFERENCE.md       ← YOU ARE HERE (read this first!)
├── ARCHITECTURE.md          ← Modular architecture rules
├── TODO.md                  ← Detailed phase tracking
├── NEXT_AI_START_HERE.md    ← Latest session status
├── MELP_VISION.md           ← Project philosophy
└── kurallar_kitabı.md       ← Comprehensive Turkish reference
```

---

## 📑 Table of Contents

1. [Architecture & Philosophy](#architecture--philosophy)
2. [Current Project Status](#current-project-status)
3. [YZ Workflow (Critical!)](#yz-workflow-critical)
4. [STO (Smart Type Optimization)](#sto-smart-type-optimization)
5. [Language Syntax Quick Reference](#language-syntax-quick-reference)
6. [Development Guidelines](#development-guidelines)
7. [Common Tasks](#common-tasks)
8. [Troubleshooting](#troubleshooting)

---

## 1. Architecture & Philosophy

### Core Principles

**1. "Melting Pot" Philosophy**
```
User Code (Any Language + Any Syntax)
    ↓
[diller.json + syntax.json]     ← User can customize
    ↓
Pragmatic MLP (English base)     ← AI works HERE
    ↓
[Lexer → Parser → Codegen]
    ↓
Native Binary
```

**Key Insight:** AI only sees Pragmatic MLP (English keywords). User-facing layer is abstracted away.

**2. Stateless by Default**
- All variables are function-local
- No global state unless explicitly declared with `state` keyword
- Predictable, testable, parallel-friendly

**3. Modular Architecture (CRITICAL!)**
```
compiler/stage0/modules/
├── arithmetic/          ← Independent module
├── comparison/          ← Independent module
├── functions/           ← Independent module
└── ...                  ← 26 total modules
```

**NO CENTRAL FILES!** - Modules communicate via JSON/pipes only

**4. STO = Simplicity + Performance**
- User: 2 types (`numeric`, `string`)
- Compiler: Optimizes to int64/double/BigDecimal/SSO/heap automatically
- User never sees optimization complexity

---

## 2. Current Project Status

### ✅ Completed Features (Stage 0)

**Core Language (Phase 0-5):**
- ✅ Variables: numeric, string, boolean
- ✅ Functions: declaration, calls, return, recursion
- ✅ Arithmetic: +, -, *, /, unary minus
- ✅ Comparison: >, <, >=, <=, ==, !=
- ✅ Boolean logic: and, or, not
- ✅ Control flow: if/else, while, for, exit, continue
- ✅ Arrays: [1,2,3], fixed size, fast access
- ✅ Lists: (1;2;3;), dynamic, mutable
- ✅ Tuples: <1,2,3>, immutable, stack-based

**Advanced Features (Phase 6-11):**
- ✅ Structs: Custom data types
- ✅ Nested functions & Closures
- ✅ Defer statement: Cleanup code
- ✅ Module system: import/export
- ✅ Switch-case: Pattern matching (basic)
- ✅ Exception handling: try/catch/throw/finally
- ✅ Lambda functions: Anonymous functions
- ✅ Generics: Type parameters
- ✅ Iterators & Generators: for-in loops, yield
- ✅ Null safety: ??, ?., null literal
- ✅ Operator overloading: Custom operators for structs
- ✅ Pattern matching: match/case with ranges
- ✅ State management: global state variables

**I/O & System:**
- ✅ File I/O: read_file(), write_file()
- ✅ String operations: concat, compare, methods
- ✅ Incremental compilation: Module caching (10-15x speedup)
- ✅ Error handling: Colored messages, typo detection

### 🚧 In Progress (Phase 12)

**TTO→STO Refactoring** (Naming consistency)
- ✅ Part 1: Documentation update (YZ_49)
- ✅ Part 2: Runtime library rename (YZ_50)
- ✅ Part 3: Compiler code update (YZ_51) - 453 references, 58 files
- ⏳ Part 4: Comprehensive testing (quick validation done, full suite pending)
- ⏳ Part 5: Final cleanup & migration guide

**Why Critical:** Must complete before Stage 1 (self-hosting)

### 📊 Completion Status
```
Core Language:     ████████████████████ 100%
Module System:     ███████████████████░  95%
Phase 12:          ████████████████░░░░  80% (Parts 1-3 done)
Self-Hosting:      ████░░░░░░░░░░░░░░░░  20%
```

### Backups (Safety Net)
- ✅ `melp_yedek_stage0_phase11_core_20251212` - Main branch (commit 8040c5e)
- ✅ `melp_yedek_stage0_phase11_core_20251212_2` - With YZ_47/48 changes

---

## 3. YZ Workflow (CRITICAL!)

### ⚠️ MANDATORY GIT WORKFLOW

**Every AI session MUST follow this:**

```bash
# 1. Create numbered branch
git checkout -b feature-name_YZ_XX
# Example: git checkout -b sto-refactoring_YZ_52

# 2. Work and commit
git add .
git commit -m "YZ_XX: Clear description of changes"

# 3. Push to remote
git push origin feature-name_YZ_XX

# 4. ⚠️ STOP! DO NOT MERGE!
# Human will review and merge
```

### Why This Matters
- ✅ Each AI's work is tracked separately
- ✅ Easy rollback if needed
- ✅ Clear audit trail
- ✅ No accidental overwrites
- ✅ Human reviews all changes

### YZ Numbering
```
YZ_01 = First AI session
YZ_02 = Second AI session
...
YZ_52 = Current (12 Dec 2025)
YZ_53 = Next session (you!)
```

### Session Documentation
Create `/YZ/YZ_XX.md` with:
```markdown
# YZ_XX Session Report

**Date:** [Date]
**Duration:** [Hours]
**Task:** [What you did]

## Changes Made
- File 1: Description
- File 2: Description

## Test Results
- Test 1: ✅ Pass
- Test 2: ✅ Pass

## Next Steps
- [ ] Task for next session
```

---

## 4. STO (Smart Type Optimization)

### Philosophy

**User Perspective:**
```mlp
numeric x = 42           -- Simple!
numeric y = 3.14         -- Simple!
numeric z = 10 ^ 1000    -- Still simple!

string s = "Ali"         -- Simple!
string t = read_file("book.txt")  -- Still simple!
```

**Compiler Perspective (Hidden from User):**
```
numeric x = 42        → int64 (register, ⚡ fastest)
numeric y = 3.14      → double (xmm register, ⚡ fast)
numeric z = 10^1000   → BigDecimal (heap, 🐢 safe)

string s = "Ali"      → SSO/stack (≤23 bytes, ⚡ fastest)
string t = long...    → heap pointer (🔄 normal)
```

### Implementation Status

**Runtime (Complete):**
- ✅ `sto_print_int64()`, `sto_print_double()`
- ✅ `sto_safe_add_i64()`, overflow detection
- ✅ `sto_sso_*()` - Small String Optimization
- ✅ `sto_array_alloc()`, `sto_list_alloc()`, `sto_tuple_alloc()`

**Compiler (Complete):**
- ✅ Type tracking: `is_numeric` flag (1 bit per variable)
- ✅ Codegen: Type-aware assembly generation
- ✅ Module: `modules/runtime_sto/` (renamed from tto)

**Phase 12 Status:**
- ✅ Runtime: All TTO→STO renamed (YZ_50)
- ✅ Compiler: All 453 references updated (YZ_51)
- ⏳ Testing: Comprehensive validation needed

### Key Principle
**"2 types, 1 bit, simple!"** - Keep tracking minimal, optimization automatic.

---

## 5. Language Syntax Quick Reference

### Variable Declaration
```mlp
numeric x = 10
string name = "Ali"
boolean flag = true
state numeric counter = 0    -- Global state (opt-in)
```

### Functions
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function
```

### Control Flow
```mlp
-- If-else
if condition then
    print("yes")
else if other_condition then
    print("maybe")
else
    print("no")
end if

-- While
while x < 10
    x = x + 1
end while

-- For
for i = 0 to 10
    print(i)
end for

-- For-in (iterator)
for item in collection
    print(item)
end for
```

### Collections
```mlp
-- Array (homogeneous, mutable)
numeric[] arr = [1, 2, 3]
arr[0] = 100

-- List (heterogeneous, mutable, semicolon + trailing required!)
list() = (1; "hello"; true;)   -- ✅ Correct
list() = (1, "hello", true)    -- ❌ Wrong (comma)
list() = (1; 2; 3)             -- ❌ Wrong (no trailing semicolon)

-- Tuple (heterogeneous, immutable)
tuple<> = <10, "point", true>
```

### Advanced Features
```mlp
-- Exception handling
try
    throw RuntimeError("Oops!")
catch RuntimeError e
    print("Caught runtime error")
finally
    print("Cleanup")
end try

-- Pattern matching
match value
    case 1 => print(100)
    case 2, 3, 4 => print(200)
    case 5 to 10 => print(300)
    case _ => print(999)
end match

-- Null safety
numeric result = maybeNull ?? defaultValue
string? name = person?.name
```

### Comments
```mlp
-- Single line comment

---
Multi-line comment
Can span multiple lines
---
```

**⚠️ NOT `//` or `#` - Only `--` and `---`!**

---

## 6. Development Guidelines

### Before You Start

**1. Read These (Priority Order):**
```
1. MELP_REFERENCE.md      ← This file (you're here!)
2. NEXT_AI_START_HERE.md  ← Latest session status
3. ARCHITECTURE.md        ← Modular architecture rules
4. TODO.md                ← Find your next task
```

**2. Check Git Status:**
```bash
git status                # Should be clean
git log --oneline -5      # See recent changes
```

**3. Understand Current Task:**
- What phase are we in?
- What was completed last session?
- What's the next priority?

### Modular Architecture Rules (CRITICAL!)

**Rule #0: NO CENTRAL FILES**
- ❌ No main.c with everything
- ❌ No orchestrator.c
- ❌ No helpers.c with utilities
- ✅ Each module is standalone
- ✅ Modules communicate via JSON/pipes

**Rule #1: Module Independence**
```
modules/arithmetic/     ← Can compile standalone
modules/comparison/     ← Can compile standalone
modules/functions/      ← Can compile standalone
```

**Rule #2: Communication**
```bash
# ✅ CORRECT: Unix pipes
echo '{"type":"add","a":5,"b":3}' | ./modules/arithmetic/arithmetic

# ❌ WRONG: C function calls
#include "modules/arithmetic/arithmetic.h"
result = arithmetic_add(5, 3);  // NO!
```

**Rule #3: File Size Limits**
- Max 300 lines per file
- If larger, split into modules
- Exception: Generated code, data files

### Testing Guidelines

**Always Test:**
```bash
# After code changes
cd compiler/stage0
make clean && make

# Run basic tests
./melpc test_basic.mlp -o test_basic
./test_basic
echo $?  # Check exit code

# Run specific feature test
./melpc test_for_simple.mlp -o test_for
./test_for
echo $?  # Should match expected
```

**Test Files Location:**
```
compiler/stage0/modules/functions/
├── test_basic.mlp
├── test_for_simple.mlp
├── test_basic_println.mlp
└── ...
```

---

## 7. Common Tasks

### Task 1: Adding a New Feature

**Example: Add new string method `replace()`**

1. **Runtime Function** (`runtime/stdlib/mlp_stdlib.c`):
```c
char* mlp_string_replace(const char* str, const char* old, const char* new) {
    // Implementation
}
```

2. **Register Builtin** (`modules/functions/functions.c`):
```c
int function_is_builtin(const char* name) {
    if (strcmp(name, "replace") == 0) return 1;
    // ...
}
```

3. **Parser Support** (`modules/arithmetic/arithmetic_parser.c`):
```c
// Add function call parsing if needed
```

4. **Codegen** (`modules/functions/functions_codegen.c`):
```c
if (strcmp(func_name, "replace") == 0) {
    fprintf(output, "    call mlp_string_replace\n");
}
```

5. **Test**:
```mlp
function main() returns numeric
    string result = replace("hello world", "world", "MELP")
    println(result)  -- Should print: hello MELP
    return 0
end function
```

### Task 2: Fixing a Bug

1. **Reproduce:**
```bash
# Create minimal test case
cat > test_bug.mlp << 'EOF'
function main() returns numeric
    # Code that triggers bug
    return 0
end function
EOF

./melpc test_bug.mlp -o test_bug
./test_bug
```

2. **Debug:**
```bash
# Check lexer output
./melpc test_bug.mlp --debug-tokens

# Check parser output
./melpc test_bug.mlp --debug-ast

# Check assembly
cat test_bug.s
```

3. **Fix & Test:**
```bash
# Edit the relevant module
vim modules/[module]/[file].c

# Rebuild
make clean && make

# Test again
./melpc test_bug.mlp -o test_bug
./test_bug
```

4. **Document:**
```markdown
# In YZ/YZ_XX.md
## Bug Fix: [Description]
**Issue:** [What was wrong]
**Root Cause:** [Why it happened]
**Solution:** [What was changed]
**Test:** test_bug.mlp ✅
```

### Task 3: Refactoring (Like Phase 12)

**Current Phase 12 Status:**
- ✅ Part 1: Docs updated (TTO→STO)
- ✅ Part 2: Runtime renamed
- ✅ Part 3: Compiler code updated (453 references)
- ⏳ Part 4: Testing needed
- ⏳ Part 5: Cleanup needed

**To Complete Part 4 (Testing):**
```bash
# 1. Build system
cd compiler/stage0
make clean && make

# 2. Run existing tests
./melpc test_for_simpler.mlp -o test_for
./test_for
echo $?  # Should be 6

./melpc test_sto_final.mlp -o test_sto
./test_sto
echo $?  # Should be 150

# 3. Test functions + println
./melpc test_basic_println.mlp -o test_println
./test_println
# Should print: 30

# 4. Document results in YZ_52.md
```

---

## 8. Troubleshooting

### Problem: Compilation Fails

**Check:**
```bash
# 1. Syntax error in .mlp file?
cat problematic_file.mlp

# 2. Missing runtime functions?
nm runtime/sto_runtime.o | grep mlp_

# 3. Linker errors?
make clean && make 2>&1 | tee build.log
grep "undefined reference" build.log
```

**Solutions:**
- Missing function → Add to runtime
- Syntax error → Check against kurallar_kitabı.md syntax
- Linker error → Check Makefile linker order

### Problem: Test Fails

**Debug:**
```bash
# 1. Check exit code
./test_program
echo $?  # What did it return?

# 2. Check assembly
cat test_program.s | less

# 3. Add debug prints
function main() returns numeric
    println(42)  -- Debug checkpoint
    # ... rest of code
end function
```

### Problem: Git Conflicts

**Prevention:**
```bash
# Always work on your own branch
git checkout -b my-feature_YZ_XX

# Never merge to main
# Human will handle merges
```

**If you see conflict:**
```bash
# 1. Stop work
# 2. Tell user: "Git conflict detected, human intervention needed"
# 3. Commit your current work to your branch
git add .
git commit -m "YZ_XX: Work in progress before conflict"
git push origin my-feature_YZ_XX
```

---

## 9. Quick Command Reference

### Build Commands
```bash
cd compiler/stage0
make clean           # Clean build artifacts
make                 # Build compiler
make check-architecture  # Validate architecture rules
```

### Test Commands
```bash
./melpc program.mlp -o program     # Compile
./program                          # Run
echo $?                            # Check exit code

./melpc program.mlp --debug-tokens # Debug lexer
./melpc program.mlp --debug-ast    # Debug parser
```

### Git Commands
```bash
git checkout -b feature_YZ_XX      # Create branch
git add .                          # Stage changes
git commit -m "YZ_XX: Message"     # Commit
git push origin feature_YZ_XX      # Push (DO NOT MERGE!)
```

### File Navigation
```bash
# View module structure
ls -la compiler/stage0/modules/

# Find all .mlp files
find . -name "*.mlp" -type f

# Search for function definition
grep -r "mlp_string_replace" compiler/

# Check runtime functions
nm runtime/sto_runtime.o | grep mlp_
```

---

## 10. Frequently Asked Questions

### Q: What's the difference between TTO and STO?
**A:** Same thing, renamed!
- **TTO** = Transparent Type Optimization (old name)
- **STO** = Smart Type Optimization (new name, more descriptive)
- Phase 12 renamed everything for consistency (Parts 1-3 complete)

### Q: Can I modify central files like main.c?
**A:** NO! There are no central files.
- Architecture Rule #1: NO CENTRAL FILES
- All work must be in modules/
- Modules communicate via JSON/pipes

### Q: Should I merge my branch to main?
**A:** NEVER!
- AI creates branch
- AI commits and pushes
- Human reviews and merges
- This prevents accidents

### Q: Where do test files go?
**A:** 
- Feature-specific tests: `modules/[feature]/test_*.mlp`
- General tests: `compiler/stage0/test_*.mlp`
- All tests should be committed

### Q: What's the difference between numeric and string?
**A:**
- User sees: `numeric` (all numbers), `string` (all text)
- Compiler sees: int64/double/BigDecimal for numeric, SSO/heap for string
- This is STO in action!

### Q: How do I know what to work on next?
**A:**
1. Read `NEXT_AI_START_HERE.md` - Latest status
2. Read `TODO.md` - Phase details
3. Ask user: "What should I prioritize?"

### Q: Can I create new .md documentation files?
**A:** Only if necessary.
- Prefer updating existing files
- This file (MELP_REFERENCE.md) should have most info
- Session-specific details go in `/YZ/YZ_XX.md`

---

## 11. Success Metrics

### How to Know You Succeeded

**After Your Session:**

✅ **Code Quality:**
- [ ] All tests pass
- [ ] No compilation warnings
- [ ] Architecture rules followed (no central files)
- [ ] Code is modular (< 300 lines per file)

✅ **Documentation:**
- [ ] YZ_XX.md created with session details
- [ ] TODO.md updated with progress
- [ ] NEXT_AI_START_HERE.md updated for next session

✅ **Git Workflow:**
- [ ] Branch created: `feature_YZ_XX`
- [ ] Changes committed with clear messages
- [ ] Pushed to remote
- [ ] Did NOT merge to main

✅ **Testing:**
- [ ] New features have test files
- [ ] Existing tests still pass
- [ ] Exit codes match expected values

---

## 12. Emergency Contacts

**If Something Goes Wrong:**

### Critical Issues:
1. **Build completely broken:** Restore from backup branch
2. **Git disaster:** Tell user, don't try to fix
3. **Architecture violation:** Revert changes, follow rules

### Where to Get Help:
- **ARCHITECTURE.md** - Architecture rules and enforcement
- **TODO.md** - Detailed phase information
- **kurallar_kitabı.md** - Comprehensive syntax reference (Turkish)
- **User** - When in doubt, ask!

### Backup Branches (Last Resort):
```bash
# If everything is broken, restore from:
git checkout melp_yedek_stage0_phase11_core_20251212

# This is clean state with println() working
# Phase 11 complete, before Phase 12 refactoring
```

---

## 13. Project Vision (Inspiration!)

### What Makes MELP Special?

**1. Melting Pot Philosophy**
- Write in ANY language (Turkish, English, Hindi, Chinese...)
- Use ANY syntax (Python-like, C-like, your own style)
- Same underlying semantics

**2. STO = Simplicity + Performance**
- User: Simple (`numeric`, `string`)
- Performance: Optimized (registers, SIMD, BigDecimal)
- Best of both worlds!

**3. Stateless by Default**
- Predictable behavior
- Easy to test
- Parallel-friendly
- Opt-in state when needed

**4. Self-Hosting Goal**
- MELP compiler written in MELP
- Proves language is mature
- Dog-fooding forces good design

### Long-Term Vision

```
2025 Q4: Stage 0 Complete ✅ (almost there!)
2026 Q1: Self-hosting (MELP compiler in MELP)
2026 Q2: Multi-syntax active (diller.json working)
2026 Q3: LLVM backend (WebAssembly, ARM)
2026+:  MELP Ecosystem (packages, IDE, community)
```

---

## 14. Final Checklist (Before You Finish)

### When Ending Your Session:

- [ ] All code tested and working
- [ ] Git branch pushed (NOT merged!)
- [ ] YZ_XX.md created in `/YZ/` folder
- [ ] TODO.md progress updated
- [ ] NEXT_AI_START_HERE.md updated with:
  - [ ] What you completed
  - [ ] What's next for next AI
  - [ ] Any blockers or issues
- [ ] User knows what was done
- [ ] You feel good about handoff! 😊

---

## 15. Useful Links & Resources

### GitHub
- **Main Repo:** https://github.com/MELP-Lang/MELP
- **Backup Branch 1:** melp_yedek_stage0_phase11_core_20251212
- **Backup Branch 2:** melp_yedek_stage0_phase11_core_20251212_2

### Key Documentation Files
- `/mnt/project/MELP_REFERENCE.md` ← This file
- `/mnt/project/ARCHITECTURE.md` ← Rules
- `/mnt/project/TODO.md` ← Tasks
- `/mnt/project/NEXT_AI_START_HERE.md` ← Status

### Example Test Files
```
compiler/stage0/modules/functions/test_basic_println.mlp
compiler/stage0/test_for_simpler.mlp
compiler/stage0/test_sto_final.mlp
```

---

## Appendix A: File Structure Map

```
MELP/
├── compiler/stage0/              ← Main compiler location
│   ├── melpc                     ← Compiler binary
│   ├── modules/                  ← 26 feature modules
│   │   ├── arithmetic/
│   │   ├── comparison/
│   │   ├── functions/
│   │   ├── runtime_sto/          ← STO runtime module
│   │   └── ...
│   ├── Makefile                  ← Build system
│   └── test_*.mlp                ← Test files
│
├── runtime/                      ← C runtime libraries
│   ├── sto/                      ← STO runtime (renamed from tto)
│   │   ├── sto_runtime.c
│   │   ├── sto_runtime.h
│   │   └── ...
│   ├── memory/                   ← Memory management
│   └── stdlib/                   ← Standard library
│       └── mlp_stdlib.c
│
├── modules/                      ← Language modules
│   ├── core/
│   ├── advanced/
│   └── experimental/
│
├── YZ/                          ← AI session documentation
│   ├── YZ_HISTORY.md            ← Summary of all sessions
│   ├── YZ_01.md                 ← First session
│   ├── ...
│   ├── YZ_51.md                 ← Compiler refactoring
│   └── YZ_52.md                 ← Latest session (handoff)
│
├── docs/                        ← English documentation
├── docs_tr/                     ← Turkish documentation
├── examples/                    ← Example programs
├── tests/                       ← Test suite
│
└── [Root documentation files]
    ├── MELP_REFERENCE.md        ← This file
    ├── ARCHITECTURE.md
    ├── TODO.md
    ├── NEXT_AI_START_HERE.md
    ├── MELP_VISION.md
    ├── README.md
    └── kurallar_kitabı.md
```

---

## Appendix B: Token Types Reference

```
KEYWORDS:
function, end, if, then, else, while, for, return, numeric, string, boolean,
struct, import, export, try, catch, throw, finally, match, case, state, shared

MERGED KEYWORDS (Lexer combines these):
end_if, end_while, end_for, end_function, end_struct,
exit_for, exit_while, exit_if, exit_function

OPERATORS:
+, -, *, /, %, ==, !=, <, <=, >, >=, and, or, not, ??, ?.

LITERALS:
42 (numeric), "hello" (string), true/false (boolean), null
```

---

## Appendix C: Runtime Function Reference

### Numeric Operations
```c
void sto_print_int64(int64_t value);
void sto_print_double(double value);
int64_t sto_safe_add_i64(int64_t a, int64_t b);
```

### String Operations
```c
char* mlp_string_concat(const char* a, const char* b);
int mlp_string_compare(const char* a, const char* b);
size_t mlp_string_length(const char* str);
char* mlp_string_substring(const char* str, size_t start, size_t len);
```

### Collection Operations
```c
void* sto_array_alloc(size_t element_size, size_t count);
void* sto_list_alloc(void);
void* sto_tuple_alloc(size_t count);
```

### I/O Operations
```c
void mlp_println(const char* str);
void mlp_print(const char* str);
char* mlp_read_file(const char* path);
int mlp_write_file(const char* path, const char* content);
```

---

**Remember:** This is your single source of truth. Everything you need to know is here or referenced from here.

**Good luck, YZ_53! 🚀**

---

*This document is maintained by the MELP project. Last reviewed by YZ_52 on 12 Dec 2025.*