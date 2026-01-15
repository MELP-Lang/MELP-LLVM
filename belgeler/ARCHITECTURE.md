# MELP Architecture Rules - IMMUTABLE

## 🚨 CRITICAL: AI AGENT WORKFLOW (NEW - 9 Aralık 2025) 🚨

**EVERY AI AGENT MUST:**

1. **Read this file FIRST** before making any changes
2. **Create a numbered branch**: `git checkout -b feature-name_YZ_XX`
   - YZ_01 = First AI agent (this session)
   - YZ_02 = Second AI agent
   - YZ_03 = Third AI agent, etc.
3. **Work on your branch**, make commits with clear messages
4. **Before finishing**: `git push origin feature-name_YZ_XX`
5. **Leave a handoff note** in `/NEXT_AI_START_HERE.md`
6. **⚠️ NEVER merge or create pull requests** - Human will review and merge

**Example:**
```bash
git checkout -b sto-cleanup_YZ_01
# ... work ...
git add .
git commit -m "STO: Remove legacy int/float API, pure numeric only"
git push origin sto-cleanup_YZ_01
```

**WHY:** 
- Track each AI's contribution clearly
- Prevent overwriting previous AI's work
- Easy rollback if needed
- Clear audit trail
- **Human reviews and merges** - AI never merges to main

**CURRENT AI:** YZ_49 (Phase 12: STO Refactoring - Documentation)
**PREVIOUS AI:** YZ_48 (12 Aralık 2025 - println() Complete) ✅ COMPLETED

---

## ⚠️ FOR AI AGENTS: READ THIS FIRST ⚠️

Previous AI agents violated these rules and created a 736-line monolithic `main.c`.
**DO NOT REPEAT THIS MISTAKE.**

---

## Rule #0: STO (Smart Type Optimization) - CORE PRINCIPLE

### 🎯 The Philosophy

**User sees only 2 types:**
- `numeric` - All numbers (integers, decimals, big numbers)
- `text` - All strings (short, long, constant)

**Compiler optimizes behind the scenes.** User never knows, never cares.

### 📊 Why This Approach?

**Key Insights:**
1. In other languages, BigDecimal is actually string-based
2. MLP has only `numeric` and `string` → essentially one concept: "data"
3. "Ali" and a 10,000-page book shouldn't use same memory strategy
4. User doesn't see pragmatic layer → backend optimization possible

### 🔄 How TTO Works

**User writes:**
```mlp
numeric small = 42
numeric decimal = 3.14159
numeric huge = 10 ^ 1000

text short = "Ali"
text long = read_file("book.txt")
```

**Compiler decides internally:**

| User Type | Value | Internal Type | Location | Speed |
|-----------|-------|---------------|----------|-------|
| `numeric` | 42 | int64 | register/stack | ⚡ Fastest |
| `numeric` | 3.14 | double | xmm register | ⚡ Fast |
| `numeric` | 10^100 | BigDecimal | heap | 🐢 Safe |
| `text` | "Ali" | SSO (inline) | stack | ⚡ Fastest |
| `text` | long... | heap pointer | heap | 🔄 Normal |
| `text` | constant | .rodata | readonly | ⚡ Shared |

### ⚠️ CRITICAL FOR AI AGENTS

**DO:**
- ✅ Track minimal type info (1 bit: `is_numeric` flag)
- ✅ Read `temp/kurallar_kitabı.md` TTO section for full details
- ✅ Keep user API simple (only `numeric` and `string`)

**DON'T:**
- ❌ Create complex type enums (VarType with 10 variants)
- ❌ Expose internal types to user
- ❌ Add type tracking beyond what's needed for codegen

**Example (YZ_05):**
```c
// ✅ CORRECT: Simple flag
typedef struct LocalVariable {
    char* name;
    int stack_offset;
    int is_numeric;    // 1=numeric, 0=string (2 types, 1 bit!)
    struct LocalVariable* next;
} LocalVariable;

// ❌ WRONG: Complex enum
typedef enum { VAR_INT, VAR_FLOAT, VAR_BIGDEC, VAR_STRING, ... } VarType;
```

**Key Principle:** "2 types, 1 bit, simple!" - Keep it minimal.

### 📚 Full TTO Documentation

For complete implementation details, algorithms, and memory strategies:
→ See **`temp/kurallar_kitabı.md`** Section 4: "Transparent Type Optimization (TTO)"

---

## Rule #0.5: Stateless Architecture

### 🎯 MELP is Stateless by Default

**Core Philosophy:** MELP is a **stateless language by default**. All variables are function-scoped (local) and do not persist between function calls.

#### 1. Language Level: Stateless by Default
- **No global variables** - All variables are local to functions
- **No implicit state** - Functions are pure by default
- **Explicit state when needed** - Use `import state` module for persistence
- **Predictable behavior** - Same inputs always produce same outputs

```mlp
-- ✅ Default: Stateless (no state persists)
function counter() returns numeric
    numeric x = 0    -- Always starts at 0
    x = x + 1
    return x         -- Always returns 1
end function

-- ✅ Explicit state: Import state module when needed
import state
state.set("counter", 0)

function counter_with_state() returns numeric
    numeric x = state.get("counter")
    x = x + 1
    state.set("counter", x)
    return x         -- Returns 1, 2, 3, 4...
end function
```

#### 2. Compiler Level: Stateless Implementation
- Parser functions are stateless (`*_stateless()` pattern)
- No global state between modules
- Token ownership/borrowing pattern for memory management
- Pipeline: Lexer → Parser → Codegen are separate, independent stages

**Why Stateless by Default?**
- Easier to test and debug
- Predictable, deterministic behavior
- Better for parallel execution
- State is opt-in, not opt-out

---

## Rule #0.6: Design Philosophy

### 🎯 MELP = VB.NET Readability + Python Simplicity

MELP combines the best of both worlds:

