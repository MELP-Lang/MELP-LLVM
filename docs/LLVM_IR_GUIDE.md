# 🔧 LLVM IR Mapping Guide for MELP

**Author:** YZ_57/YZ_58  
**Date:** 13 Aralık 2025  
**Purpose:** Reference guide for converting MELP AST to LLVM IR

---

## 📚 Table of Contents

1. [Basic Concepts](#basic-concepts)
2. [Type Mapping](#type-mapping)
3. [Variables & Assignments](#variables--assignments)
4. [Arithmetic Operations](#arithmetic-operations)
5. [Function Definitions](#function-definitions)
6. [Function Calls](#function-calls)
7. [Control Flow](#control-flow)
8. [Standard Library Integration](#standard-library-integration)
9. [STO Runtime Integration](#sto-runtime-integration)
10. [Complete Examples](#complete-examples)

---

## 🎯 Basic Concepts

### LLVM IR Fundamentals

**SSA Form (Static Single Assignment):**
- Every variable is assigned exactly once
- Use temporaries: `%1`, `%2`, `%result`, etc.
- No reassignment - use `alloca` + `load`/`store` for mutable variables

**Registers vs Memory:**
```llvm
; Register (temporary, SSA)
%x = add i64 10, 20

; Memory (mutable variable)
%x_ptr = alloca i64        ; Allocate stack space
store i64 30, i64* %x_ptr  ; Store value
%x_val = load i64, i64* %x_ptr  ; Load value
```

**Basic Syntax:**
```llvm
; Comments start with semicolon
@global_var = global i32 42    ; Global variable
%local_var = alloca i64        ; Local variable (stack)
%result = add i64 %a, %b       ; Instruction
ret i64 %result                ; Return statement
```

---

## 🔤 Type Mapping

### MELP → LLVM Type Conversions

| MELP Type | LLVM Type | Description |
|-----------|-----------|-------------|
| `numeric` | `i64` | 64-bit signed integer |
| `text` | `i8*` | Pointer to null-terminated string |
| `boolean` | `i1` | 1-bit boolean (true/false) |
| `list` | `{i64, i64, i8*}*` | Pointer to list structure |
| `tuple` | `{...}` | Structure with fixed fields |
| `function` | `i64 (i64, i64)*` | Function pointer |

### STO Type System

```llvm
; STO value wrapper (for runtime type tracking)
%sto_value = type {
    i8,   ; type_tag (0=numeric_int, 1=text, 2=bigdec, etc.)
    i64   ; value (integer or pointer)
}

; Type tags (from STO runtime)
; 0 = STO_NUMERIC_INT64
; 1 = STO_TEXT
; 2 = STO_NUMERIC_BIGDEC
; 3 = STO_NUMERIC_FLOAT64
```

---

## 📦 Variables & Assignments

### Variable Declaration

**MELP:**
```mlp
numeric x = 10
```

**LLVM IR:**
```llvm
; Allocate stack space for variable
%x = alloca i64, align 8

; Store initial value
store i64 10, i64* %x, align 8
```

### Variable Assignment

**MELP:**
```mlp
x = 20
```

**LLVM IR:**
```llvm
; Store new value (assuming %x is already allocated)
store i64 20, i64* %x, align 8
```

### Variable Usage

**MELP:**
```mlp
numeric y = x + 5
```

**LLVM IR:**
```llvm
; Load x value
%x_val = load i64, i64* %x, align 8

; Perform addition
%add_result = add nsw i64 %x_val, 5

; Store to y
%y = alloca i64, align 8
store i64 %add_result, i64* %y, align 8
```

---

## ➕ Arithmetic Operations

### Addition

**MELP:**
```mlp
numeric result = a + b
```

**Assembly (Current):**
```asm
movq -8(%rbp), %r8   # Load a
movq -16(%rbp), %r9  # Load b
addq %r9, %r8
```

**LLVM IR:**
```llvm
%a_val = load i64, i64* %a, align 8
%b_val = load i64, i64* %b, align 8
%result = add nsw i64 %a_val, %b_val
; nsw = no signed wrap (undefined behavior on overflow)
; Alternative: add (allows overflow, wraps around)
```

### Subtraction

**MELP:**
```mlp
numeric diff = a - b
```

**LLVM IR:**
```llvm
%a_val = load i64, i64* %a, align 8
%b_val = load i64, i64* %b, align 8
%diff = sub nsw i64 %a_val, %b_val
```

### Multiplication

**MELP:**
```mlp
numeric product = a * b
```

**LLVM IR:**
```llvm
%a_val = load i64, i64* %a, align 8
%b_val = load i64, i64* %b, align 8
%product = mul nsw i64 %a_val, %b_val
```

### Division

**MELP:**
```mlp
numeric quotient = a / b
```

**LLVM IR:**
```llvm
%a_val = load i64, i64* %a, align 8
%b_val = load i64, i64* %b, align 8

; Check for division by zero
%is_zero = icmp eq i64 %b_val, 0
br i1 %is_zero, label %div_error, label %div_ok

div_error:
    ; Call error handler
    call void @mlp_panic_division_by_zero()
    unreachable

div_ok:
    %quotient = sdiv i64 %a_val, %b_val
    ; sdiv = signed division
```

---

## 🔧 Function Definitions

### Simple Function

**MELP:**
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function
```

**Assembly (Current):**
```asm
.global add
add:
    pushq %rbp
    movq %rsp, %rbp
    # a in %rdi, b in %rsi (System V ABI)
    movq %rdi, %r8
    movq %rsi, %r9
    addq %r9, %r8
    movq %r8, %rax
    popq %rbp
    ret
```

**LLVM IR:**
```llvm
define i64 @add(i64 %a, i64 %b) {
entry:
    ; Perform addition
    %result = add nsw i64 %a, %b
    
    ; Return result
    ret i64 %result
}
```

### Function with Local Variables

**MELP:**
```mlp
function multiply(numeric x, numeric y) returns numeric
    numeric product = x * y
    return product
end function
```

**LLVM IR:**
```llvm
define i64 @multiply(i64 %x, i64 %y) {
entry:
    ; Allocate local variable
    %product = alloca i64, align 8
    
    ; Calculate x * y
    %mul_result = mul nsw i64 %x, %y
    
    ; Store to product
    store i64 %mul_result, i64* %product, align 8
    
    ; Load for return
    %ret_val = load i64, i64* %product, align 8
    ret i64 %ret_val
}
```

### Main Function

**MELP:**
```mlp
function main() returns numeric
    return 0
end function
```

**LLVM IR:**
```llvm
define i64 @main() {
entry:
    ret i64 0
}
```

---

## 📞 Function Calls

### Simple Call

**MELP:**
```mlp
numeric result = add(10, 20)
```

**LLVM IR:**
```llvm
; Call function with literal arguments
%result = call i64 @add(i64 10, i64 20)
```

### Call with Variables

**MELP:**
```mlp
numeric a = 5
numeric b = 7
numeric sum = add(a, b)
```

**LLVM IR:**
```llvm
; Allocate and initialize a
%a = alloca i64, align 8
store i64 5, i64* %a, align 8

; Allocate and initialize b
%b = alloca i64, align 8
store i64 7, i64* %b, align 8

; Load values for call
%a_val = load i64, i64* %a, align 8
%b_val = load i64, i64* %b, align 8

; Call function
%sum_val = call i64 @add(i64 %a_val, i64 %b_val)

; Store result
%sum = alloca i64, align 8
store i64 %sum_val, i64* %sum, align 8
```

### Chained Calls

**MELP:**
```mlp
numeric result = add(multiply(2, 3), 10)
```

**LLVM IR:**
```llvm
; First call: multiply(2, 3)
%mul_result = call i64 @multiply(i64 2, i64 3)

; Second call: add(mul_result, 10)
%result = call i64 @add(i64 %mul_result, i64 10)
```

---

## 🔀 Control Flow

### If Statement

**MELP:**
```mlp
if x > 10 then
    y = 1
else
    y = 0
end if
```

**LLVM IR:**
```llvm
; Load x value
%x_val = load i64, i64* %x, align 8

; Compare x > 10
%cmp = icmp sgt i64 %x_val, 10
; sgt = signed greater than

; Branch based on comparison
br i1 %cmp, label %then_block, label %else_block

then_block:
    store i64 1, i64* %y, align 8
    br label %after_if

else_block:
    store i64 0, i64* %y, align 8
    br label %after_if

after_if:
    ; Continue execution
```

### While Loop

**MELP:**
```mlp
while x < 10 do
    x = x + 1
end while
```

**LLVM IR:**
```llvm
; Jump to condition check
br label %while_cond

while_cond:
    ; Load x value
    %x_val = load i64, i64* %x, align 8
    
    ; Check condition x < 10
    %cmp = icmp slt i64 %x_val, 10
    
    ; Branch: continue loop or exit
    br i1 %cmp, label %while_body, label %while_end

while_body:
    ; x = x + 1
    %x_val_body = load i64, i64* %x, align 8
    %x_inc = add nsw i64 %x_val_body, 1
    store i64 %x_inc, i64* %x, align 8
    
    ; Loop back to condition
    br label %while_cond

while_end:
    ; Continue after loop
```

### For Loop

**MELP:**
```mlp
for i = 1 to 10 do
    sum = sum + i
end for
```

**LLVM IR:**
```llvm
; Initialize loop variable
%i = alloca i64, align 8
store i64 1, i64* %i, align 8

; Jump to condition
br label %for_cond

for_cond:
    ; Load i
    %i_val = load i64, i64* %i, align 8
    
    ; Check i <= 10
    %cmp = icmp sle i64 %i_val, 10
    br i1 %cmp, label %for_body, label %for_end

for_body:
    ; sum = sum + i
    %sum_val = load i64, i64* %sum, align 8
    %i_val_body = load i64, i64* %i, align 8
    %sum_new = add nsw i64 %sum_val, %i_val_body
    store i64 %sum_new, i64* %sum, align 8
    
    ; Increment i
    %i_inc = add nsw i64 %i_val_body, 1
    store i64 %i_inc, i64* %i, align 8
    
    ; Loop back
    br label %for_cond

for_end:
    ; Continue after loop
```

---

## 📚 Standard Library Integration

### println Function

**MELP:**
```mlp
numeric result = println(42)
```

**Option 1: Direct printf (Simple):**
```llvm
; External C function
declare i32 @printf(i8*, ...)

; String constant
@.fmt_num = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

; In function:
%fmt_ptr = getelementptr inbounds [5 x i8], [5 x i8]* @.fmt_num, i64 0, i64 0
call i32 (i8*, ...) @printf(i8* %fmt_ptr, i64 42)
```

**Option 2: MELP stdlib (Full Integration):**
```llvm
; External MELP function
declare void @mlp_println_numeric(i8*, i8)

; In function:
%val_ptr = inttoptr i64 42 to i8*
call void @mlp_println_numeric(i8* %val_ptr, i8 0)
; Second arg = STO type tag (0 = numeric int)
```

---

## 🔄 STO Runtime Integration

### Overflow Detection (Future)

**Assembly (Current):**
```asm
addq %r9, %r8
jo .overflow_detected_0  # Jump if overflow
jmp .no_overflow_0
.overflow_detected_0:
    call sto_bigdec_from_int64
    # ... promote to BigDecimal
.no_overflow_0:
```

**LLVM IR (Option 1: Explicit Check):**
```llvm
; Perform addition with overflow detection
%add_result = call {i64, i1} @llvm.sadd.with.overflow.i64(i64 %a, i64 %b)
%result = extractvalue {i64, i1} %add_result, 0
%overflow = extractvalue {i64, i1} %add_result, 1

; Branch on overflow
br i1 %overflow, label %handle_overflow, label %no_overflow

handle_overflow:
    ; Promote to BigDecimal
    %bigdec_a = call %sto_value* @sto_bigdec_from_int64(i64 %a)
    %bigdec_b = call %sto_value* @sto_bigdec_from_int64(i64 %b)
    %bigdec_result = call %sto_value* @sto_bigdec_add(%sto_value* %bigdec_a, %sto_value* %bigdec_b)
    br label %after_add

no_overflow:
    ; Use integer result
    br label %after_add

after_add:
    ; Phi node to merge results
    %final_result = phi i64 [ %result, %no_overflow ], [ 0, %handle_overflow ]
```

**LLVM IR (Option 2: Simplified - Initial Implementation):**
```llvm
; For initial implementation, just use nsw/nuw and let it trap
%result = add nsw i64 %a, %b
; STO integration deferred to Phase 14
```

---

## 📝 Complete Examples

### Example 1: test_basic.mlp

**MELP Source:**
```mlp
function main() returns numeric
    numeric x = 10
    numeric y = 20
    return x + y
end function
```

**LLVM IR Output:**
```llvm
define i64 @main() {
entry:
    ; Allocate local variables
    %x = alloca i64, align 8
    %y = alloca i64, align 8
    
    ; numeric x = 10
    store i64 10, i64* %x, align 8
    
    ; numeric y = 20
    store i64 20, i64* %y, align 8
    
    ; Load values for addition
    %x_val = load i64, i64* %x, align 8
    %y_val = load i64, i64* %y, align 8
    
    ; Perform addition
    %add_result = add nsw i64 %x_val, %y_val
    
    ; Return result
    ret i64 %add_result
}
```

### Example 2: Function Call

**MELP Source:**
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function

function main() returns numeric
    numeric result = add(10, 20)
    return result
end function
```

**LLVM IR Output:**
```llvm
; Function: add
define i64 @add(i64 %a, i64 %b) {
entry:
    %result = add nsw i64 %a, %b
    ret i64 %result
}

; Function: main
define i64 @main() {
entry:
    ; Call add(10, 20)
    %result = call i64 @add(i64 10, i64 20)
    
    ; Return result
    ret i64 %result
}
```

### Example 3: With println

**MELP Source:**
```mlp
function main() returns numeric
    numeric x = 42
    numeric dummy = println(x)
    return 0
end function
```

**LLVM IR Output:**
```llvm
; External declarations
declare i32 @printf(i8*, ...)

@.fmt_num = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

define i64 @main() {
entry:
    ; Allocate local variable
    %x = alloca i64, align 8
    
    ; numeric x = 42
    store i64 42, i64* %x, align 8
    
    ; Load x value
    %x_val = load i64, i64* %x, align 8
    
    ; Call printf to print
    %fmt_ptr = getelementptr inbounds [5 x i8], [5 x i8]* @.fmt_num, i64 0, i64 0
    call i32 (i8*, ...) @printf(i8* %fmt_ptr, i64 %x_val)
    
    ; Return 0
    ret i64 0
}
```

---

## 🔨 Compilation Workflow

### Current (Assembly):
```bash
# Compile MELP to assembly
./functions_compiler test.mlp test.s

# Assemble and link
gcc -no-pie test.s -L../../runtime/stdlib -lmlp_stdlib \
    -L../../runtime/sto -lsto_runtime -lm -o test

# Run
./test
```

### LLVM Backend (Planned):
```bash
# Compile MELP to LLVM IR
./functions_compiler --backend=llvm test.mlp test.ll

# Compile LLVM IR to binary
clang test.ll ../../runtime/stdlib/libmlp_stdlib.a \
    ../../runtime/sto/libsto_runtime.a -lm -o test

# Run
./test
```

---

## 📊 Comparison Table

| Feature | Assembly Backend | LLVM IR Backend |
|---------|------------------|-----------------|
| **Portability** | x86-64 only | Cross-platform |
| **Optimization** | Manual | LLVM optimizations |
| **Code Size** | Verbose | Compact |
| **Debugging** | GDB-friendly | LLVM debug info |
| **Maintainability** | Low | High |
| **Register Allocation** | Manual | Automatic |
| **Instruction Selection** | Manual | Automatic |

---

## 🎯 Implementation Strategy

### Phase 13.5 Priorities

**Part 2: Core IR Generation**
1. Variables (alloca, store, load) ✅
2. Arithmetic (add, sub, mul, sdiv) ✅
3. Functions (define, call, ret) ✅
4. Basic comparisons (icmp)

**Part 3: Advanced Features**
5. Control flow (br, label, phi)
6. String literals (getelementptr)
7. stdlib integration (printf)

**Part 4: STO Integration** (Simplified)
8. Direct integer operations (skip overflow for now)
9. Defer BigDecimal promotion to Phase 14

---

## 📚 References

- **LLVM Language Reference:** https://llvm.org/docs/LangRef.html
- **LLVM Tutorial:** https://llvm.org/docs/tutorial/
- **System V ABI:** Calling conventions for x86-64
- **MELP Documentation:** `/docs/language/`

---

**Last Updated:** 13 Aralık 2025  
**Status:** Part 1 Complete ✅  
**Next:** Create llvm_backend module (Part 2)