| Decision | MELP Choice | Reason |
|----------|-------------|--------|
| `=` vs `==` | `=` assignment, `==` comparison | Explicit, prevents bugs (unlike VB.NET's context-dependent `=`) |
| `function` keyword | Single keyword, no `Sub` | Python-like simplicity, no void/return distinction needed |
| Variable declaration | `numeric x = 5` (no `Dim`) | Type-first is clear enough, `Dim` is verbose |
| String type | `string` keyword | Universal term, understood in all languages |
| Collection iteration | `for each item in collection` | VB.NET compatibility, readable |
| Block endings | `end if`, `end while`, `end function` | VB.NET style, self-documenting |

### ⚠️ Design Principles

1. **Readability over brevity** - `end if` not `}`
2. **Explicit over implicit** - `==` for comparison, not context-dependent `=`
3. **One way to do things** - Single `function` keyword, no `Sub`/`Function` split
4. **Type-first declarations** - `numeric x`, not `Dim x As Integer`

---

## Rule #1: NO CENTRAL FILES (Radical Modularity)

**WHY:** Central files (main.c, orchestrator.c, helpers.c) attract AI agents to write monolithic code.

**ENFORCED BY:** File removal + Makefile (`make check-architecture`)

**CURRENT STATUS:** 
- ✅ All central files backed up to `temp/yedek_merkezi_dosyalar/`
- ✅ AI agents FORCED to work inside `modules/` only

**VIOLATION CONSEQUENCE:** No central files exist = Cannot violate!

---

## Rule #2: Modules Communicate via JSON/Pipes ONLY

**FORBIDDEN:**
```c
// In main.c or orchestrator.c:
#include "modules/arithmetic/arithmetic_parser.h"  // ❌ NO!
result = arithmetic_parse(tokens);  // ❌ NO! (direct function call)
```

**ALLOWED:**
```bash
# Unix pipes (stdin/stdout):
echo '{"token":"NUMERIC"}' | ./modules/arithmetic/arithmetic

# JSON files:
./lexer < input.mlp > tokens.json
./parser < tokens.json > ast.json
```

**WHY:** 
- No C-level coupling
- Language-agnostic (can rewrite in MELP)
- Testable with simple text files

**ENFORCED BY:** No central files = No #include possible!

---

## Rule #3: Each Module is Standalone

**REQUIRED:** Every module must be compilable as standalone binary:

```
modules/arithmetic/arithmetic_compiler  (standalone)
modules/comparison/comparison_compiler  (standalone)
modules/tto/tto_analyzer               (standalone)
```

**WHY:** 
- Testable independently
- Replaceable without breaking system
- Self-hosting ready (can rewrite in MELP)

**COMMUNICATION:** JSON/stdin/stdout only

---

## Rule #4: Pipeline Architecture (Unix Philosophy)

**CORRECT FLOW:**
```
melp_lexer (standalone) 
    → tokens.json 
    → melp_parser (standalone)
    → ast.json
    → melp_tto (standalone)
    → ast_tto.json
    → melp_codegen (standalone)
    → output.s
```

**EACH STEP:**
- Separate binary
- Reads from stdin/file
- Writes to stdout/file
- No direct function calls between modules

---

## Rule #5: MELP Value Proposition

**Core Principle:** Transparent optimization without user complexity

**User Experience:**
```mlp
numeric x = 3        # Simple syntax
text msg = "Hello"   # No type annotations needed
```

**Behind The Scenes:**
- Compiler automatically chooses optimal representation
- Small values → registers (fast)
- Large values → heap with safety (correct)
- User never sees complexity

**See Rule #0 (TTO) for full architecture details.**

---

## Rule #6: LLVM Backend (Phase 13.5) 🆕

### 🎯 Compiler Backend Architecture

**Design Decision:** Two-backend system for flexibility and portability

**Backend Options:**
1. **Assembly Backend** (x86-64) - Direct assembly generation
2. **LLVM IR Backend** (Portable) - LLVM IR text format (`.ll`)

**Selection:** Via `--backend` flag (default: assembly)

```bash
# Assembly backend (original, x86-64 only)
./functions_compiler -c input.mlp output.s

# LLVM backend (portable, multi-platform)
./functions_compiler -c --backend=llvm input.mlp output.ll
clang output.ll -o program
```

### 📁 LLVM Backend Module Structure

```
compiler/stage0/modules/
├── llvm_backend/           # Core LLVM IR emission
│   ├── llvm_backend.h      # API definitions (174 lines)
│   ├── llvm_backend.c      # IR generation (368 lines)
│   ├── Makefile           # Independent build system
│   └── test_llvm_backend.c # Unit tests
│
└── functions/              # Integration layer
    ├── functions_codegen_llvm.h   # LLVM codegen wrapper
    ├── functions_codegen_llvm.c   # Statement/expression IR gen (535 lines)
    └── functions_standalone.c     # CLI with --backend flag
```

### 🔧 LLVM Backend API Design

**Principle:** Clean abstraction over LLVM IR text format

```c
// Context management
LLVMContext* ctx = llvm_context_create(output_file);

// Module-level
llvm_emit_module_header(ctx);
llvm_emit_function_start(ctx, "main", params, count);
llvm_emit_function_entry(ctx);

// Variables
LLVMValue* ptr = llvm_emit_alloca(ctx, "x");
LLVMValue* val = llvm_const_i64(42);
llvm_emit_store(ctx, val, ptr);
LLVMValue* loaded = llvm_emit_load(ctx, ptr);

// Arithmetic
LLVMValue* sum = llvm_emit_add(ctx, left, right);
LLVMValue* diff = llvm_emit_sub(ctx, left, right);

// Logical (Boolean)
LLVMValue* and_result = llvm_emit_and(ctx, a, b);
LLVMValue* or_result = llvm_emit_or(ctx, a, b);

// Comparison
LLVMValue* cmp = llvm_emit_icmp(ctx, "sgt", a, b);  // signed greater than

// Control flow
llvm_emit_br_cond(ctx, condition, "then_label", "else_label");
llvm_emit_label(ctx, "then_label");
llvm_emit_br(ctx, "after_if");

// Function end
llvm_emit_return(ctx, result);
llvm_emit_function_end(ctx);
llvm_emit_module_footer(ctx);
```

### ✅ Implemented Features (Phase 13.5)

**Part 1-4: Core Infrastructure** (YZ_57)
- ✅ LLVM backend module (llvm_backend.c/h)
- ✅ Integration with functions_compiler
- ✅ `--backend=llvm` CLI flag
- ✅ Basic arithmetic operations (+, -, *, /)
- ✅ Function calls and parameters
- ✅ Variable declarations and assignments

**Part 5.1: Control Flow** (YZ_58)
- ✅ If/else statements with conditional branches
- ✅ Comparison operators (>, <, ==, !=, >=, <=)
- ✅ Label generation and branching
- ✅ Assignment statements

**Part 5.2: Boolean Operations** (YZ_59)
- ✅ Boolean literals (true=1, false=0)
- ✅ Logical AND operation (`and i64`)
- ✅ Logical OR operation (`or i64`)
- ✅ Boolean expression evaluation

### 📊 Test Results

**All tests passing (8/8):**
```bash
✅ test_basic.mlp          # 10 + 20 = Exit 30
✅ test_sanity.mlp         # return 100 = Exit 100
✅ test_llvm_functions.mlp # add(15, 27) = Exit 42
✅ test_llvm_if.mlp        # if 15 > 10 then 1 else 0 = Exit 1
✅ test_llvm_assign.mlp    # x=30, y=25 = Exit 25
✅ test_boolean_and.mlp    # true and false = Exit 0
✅ test_boolean_and_true.mlp # true and true = Exit 1
✅ test_boolean_or.mlp     # true or false = Exit 1
```

### ⚠️ Known Limitations

1. **While/For Loops:** Codegen ready, parser doesn't support them yet
2. **Standard Library:** Using printf temporarily (TODO: mlp_println_numeric)
3. **Optimization:** No LLVM opt passes yet (focus on correctness)
4. **String Operations:** Not implemented in LLVM backend

### 🔜 Next Steps (Phase 14)

- Parser enhancement for while/for loops
- Standard library integration (mlp_println_numeric)
- Self-hosting lexer/parser with LLVM backend
- Performance benchmarking vs Assembly backend

### 📚 Documentation

- **LLVM IR Guide:** `docs/LLVM_IR_GUIDE.md` (753 lines)
- **Examples:** `examples/llvm/*.ll` (working IR samples)
- **Session Reports:** `YZ/YZ_57.md`, `YZ/YZ_58.md`, `YZ/YZ_59.md`

---

## Current Architecture Status

### ✅ RESOLVED: TTO Duplicate Definition (9 Aralık 2025 - YZ_02)
- **Problem:** `tto_infer_numeric_type()` defined in both compiler and runtime
- **Action:** Renamed compiler functions with `codegen_` prefix (namespace separation)
- **Result:** Clean separation - compiler uses `codegen_tto_*`, runtime uses `tto_*`
- **Status:** ✅ NO HACKS (rejected `--allow-multiple-definition`)

### ✅ RESOLVED: Stdlib Integration (9 Aralık 2025 - YZ_02)
- **Achievement:** `println()` working in compiled programs! 🎉
- **Action:** Proper linker order - stdlib before tto_runtime
- **Result:** Function calls + stdlib = MVC ~90% complete
- **Status:** ✅ MODULAR (no central files created)

### ✅ RESOLVED: Central Files Removed! (7 Aralık 2025)
- **Action:** All central files moved to `temp/yedek_merkezi_dosyalar/`
- **Backed up:**
  - main.c, orchestrator.c, helpers.c (orchestration files)
  - lexer.c, lexer.h (will move to modules/lexer/)
  - cli/, pipeline/, error/ (will convert to modules)
- **Result:** AI agents CANNOT create monolithic code (no place to put it!)
- **Status:** ✅ RADICAL SOLUTION ACTIVE

### 📋 TODO: Convert Backed-up Files to Modules
1. **modules/lexer/** - Move lexer.c + add main.c (standalone)
2. **modules/parser/** - Create new parser module
3. **modules/codegen/** - Create new codegen module
4. **modules/cli/** - Convert cli/ directory
5. **Delete:** pipeline/ (replaced by Unix pipes)

### ✅ Problem: TTO runtime
- **Status:** ✅ RESOLVED (libtto_runtime.a linked)
- **Date:** 7 Aralık 2025

### ✅ Problem: Stdlib Integration
- **Status:** ✅ RESOLVED (println working, proper namespace separation)
- **Date:** 9 Aralık 2025 (YZ_02)

---

## How to Avoid Architecture Decay

### For Current AI Agent:
1. ✅ Implement TTO runtime FIRST
2. ✅ Keep main.c < 300 lines
3. ✅ Use existing modules, don't merge them
4. ✅ Run `make check-architecture` before committing

### For Future AI Agents:
1. 📖 Read this file BEFORE coding
2. 🛡️ Run checks: `make check-architecture`
3. 🧪 Test modules independently
4. ❌ NEVER merge modules into main.c "for simplicity"

---

## Enforcement

**Automatic Checks:**
```bash
make check-architecture   # Runs all architecture checks
make check-main-size      # Checks main.c line count
make check-coupling       # Checks forbidden imports
```

**Manual Review:**
- Every PR reviewed for architecture compliance
- Violations = immediate rejection

---

## Success Metrics

**Stage 0 (Current - 9 Aralık 2025):**
- [x] TTO runtime implemented ✅ (YZ_01)
- [x] Stdlib integrated ✅ (YZ_02)
- [x] Functions module complete ✅ (YZ_02)
- [x] Variables module complete ✅ (YZ_02)
- [x] println() working ✅ (YZ_02)
- [ ] Array/string integration tests (YZ_03 - next)
- [ ] MVC 100% complete

**Stage 1 (Self-hosting):**
- [ ] Each module has MELP rewrite
- [ ] Pipeline fully JSON-based
- [ ] Zero coupling between modules

---

## Remember

> "Architecture is about the important stuff. Whatever that is."
> — Ralph Johnson

For MELP, the important stuff is:
1. **Modularity** (AI can manage small pieces)
2. **TTO** (Performance without complexity)
3. **Self-hosting** (MELP written in MELP)

If you violate these, you're breaking MELP's core vision.

---

## 📊 AI Agent Progress Log

### YZ_59 (13 Aralık 2025) - LLVM Boolean Operations ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Duration:** ~2 hours
- **Token Usage:** 56K / 1M (5.6%)
- **Achievements:**
  - ✅ Boolean literal support (true=1, false=0)
  - ✅ Logical AND operation (`and i64`)
  - ✅ Logical OR operation (`or i64`)
  - ✅ All 8 LLVM tests passing
- **Documentation:** Part of Phase 13.5 (YZ_57-YZ_59)
- **Status:** Phase 13.5 ~95% complete

### YZ_58 (13 Aralık 2025) - LLVM Control Flow ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Achievements:**
  - ✅ If/else statements with conditional branches
  - ✅ Comparison operators (>, <, ==, !=, >=, <=)
  - ✅ Assignment statements
  - ✅ Label generation and branching
- **Documentation:** `/YZ/YZ_58.md`
- **Status:** Control flow complete

### YZ_57 (12-13 Aralık 2025) - LLVM Backend Core ✅
- **Branch:** `phase13.5-llvm-backend` (active)
- **Duration:** ~6 hours
- **Achievements:**
  - ✅ LLVM backend module (llvm_backend.c/h)
  - ✅ Integration with functions_compiler
  - ✅ `--backend=llvm` flag working
  - ✅ Basic arithmetic and function calls
- **Documentation:** `/YZ/YZ_57.md`, `docs/LLVM_IR_GUIDE.md`
- **Status:** Foundation complete

### YZ_02 (9 Aralık 2025) - Stdlib Integration ✅
- **Branch:** `stdlib-integration_YZ_02` (pushed)
- **Duration:** ~2 hours
- **Token Usage:** 75K / 1M (7.5%)
- **Achievements:**
  - ✅ Fixed TTO duplicate definition (namespace separation)
  - ✅ Integrated stdlib (println working!)
  - ✅ Maintained modular architecture (no central files)
  - ✅ Tests passing: println(42), function+println
- **Documentation:** `/YZ/YZ_02.md`
- **Status:** MVC ~90% complete

### YZ_01 (7 Aralık 2025) - TTO Cleanup ✅
- **Branch:** `tto-cleanup_YZ_01` (assumed pushed)
- **Achievements:**
  - ✅ TTO runtime linked
  - ✅ Central files removed
  - ✅ Radical modularity enforced
- **Status:** Foundation set for stdlib integration

---

## 🏗️ COMPILER ARCHITECTURE OVERVIEW

### Stage 0: Bootstrap Compiler Architecture

**Philosophy:** MELP's Stage 0 compiler is written in C to bootstrap the language. It's designed for radical modularity and eventual self-hosting.

**Three-Stage Vision:**
- **Stage 0** (Current): C-based bootstrap compiler - Core features only
- **Stage 1**: MELP compiler written in MELP (self-hosting)
- **Stage 2**: Advanced features, multi-language support, full optimization

### Pipeline Architecture (Unix Philosophy)

```
┌─────────────┐
│  Source.mlp │
└──────┬──────┘
       │
       ▼
┌─────────────────────────────────────────────┐
│  LEXER (modules/lexer_mlp/)                 │
│  • Tokenization                             │
│  • UTF-8 support                            │
│  • String/numeric/keyword recognition       │
└──────┬──────────────────────────────────────┘
       │ tokens.json (or in-memory Token[])
       ▼
┌─────────────────────────────────────────────┐
│  PARSER (modules/parser_core/)              │
│  • Variable parser (modules/variable/)      │
│  • Function parser (modules/functions/)     │
│  • Statement parser (modules/statement/)    │
│  • Expression parser (modules/arithmetic/)  │
│  • Control flow (modules/control_flow/)     │
└──────┬──────────────────────────────────────┘
       │ AST (Abstract Syntax Tree)
       ▼
┌─────────────────────────────────────────────┐
│  STO ANALYZER (modules/sto_runtime/)        │
│  • Smart Type Optimization hints            │
│  • Numeric type inference (int64/double)    │
│  • String optimization (SSO/heap/constant)  │
└──────┬──────────────────────────────────────┘
       │ AST + STO annotations
       ▼
       ┌────────────────┐
       │ BACKEND SELECT │
       └────┬──────┬────┘
            │      │
    ┌───────┘      └───────┐
    ▼                      ▼
┌───────────────┐    ┌─────────────────┐
│ LLVM BACKEND  │    │ x86-64 BACKEND  │
│ (llvm_backend)│    │ (functions/     │
│               │    │  *_codegen.c)   │
│ • LLVM IR     │    │ • Direct ASM    │
│ • Portable    │    │ • Linux x86-64  │
│ • Multi-arch  │    │ • Native speed  │
└───────┬───────┘    └────────┬────────┘
        │                     │
        ▼                     ▼
    output.ll             output.s
        │                     │
        ▼                     ▼
    clang/llc             gcc/as
        │                     │
        └──────────┬──────────┘
                   ▼
            ┌──────────────┐
            │  Executable  │
            │  (linked w/  │
            │   stdlib +   │
            │   runtime)   │
            └──────────────┘
```

### Dual Backend Architecture

**Why Two Backends?**

1. **LLVM Backend** (Primary - Portable)
   - Cross-platform (x86-64, ARM, RISC-V, WebAssembly)
   - Optimization passes available
   - Industry-standard IR
   - Easier to maintain (high-level IR)
   - **Use case:** Production, multi-platform deployment

2. **x86-64 Assembly Backend** (Secondary - Educational)
   - Direct assembly generation
   - No dependencies (except GCC/AS)
   - Educational value (understand code generation)
   - Fast development iteration
   - **Use case:** Learning, debugging, Linux-only quick tests

**Selection Mechanism:**
```bash
# LLVM backend (default now, portable)
./functions_compiler --backend=llvm input.mlp output.ll
clang output.ll -o program

# x86-64 backend (direct assembly)
./functions_compiler --backend=x86-64 input.mlp output.s
gcc output.s -o program
```

### Module Structure

```
compiler/stage0/modules/
│
├── lexer_mlp/              # Tokenization (standalone)
│   ├── lexer_mlp.c/.h      # UTF-8 lexer
│   ├── Makefile            # Independent build
│   └── test_lexer.c        # Unit tests
│
├── parser_core/            # Base parsing utilities
│   ├── parser_utils.c/.h   # Token management
│   └── ast.h               # AST node definitions
│
├── variable/               # Variable declarations
│   ├── variable_parser.c   # `numeric x = 5` parsing
│   └── variable_codegen.c  # Stack allocation codegen
│
├── functions/              # Function definitions & calls
│   ├── functions_parser.c          # Function syntax parsing
│   ├── functions_codegen.c         # x86-64 function codegen
│   ├── functions_codegen_llvm.c    # LLVM function codegen
│   └── functions_standalone.c      # Main compiler binary
│
├── statement/              # Statements (print, return, assign)
│   ├── statement_parser.c  # Statement-level parsing
│   └── statement_codegen.c # x86-64 statement codegen
│
├── arithmetic/             # Arithmetic expressions (+, -, *, /)
│   ├── arithmetic.c/.h     # Expression tree building
│   └── arithmetic_codegen.c # x86-64 arithmetic codegen
│
├── comparison/             # Comparison operators (>, <, ==, etc)
│   ├── comparison.c/.h     # Comparison parsing
│   └── comparison_codegen.c # x86-64 comparison codegen
│
├── logical/                # Boolean operations (and, or, not)
│   ├── logical.c/.h        # Logical expression parsing
│   └── logical_codegen.c   # x86-64 logical codegen
│
├── control_flow/           # If/while/for statements
│   ├── if_else.c/.h        # If/else parsing & codegen
│   ├── while_loop.c/.h     # While loop
│   └── for_loop.c/.h       # For loop
│
├── array/                  # Array/list support
│   ├── array_parser.c      # Array literal [1,2,3]
│   └── array_codegen.c     # Array allocation & indexing
│
├── string_operations/      # String methods
│   ├── string_ops.c/.h     # length, substring, indexOf
│   └── string_codegen.c    # String operation codegen
│
├── llvm_backend/           # LLVM IR generation (core)
│   ├── llvm_backend.c/.h   # LLVM IR API wrapper
│   ├── llvm_context.c      # Context management
│   └── Makefile            # Independent build
│
├── sto_runtime/            # Smart Type Optimization
│   ├── sto_numeric.c       # Numeric STO hints
│   └── sto_string.c        # String STO hints
│
└── print/                  # Print statement
    ├── print_parser.c      # Print syntax parsing
    └── print_codegen.c     # x86-64 print codegen
```

**Key Design Principles:**

1. **Each module is standalone** - Has own Makefile, can compile independently
2. **No circular dependencies** - Modules communicate via well-defined APIs
3. **Stateless parsing** - Parser functions don't maintain global state
4. **Token ownership** - Clear memory management (tokens passed/returned)
5. **Dual codegen** - Each feature has both LLVM and x86-64 codegen

---

## 🔧 COMPONENT DOCUMENTATION

### 1. Lexer Module (`modules/lexer_mlp/`)

**Purpose:** Convert source code text into tokens

**Key Features:**
- UTF-8 support (handles "MELP Dünya")
- String literal recognition with escape sequences (`\n`, `\t`, `\"`, `\\`)
- Numeric literals (integers and floats)
- Keywords (function, if, while, for, return, etc.)
- Operators (+, -, *, /, ==, !=, <, >, <=, >=, and, or, not)
- Comments (-- single line, /* multi-line */)

**Token Types:**
```c
typedef enum {
    TOKEN_NUMERIC,      // 42, 3.14
    TOKEN_STRING,       // "Hello World"
    TOKEN_IDENTIFIER,   // variable names, function names
    TOKEN_KEYWORD,      // function, if, while, for, return
    TOKEN_OPERATOR,     // +, -, *, /, ==, etc.
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_COMMA,        // ,
    TOKEN_NEWLINE,      // \n
    TOKEN_EOF,          // End of file
    // ... more token types
} TokenType;
```

**Example Input/Output:**
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function
```

**Tokens:**
```
TOKEN_KEYWORD("function")
TOKEN_IDENTIFIER("add")
TOKEN_LPAREN
TOKEN_NUMERIC_TYPE("numeric")
TOKEN_IDENTIFIER("a")
TOKEN_COMMA
TOKEN_NUMERIC_TYPE("numeric")
TOKEN_IDENTIFIER("b")
TOKEN_RPAREN
TOKEN_KEYWORD("returns")
TOKEN_NUMERIC_TYPE("numeric")
TOKEN_NEWLINE
TOKEN_KEYWORD("return")
TOKEN_IDENTIFIER("a")
TOKEN_OPERATOR("+")
TOKEN_IDENTIFIER("b")
TOKEN_NEWLINE
TOKEN_KEYWORD("end")
TOKEN_KEYWORD("function")
TOKEN_EOF
```

### 2. Parser Module (`modules/parser_core/`, `modules/functions/`, etc.)

**Purpose:** Convert tokens into Abstract Syntax Tree (AST)

**Architecture:** Distributed parsing (no central parser)
- Each module parses its own syntax
- Stateless pattern (`parse_*_stateless()` functions)
- Token passing/returning for memory safety

**Key Components:**

**Variable Parser** (`modules/variable/`)
```c
// Parses: numeric x = 42
// Parses: string msg = "Hello"
ASTNode* parse_variable_declaration(Token** tokens);
```

**Function Parser** (`modules/functions/`)
```c
// Parses entire function definition
// Handles parameters, return type, body
FunctionNode* parse_function_definition(Token** tokens);
```

**Statement Parser** (`modules/statement/`)
```c
// Parses: print(x)
// Parses: return 42
// Parses: x = 10
ASTNode* parse_statement(Token** tokens);
```

**Expression Parser** (`modules/arithmetic/`, `modules/comparison/`, `modules/logical/`)
```c
// Parses: a + b * c (with precedence)
// Parses: x > 10 and y < 20
// Parses: not (a == b)
ExpressionNode* parse_expression(Token** tokens);
```

**AST Node Example:**
```c
typedef struct ASTNode {
    ASTNodeType type;  // STMT_PRINT, STMT_RETURN, EXPR_ADD, etc.
    union {
        struct {
            char* name;
            ExpressionNode* value;
        } variable;
        
        struct {
            ExpressionNode* left;
            ExpressionNode* right;
            ArithmeticOp op;  // ADD, SUB, MUL, DIV
        } arithmetic;
        
        struct {
            ExpressionNode* condition;
            ASTNode* then_block;
            ASTNode* else_block;
        } if_stmt;
    } data;
} ASTNode;
```

### 3. LLVM Backend Module (`modules/llvm_backend/`)

**Purpose:** Generate LLVM IR text format from AST

**API Design Philosophy:**
- Clean abstraction over LLVM IR text syntax
- Context-based (LLVMContext tracks state)
- Type-safe value representation (LLVMValue)
- Automatic register naming (%1, %2, %3, ...)

**Core API:**

```c
// Context management
LLVMContext* llvm_context_create(FILE* output);
void llvm_context_destroy(LLVMContext* ctx);

// Module structure
void llvm_emit_module_header(LLVMContext* ctx);
void llvm_emit_module_footer(LLVMContext* ctx);

// Function structure
void llvm_emit_function_start(LLVMContext* ctx, const char* name, 
                              const char** param_types, int param_count);
void llvm_emit_function_entry(LLVMContext* ctx);
void llvm_emit_function_end(LLVMContext* ctx);

// Variables (stack allocation)
LLVMValue* llvm_emit_alloca(LLVMContext* ctx, const char* var_name);
void llvm_emit_store(LLVMContext* ctx, LLVMValue* value, LLVMValue* ptr);
LLVMValue* llvm_emit_load(LLVMContext* ctx, LLVMValue* ptr);

// Arithmetic
LLVMValue* llvm_emit_add(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);
LLVMValue* llvm_emit_sub(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);
LLVMValue* llvm_emit_mul(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);
LLVMValue* llvm_emit_sdiv(LLVMContext* ctx, LLVMValue* left, LLVMValue* right);

// Comparison
LLVMValue* llvm_emit_icmp(LLVMContext* ctx, const char* predicate, 
                         LLVMValue* left, LLVMValue* right);
// Predicates: "sgt", "slt", "eq", "ne", "sge", "sle"

// Control flow
void llvm_emit_label(LLVMContext* ctx, const char* label);
void llvm_emit_br(LLVMContext* ctx, const char* label);
void llvm_emit_br_cond(LLVMContext* ctx, LLVMValue* condition,
                      const char* true_label, const char* false_label);

// Function calls
LLVMValue* llvm_emit_call(LLVMContext* ctx, const char* func_name,
                         LLVMValue** args, int arg_count);

// Return
void llvm_emit_return(LLVMContext* ctx, LLVMValue* value);
```

**LLVMValue Type System:**
```c
typedef enum {
    LLVM_TYPE_I64,      // 64-bit integer (numeric)
    LLVM_TYPE_I8_PTR,   // char* (string)
    LLVM_TYPE_I1,       // 1-bit integer (boolean)
    LLVM_TYPE_VOID      // void (no return)
} LLVMType;

typedef struct {
    char register_name[32];  // "%1", "%x_ptr", etc.
    LLVMType type;
} LLVMValue;
```

**Generated LLVM IR Example:**
```llvm
; ModuleID = 'melp_module'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i64 @add(i64 %a, i64 %b) {
entry:
    %a_ptr = alloca i64
    store i64 %a, i64* %a_ptr
    %b_ptr = alloca i64
    store i64 %b, i64* %b_ptr
    %1 = load i64, i64* %a_ptr
    %2 = load i64, i64* %b_ptr
    %3 = add i64 %1, %2
    ret i64 %3
}
```

### 4. x86-64 Assembly Backend (`modules/functions/*_codegen.c`)

**Purpose:** Generate native x86-64 assembly directly from AST

**Architecture:**
- Direct assembly emission (no intermediate representation)
- Stack-based variable allocation
- System V AMD64 ABI calling convention
- Register allocation strategy

**Key Components:**

**Function Codegen** (`functions_codegen.c`)
```c
// Generates function prologue/epilogue
void codegen_function(FunctionNode* func, FILE* output);

// Function structure:
// .globl function_name
// function_name:
//     pushq %rbp
//     movq %rsp, %rbp
//     subq $stack_size, %rsp
//     ... function body ...
//     leave
//     ret
```

**Statement Codegen** (`statement_codegen.c`)
```c
// Generates assembly for statements
void codegen_statement(ASTNode* stmt, FILE* output, 
                      LocalVariable* locals, int* label_counter);

// Handles:
// - print statements → call puts@PLT
// - return statements → movq value, %rax; leave; ret
// - assignments → movq value, offset(%rbp)
```

**Arithmetic Codegen** (`arithmetic_codegen.c`)
```c
// Generates arithmetic operations
void codegen_arithmetic(ExpressionNode* expr, FILE* output,
                       LocalVariable* locals);

// Strategy:
// 1. Evaluate left operand → %rax
// 2. Push %rax to stack
// 3. Evaluate right operand → %rax
// 4. Pop left operand → %rbx
// 5. Perform operation (addq, subq, imulq, idivq)
// 6. Result in %rax
```

**Register Usage:**
```
%rax - Return value, arithmetic operations
%rbx - Temporary storage (left operand)
%rcx - Temporary storage (right operand)
%rdx - Division operations (remainder)
%rdi - 1st function argument
%rsi - 2nd function argument
%rdx - 3rd function argument
%rcx - 4th function argument (note: conflicts with temp)
%r8  - 5th function argument
%r9  - 6th function argument
%rbp - Frame pointer
%rsp - Stack pointer
```

**Example Generated Assembly:**
```asm
.globl add
add:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp           # Allocate stack for 2 params
    movq %rdi, -8(%rbp)      # Store param a
    movq %rsi, -16(%rbp)     # Store param b
    movq -8(%rbp), %rax      # Load a
    movq -16(%rbp), %rbx     # Load b
    addq %rbx, %rax          # a + b → %rax
    leave
    ret
```

### 5. Runtime Library (`runtime/`)

**Structure:**
```
runtime/
├── stdlib/                 # Standard library functions
│   ├── mlp_print.c         # mlp_println_numeric()
│   ├── mlp_string.c        # mlp_println_string()
│   ├── mlp_string_concat.c # mlp_string_concat()
│   ├── mlp_string_compare.c # mlp_string_compare(), mlp_string_equals()
│   └── Makefile            # Build libmelp_stdlib.a
│
└── sto/                    # Smart Type Optimization runtime
    ├── sto_numeric.c       # Numeric type inference
    ├── sto_string.c        # String optimization (SSO/heap)
    └── Makefile            # Build libtto_runtime.a
```

**Standard Library Functions:**

```c
// Print functions
void mlp_println_numeric(int64_t value);
void mlp_println_string(const char* str);

// String operations
char* mlp_string_concat(const char* a, const char* b);
int mlp_string_compare(const char* a, const char* b);  // Returns -1, 0, 1
int mlp_string_equals(const char* a, const char* b);   // Returns 0 or 1

// Future: String methods
int mlp_string_length(const char* str);
char* mlp_string_substring(const char* str, int start, int length);
int mlp_string_indexOf(const char* str, const char* search);
```

**STO Runtime Functions:**

```c
// Numeric type inference (used by compiler, not user)
NumericType codegen_tto_infer_numeric_type(const char* value);
// Returns: TYPE_INT64, TYPE_DOUBLE, TYPE_BIGDECIMAL

// String optimization hints
StringStorageType codegen_tto_infer_string_type(const char* value);
// Returns: STORAGE_SSO, STORAGE_HEAP, STORAGE_CONSTANT
```

**Linking Order (Critical!):**
```bash
gcc output.s \
    -L./runtime/stdlib -lmelp_stdlib \    # stdlib first
    -L./runtime/sto -ltto_runtime \       # then STO runtime
    -o executable
```

---

## 📝 CODE FLOW EXAMPLES

### Example 1: Simple Program Compilation (LLVM Backend)

**Source Code:** `hello.mlp`
```mlp
function main() returns numeric
    print("Hello MELP")
    return 0
end function
```

**Step-by-Step Flow:**

**1. Lexing:**
```bash
./lexer_mlp hello.mlp
```
Output (conceptual):
```
TOKEN_KEYWORD("function")
TOKEN_IDENTIFIER("main")
TOKEN_LPAREN
TOKEN_RPAREN
TOKEN_KEYWORD("returns")
TOKEN_NUMERIC_TYPE("numeric")
TOKEN_NEWLINE
TOKEN_KEYWORD("print")
TOKEN_LPAREN
TOKEN_STRING("Hello MELP")
TOKEN_RPAREN
TOKEN_NEWLINE
TOKEN_KEYWORD("return")
TOKEN_NUMERIC("0")
TOKEN_NEWLINE
TOKEN_KEYWORD("end")
TOKEN_KEYWORD("function")
TOKEN_EOF
```

**2. Parsing:**
```c
// Parse function definition
FunctionNode* func = parse_function_definition(&tokens);
// func->name = "main"
// func->return_type = TYPE_NUMERIC
// func->params = NULL (no parameters)
// func->body = [
//   STMT_PRINT("Hello MELP"),
//   STMT_RETURN(0)
// ]
```

**3. LLVM IR Generation:**
```bash
./functions_compiler --backend=llvm hello.mlp hello.ll
```

Generated `hello.ll`:
```llvm
; ModuleID = 'melp_module'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"Hello MELP\00"

declare void @mlp_println_string(i8*)

define i64 @main() {
entry:
    %1 = getelementptr [11 x i8], [11 x i8]* @.str, i32 0, i32 0
    call void @mlp_println_string(i8* %1)
    ret i64 0
}
```

**4. Compilation to Executable:**
```bash
clang hello.ll \
    -L./runtime/stdlib -lmelp_stdlib \
    -L./runtime/sto -ltto_runtime \
    -o hello
```

**5. Execution:**
```bash
./hello
# Output: Hello MELP
# Exit code: 0
```

### Example 2: Arithmetic with Variables (x86-64 Backend)

**Source Code:** `calc.mlp`
```mlp
function calculate(numeric x, numeric y) returns numeric
    numeric result = x * 2 + y
    return result
end function

function main() returns numeric
    return calculate(10, 5)
end function
```

**Parsing Output (AST):**
```
Function: calculate
├── Parameters: [x: numeric, y: numeric]
├── Return Type: numeric
└── Body:
    ├── Variable Declaration: result = EXPR_ADD(
    │   ├── EXPR_MUL(x, 2)
    │   └── y
    │   )
    └── Return: result

Function: main
├── Parameters: []
├── Return Type: numeric
└── Body:
    └── Return: CALL(calculate, [10, 5])
```

**x86-64 Assembly Generation:**
```bash
./functions_compiler --backend=x86-64 calc.mlp calc.s
```

Generated `calc.s`:
```asm
.globl calculate
calculate:
    pushq %rbp
    movq %rsp, %rbp
    subq $24, %rsp                  # Space for x, y, result
    
    # Store parameters
    movq %rdi, -8(%rbp)             # x at -8(%rbp)
    movq %rsi, -16(%rbp)            # y at -16(%rbp)
    
    # Compute x * 2
    movq -8(%rbp), %rax             # Load x
    movq $2, %rbx
    imulq %rbx, %rax                # x * 2 → %rax
    
    # Add y
    movq -16(%rbp), %rbx            # Load y
    addq %rbx, %rax                 # (x*2) + y → %rax
    
    # Store in result
    movq %rax, -24(%rbp)            # result at -24(%rbp)
    
    # Return result
    movq -24(%rbp), %rax
    leave
    ret

.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    
    # Call calculate(10, 5)
    movq $10, %rdi                  # 1st arg: 10
    movq $5, %rsi                   # 2nd arg: 5
    call calculate
    
    # Return value already in %rax
    leave
    ret
```

**Assembly & Link:**
```bash
gcc calc.s \
    -L./runtime/stdlib -lmelp_stdlib \
    -L./runtime/sto -ltto_runtime \
    -o calc
```

**Execution:**
```bash
./calc
echo $?  # Exit code: 25 (10*2 + 5)
```

### Example 3: String Concatenation (Both Backends)

**Source Code:** `concat.mlp`
```mlp
function greet(string name) returns numeric
    string message = "Hello " + name
    print(message)
    return 0
end function

function main() returns numeric
    return greet("MELP")
end function
```

**LLVM IR Generation:**
```llvm
@.str.0 = private unnamed_addr constant [7 x i8] c"Hello \00"
@.str.1 = private unnamed_addr constant [5 x i8] c"MELP\00"

declare i8* @mlp_string_concat(i8*, i8*)
declare void @mlp_println_string(i8*)

define i64 @greet(i8* %name) {
entry:
    %name_ptr = alloca i8*
    store i8* %name, i8** %name_ptr
    %message_ptr = alloca i8*
    
    ; Concatenate "Hello " + name
    %1 = getelementptr [7 x i8], [7 x i8]* @.str.0, i32 0, i32 0
    %2 = load i8*, i8** %name_ptr
    %3 = call i8* @mlp_string_concat(i8* %1, i8* %2)
    store i8* %3, i8** %message_ptr
    
    ; Print message
    %4 = load i8*, i8** %message_ptr
    call void @mlp_println_string(i8* %4)
    
    ret i64 0
}

define i64 @main() {
entry:
    %1 = getelementptr [5 x i8], [5 x i8]* @.str.1, i32 0, i32 0
    %2 = call i64 @greet(i8* %1)
    ret i64 %2
}
```

**x86-64 Assembly Generation:**
```asm
.section .rodata
.str0:
    .string "Hello "
.str1:
    .string "MELP"

.text
.globl greet
greet:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    movq %rdi, -8(%rbp)              # Store name parameter
    
    # Concatenate "Hello " + name
    movq $.str0, %rdi                # 1st arg: "Hello "
    movq -8(%rbp), %rsi              # 2nd arg: name
    call mlp_string_concat@PLT
    movq %rax, -16(%rbp)             # Store message
    
    # Print message
    movq -16(%rbp), %rdi
    call mlp_println_string@PLT
    
    movq $0, %rax
    leave
    ret

.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    
    movq $.str1, %rdi                # arg: "MELP"
    call greet
    
    leave
    ret
```

### Example 4: Control Flow (If/Else)

**Source Code:** `compare.mlp`
```mlp
function max(numeric a, numeric b) returns numeric
    if a > b then
        return a
    else
        return b
    end if
end function
```

**LLVM IR Generation:**
```llvm
define i64 @max(i64 %a, i64 %b) {
entry:
    %a_ptr = alloca i64
    store i64 %a, i64* %a_ptr
    %b_ptr = alloca i64
    store i64 %b, i64* %b_ptr
    
    %1 = load i64, i64* %a_ptr
    %2 = load i64, i64* %b_ptr
    %3 = icmp sgt i64 %1, %2         ; a > b (signed greater than)
    br i1 %3, label %then, label %else

then:
    %4 = load i64, i64* %a_ptr
    ret i64 %4

else:
    %5 = load i64, i64* %b_ptr
    ret i64 %5
}
```

**x86-64 Assembly Generation:**
```asm
.globl max
max:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    movq %rdi, -8(%rbp)              # Store a
    movq %rsi, -16(%rbp)             # Store b
    
    # Compare a > b
    movq -8(%rbp), %rax              # Load a
    cmpq -16(%rbp), %rax             # Compare with b
    jle .L_else                      # Jump if a <= b
    
.L_then:
    movq -8(%rbp), %rax              # Return a
    leave
    ret
    
.L_else:
    movq -16(%rbp), %rax             # Return b
    leave
    ret
```

---

## 🎨 DESIGN DECISIONS

### Decision 1: Why Dual Backend (LLVM + x86-64)?

**Context:** Most compilers choose one backend approach.

**Options Considered:**
1. ❌ **LLVM only** - Portable but adds dependency, harder to learn
2. ❌ **x86-64 Assembly only** - Fast to develop but not portable
3. ✅ **Both LLVM and x86-64** - Best of both worlds

**Decision: Dual Backend Architecture**

**Rationale:**

**Educational Value:**
- x86-64 backend teaches direct code generation
- LLVM backend teaches IR-based compilation
- Developers can compare approaches

**Practical Benefits:**
- LLVM: Production deployment (cross-platform)
- x86-64: Quick testing on Linux (no LLVM dependency)
- Flexibility: Choose backend per use case

**Self-Hosting Path:**
- Stage 0 (C): Both backends maintained
- Stage 1 (MELP): Focus on LLVM backend
- Stage 2: Advanced features via LLVM

**Trade-offs:**
- ⚠️ Maintenance burden (two codegen paths)
- ✅ Mitigated: Shared parser/AST, only codegen differs
- ✅ Tests run on both backends (validation)

**Conclusion:** Dual backend provides maximum flexibility with manageable complexity.

---

### Decision 2: Why C for Stage 0?

**Context:** Self-hosting language needs a bootstrap compiler.

**Options Considered:**
1. ❌ **Python** - Easy to write, but slow and adds runtime dependency
2. ❌ **Rust** - Modern and safe, but steep learning curve
3. ❌ **Go** - Simple and fast, but large binary size
4. ✅ **C** - Universal, fast, minimal dependencies

**Decision: C for Bootstrap Compiler**

**Rationale:**

**Universal Compatibility:**
- C compilers on every platform
- No runtime dependencies
- Small binary size

**Performance:**
- Native speed (important for compiler)
- Direct system access
- Minimal overhead

**Educational Value:**
- Most developers know C
- Teaches low-level programming
- Closer to assembly

**Self-Hosting Strategy:**
- Stage 0 (C): Core features only
- Stage 1 (MELP): Full MELP compiler in MELP
- Stage 2+: Advanced features in MELP

**"Good Enough" Philosophy:**
- Stage 0 doesn't need perfection
- Stage 0 doesn't need all features
- Stage 0 just needs to compile Stage 1

**Trade-offs:**
- ⚠️ Manual memory management
- ⚠️ Less safety than Rust
- ✅ Mitigated: Stateless design, unit tests, clear ownership

**Conclusion:** C provides the fastest path to self-hosting with maximum compatibility.

---

### Decision 3: STO (Smart Type Optimization) vs TTO Migration

**Context:** MELP originally had "TTO" (Transparent Type Optimization), renamed to "STO" (Smart Type Optimization).

**Timeline:**
- **Phase 1-10:** Called "TTO"
- **Phase 11+:** Renamed to "STO"
- **Current:** Both terms exist in codebase

**Why the Change?**

**Marketing/Branding:**
- "Smart" is more user-friendly than "Transparent"
- STO sounds more modern
- Easier to explain to beginners

**Technical Accuracy:**
- Optimization is "smart" (compiler makes decisions)
- Optimization is still "transparent" (user doesn't see it)
- Both names are technically correct

**Migration Status:**

**Completed:**
- ✅ Architecture docs use "STO"
- ✅ New code uses "STO" terminology
- ✅ User-facing docs use "STO"

**Pending:**
- ⚠️ Runtime library still uses `tto_` prefix
- ⚠️ Some internal functions use "TTO"
- ⚠️ Old documentation uses "TTO"

**Migration Strategy:**

**Phase 1:** User-facing rename (DONE)
- Documentation updated
- New features use STO terminology

**Phase 2:** Internal rename (FUTURE - Phase 20)
- Rename `tto_runtime` → `sto_runtime`
- Rename functions: `tto_infer_*` → `sto_infer_*`
- Update all internal references

**Phase 3:** Legacy cleanup (FUTURE)
- Remove old TTO references
- Update historical documentation

**Decision:** Gradual migration, user-facing first, internal later.

**Rationale:**
- Don't break existing code during active development
- User-facing consistency is most important
- Internal rename can wait until Stage 0 is stable

---

### Decision 4: Why LLVM 19?

**Context:** LLVM has many versions (3.x, 7.x, 11.x, 14.x, 17.x, 18.x, 19.x).

**Options Considered:**
1. ❌ **LLVM 11** - Older, stable, widely available
2. ❌ **LLVM 14** - LTS version
3. ❌ **LLVM 17** - Recent stable
4. ✅ **LLVM 19** - Latest stable (2024)

**Decision: LLVM 19**

**Rationale:**

**Modern Features:**
- Better optimization passes
- Improved error messages
- Enhanced debugging support
- Latest WebAssembly support

**Future-Proofing:**
- Won't need upgrade soon
- Aligns with modern toolchains
- Better for long-term maintenance

**Ecosystem Alignment:**
- Rust uses LLVM 19
- Swift uses LLVM 19
- Industry standard

**Trade-offs:**
- ⚠️ Not available on all systems (need manual install)
- ⚠️ Larger binary size
- ✅ Mitigated: Documentation includes install instructions
- ✅ Worth it for modern features

**Installation:**
```bash
# Ubuntu/Debian
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 19

# Arch Linux
sudo pacman -S llvm

# macOS
brew install llvm@19
```

**Conclusion:** LLVM 19 provides best modern features, worth the installation effort.

---

### Decision 5: Stateless Parser Design

**Context:** Most compilers use stateful parsers with global state.

**Traditional Approach (Stateful):**
```c
// Global state (BAD for modularity)
static Token* current_token;
static Token* token_stream;
static int token_index;

void advance() {
    token_index++;
    current_token = &token_stream[token_index];
}

ASTNode* parse_expression() {
    // Uses global current_token
    if (current_token->type == TOKEN_NUMERIC) {
        // ...
    }
}
```

**MELP Approach (Stateless):**
```c
// No global state - tokens passed as parameter
ASTNode* parse_expression_stateless(Token** tokens) {
    Token* current = *tokens;
    
    if (current->type == TOKEN_NUMERIC) {
        ASTNode* node = create_numeric_node(current->value);
        *tokens = current->next;  // Advance token pointer
        return node;
    }
    // ...
}
```

**Decision: Stateless Parser Functions**

**Rationale:**

**Modularity:**
- No global state = independent modules
- Each parser function is self-contained
- Easy to test in isolation

**Memory Safety:**
- Clear token ownership (passed/returned)
- No hidden state mutations
- Easier to reason about memory

**Parallelization Ready:**
- Multiple parsers can run concurrently
- No shared state = no race conditions
- Future optimization potential

**Self-Hosting:**
- Easier to rewrite in MELP
- No hidden dependencies
- Clear function contracts

**Token Ownership Pattern:**
```c
// Caller owns tokens before call
Token* tokens = lexer_tokenize(source);

// Parser borrows tokens (pointer-to-pointer)
ASTNode* ast = parse_function_stateless(&tokens);

// Caller still owns tokens after call
// tokens pointer has advanced to next unconsumed token

// Caller frees tokens when done
free_tokens(tokens);
```

**Trade-offs:**
- ⚠️ More verbose (pass `Token**` everywhere)
- ⚠️ Caller must manage token lifetime
- ✅ Worth it for clarity and modularity

**Conclusion:** Stateless design aligns with MELP's modularity goals and simplifies self-hosting.

---

### Decision 6: No Central Files (Radical Modularity)

**Context:** Most compilers have central orchestration files (main.c, compiler.c, orchestrator.c).

**Traditional Approach:**
```c
// main.c (central orchestration)
int main() {
    tokens = lexer_tokenize(source);    // ❌ Direct function call
    ast = parser_parse(tokens);         // ❌ Direct function call
    ir = codegen_generate(ast);         // ❌ Direct function call
    // ... etc
}
```

**MELP Approach (Radical Modularity):**
```bash
# No central orchestration - Unix pipes only
./lexer_mlp < source.mlp > tokens.json
./parser < tokens.json > ast.json
./codegen < ast.json > output.s
```

**Decision: No Central Files, Modules Communicate via Pipes/JSON**

**Rationale:**

**Self-Hosting Path:**
- Each module can be rewritten in MELP independently
- No C-level dependencies between modules
- Language-agnostic architecture

**Testability:**
- Test each module with simple text files
- No need to compile entire compiler
- Mock modules by creating JSON files

**AI-Friendly Development:**
- AI agents work on one module at a time
- Can't create monolithic code (no place to put it!)
- Forced to respect module boundaries

**Replaceability:**
- Rewrite lexer in Rust? No problem (JSON interface)
- Swap parser? Easy (JSON input/output)
- Experiment with new codegen? Independent module

**Enforcement:**
```bash
# Step 1: Backup central files
mkdir temp/yedek_merkezi_dosyalar/
mv main.c orchestrator.c helpers.c temp/yedek_merkezi_dosyalar/

# Step 2: Update Makefile to check architecture
make check-architecture  # Fails if central files exist

# Step 3: AI agents literally CANNOT violate (no files to edit!)
```

**Trade-offs:**
- ⚠️ More complex build process
- ⚠️ JSON serialization overhead
- ✅ Worth it for modularity and self-hosting
- ✅ "Good Enough" - performance not critical for compiler

**Current Status:**
- ✅ Central files removed and backed up
- ⚠️ Current implementation: Modules still linked as libraries
- 🔜 Future: Full pipe-based architecture (Phase 20+)

**Conclusion:** Radical modularity forces good architecture and enables self-hosting.

---

### Decision 7: "Good Enough" Philosophy for Stage 0

**Context:** Perfectionism can delay self-hosting.

**Traditional Approach:**
- Implement all features in Stage 0
- Perfect optimization before moving on
- Complete error handling
- Full standard library

**MELP Approach: "Good Enough" Philosophy**

**Decision: Minimal Stage 0, Full Features in Stage 1**

**Rationale:**

**Avoid Duplicate Work:**
- Don't perfect C code that will be replaced
- Implement core features in C, advanced features in MELP
- Focus effort on MELP-written compiler

**Faster Self-Hosting:**
- Stage 0 goal: Compile Stage 1 (MELP compiler in MELP)
- Stage 0 doesn't need: structs, generics, advanced optimization
- Stage 1 can add those features (in MELP)

**What Stage 0 MUST Have:**
- ✅ Variables (numeric, string, boolean)
- ✅ Functions (parameters, return values)
- ✅ Control flow (if/else, while, for)
- ✅ Arrays/Lists (basic indexing)
- ✅ String operations (concat, compare, methods)
- ✅ LLVM backend (for portability)
- ✅ Standard library (print, string ops, file I/O)

**What Stage 0 Can SKIP:**
- ❌ Struct definitions (Stage 1 feature)
- ❌ Generics / Templates (Stage 1 feature)
- ❌ Exception handling (Stage 1 feature)
- ❌ Advanced optimization (LLVM does this)
- ❌ Multiple return values (Stage 1 feature)
- ❌ Operator overloading (Stage 1 feature)

**Timeline:**
- **Dec 31, 2025:** Stage 0 Complete (core features)
- **Mar 31, 2026:** Stage 1 Self-hosting (MELP in MELP)
- **May 31, 2026:** Stage 1.5 Advanced features (in MELP)
- **Aug 31, 2026:** Stage 2 Multi-language support

**Conclusion:** "Good enough" gets us to self-hosting faster, then we improve in MELP.

---

**Last Updated:** 14 Aralık 2025 (YZ_71)  
**Status:** Architecture rules active, detailed documentation added
