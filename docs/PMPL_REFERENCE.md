# PMPL (Pragmatic MLP) - Language Specification

**Version:** 1.0  
**Date:** December 14, 2025  
**Status:** Official Language Standard

---

## 📘 Introduction

**PMPL (Pragmatic MLP)** is the intermediate representation language used internally by MELP compilers. It serves as the **canonical syntax** that all user-facing code normalizes to before compilation.

### Purpose

PMPL provides:
- **Syntax Normalization**: Unified representation for multi-language input
- **Lexer Simplification**: Single-token keywords (underscore notation)
- **Parser Clarity**: Eliminates pattern matching complexity
- **Self-Hosting Foundation**: PMPL compilers can compile PMPL code

### Philosophy

> **"Write code in any syntax. The compiler sees PMPL."**

Users can write in C-style, Python-style, Ruby-style, or any supported syntax. The normalizer transforms everything to PMPL before lexical analysis.

---

## 🏗️ Architecture

```
┌─────────────┐
│ User Code   │ (any style: C, Python, Ruby, etc.)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Normalizer  │ (syntax.json rules)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│    PMPL     │ ◄── YOU ARE HERE
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Lexer     │ (tokenization)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Parser    │ (AST construction)
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Code Gen   │ (assembly/LLVM IR)
└─────────────┘
```

---

## 📝 Syntax Overview

### Core Principle: Underscore Keywords

All multi-word keywords use **underscore** (`_`) instead of spaces:

| User Syntax | PMPL Standard |
|-------------|---------------|
| `end if` | `end_if` |
| `else if` | `else_if` |
| `end while` | `end_while` |
| `end for` | `end_for` |
| `end function` | `end_function` |

**Why?** Single-token keywords simplify lexing and eliminate ambiguity.

---

## 🔤 Complete Syntax Reference

### 1. Conditionals

#### Basic If Statement

```pmpl
if condition then
    statement
end_if
```

**Tokens:**
```
TOKEN_IF, expression, TOKEN_THEN, statement, TOKEN_END_IF
```

#### If-Else

```pmpl
if condition then
    statement1
else
    statement2
end_if
```

#### If-ElseIf-Else Chain

```pmpl
if x > 10 then
    print("Large")
else_if x > 5 then        -- NOTE: underscore!
    print("Medium")
else_if x > 0 then
    print("Small")
else
    print("Zero or negative")
end_if
```

**Important:** `else_if` is a **single token** (`TOKEN_ELSE_IF`), not two tokens.

---

### 2. Loops

#### While Loop

```pmpl
while condition do
    statement
end_while
```

**Example:**
```pmpl
numeric count = 0
while count < 10 do
    print(count)
    count = count + 1
end_while
```

#### For Loop (Range)

```pmpl
for variable = start to end do
    statement
end_for
```

**Example:**
```pmpl
for i = 1 to 10 do
    print(i)
end_for
```

#### For Loop (Descending)

```pmpl
for variable = start downto end do
    statement
end_for
```

**Example:**
```pmpl
for i = 10 downto 1 do
    print(i)
end_for
```

#### For-Each Loop

```pmpl
for each element in collection do
    statement
end_for
```

**Example:**
```pmpl
for each item in items do
    print(item)
end_for
```

---

### 3. Loop Control

#### Exit (Break)

```pmpl
while condition do
    if exit_condition then
        exit        -- Exit current loop
    end_if
end_while
```

**Specific Loop Exit:**
```pmpl
exit_for        -- Exit for loop explicitly
exit_while      -- Exit while loop explicitly
exit_if         -- Exit if statement early
exit_function   -- Return from function early
exit_switch     -- Break from switch case
```

#### Continue

```pmpl
for i = 1 to 100 do
    if i mod 2 == 0 then
        continue    -- Skip even numbers
    end_if
    print(i)
end_for
```

**Specific Loop Continue:**
```pmpl
continue_for    -- Continue for loop explicitly
continue_while  -- Continue while loop explicitly
```

### 3.1 Debug Syntax

PMPL supports **two types of debug** syntax:

#### 1. Block Debug (Multi-line)

```pmpl
-- Debug block (completely removed in production)
debug
    -- Everything here is removed in production builds
    print("Debug: calculation starting")
    print("x value: " + x)
    
    numeric debug_counter = 0
    
    repeat:  -- Debug label (only inside debug)
    debug_counter = debug_counter + 1
    print("Step: " + debug_counter)
    
    if debug_counter < 5 then
        goto repeat  -- Debug goto (only inside debug)
    end_if
    
    pause  -- Debug pause (only inside debug)
end_debug
```

#### 2. Single-Line Debug

```pmpl
-- Single-line debug (NO end_debug)
debug print("x = " + x)
debug if a == b then c = d
debug numeric temp = x * 2 : print(temp)

-- Debug-only keywords (only in debug context)
debug start:           -- Define label
debug goto start       -- Jump to label
debug pause            -- Pause debugger
```

#### Debug-Only Keywords

These keywords can **only be used in debug blocks or debug lines**:

| Keyword | Usage | Description |
|---------|-------|-------------|
| `label:` | `debug start:` | Define debug label |
| `goto` | `debug goto start` | Jump to label |
| `pause` | `debug pause` | Pause debugger |

**Error Examples:**
```pmpl
-- ❌ ERROR: goto cannot be used outside debug
function test()
    goto start  -- COMPILE ERROR!
end_function

-- ❌ ERROR: label cannot be used outside debug
function test()
    start:      -- COMPILE ERROR!
    print("test")
end_function

-- ❌ ERROR: pause cannot be used outside debug
function test()
    pause       -- COMPILE ERROR!
end_function
```

**Correct Usage:**
```pmpl
function calculate(numeric x) returns numeric
    debug print("calculate() called: x = " + x)
    
    if x < 0 then
        debug print("WARNING: Negative value!")
        return 0
    end_if
    
    debug
        numeric step = 0
        repeat:
        step = step + 1
        print("Step: " + step)
        if step < 3 then
            goto repeat
        end_if
        pause  -- Stop in debugger
    end_debug
    
    return x * 2
end_function
```

**Compiler Behavior:**
- **Development mode (default):** Debug lines/blocks execute
- **Production mode (`--release` flag):** Debug completely removed at lexer level

**Benefits:**
- ✅ Single-line: `debug print(x)` - quick debug
- ✅ Multi-line: `debug ... end_debug` - complex debug logic
- ✅ Debug-only features: goto, label, pause
- ✅ Zero overhead in production binary

---

### 4. Functions

#### Function Declaration

```pmpl
function name(parameters) returns return_type
    statement
end_function
```

**Example:**
```pmpl
function add(numeric a, numeric b) returns numeric
    return a + b
end_function
```

#### Function with No Return

```pmpl
function greet(string name)
    print("Hello, " + name)
end_function
```

#### Multiple Parameters

```pmpl
function calculate(numeric x, numeric y, boolean verbose) returns numeric
    if verbose then
        print("Calculating...")
    end_if
    return x * y + x / y
end_function
```

---

### 5. Data Types

#### Type Keywords

```pmpl
numeric     -- Numbers (int/float)
string      -- Text strings
boolean     -- true/false
array       -- Arrays
list        -- Lists
tuple       -- Tuples
```

#### Variable Declaration

```pmpl
numeric x = 10
string name = "MELP"
boolean flag = true
array numbers = [1, 2, 3, 4, 5]
```

#### Type Inference

```pmpl
x = 42          -- Infers numeric
name = "Alice"  -- Infers string
```

---

### 6. Structures

#### Struct Definition

```pmpl
struct StructName
    type1 field1
    type2 field2
end_struct
```

**Example:**
```pmpl
struct Point
    numeric x
    numeric y
end_struct

struct Person
    string name
    numeric age
    boolean active
end_struct
```

#### Struct Usage

```pmpl
Point p
p.x = 10
p.y = 20

print(p.x + p.y)
```

### 7. Enumerations

#### Enum Definition

```pmpl
enum EnumName
    VALUE1
    VALUE2
    VALUE3
end_enum
```

**Example:**
```pmpl
enum Color
    RED
    GREEN
    BLUE
end_enum

enum Status
    PENDING
    ACTIVE
    COMPLETED
    FAILED
end_enum

function check_status(Status s) returns string
    if s == Status.ACTIVE then
        return "Running"
    else_if s == Status.COMPLETED then
        return "Done"
    else
        return "Other"
    end_if
end_function
```

---

### 8. Operators

#### Arithmetic

```pmpl
+       -- Addition
-       -- Subtraction
*       -- Multiplication
/       -- Division
%       -- Modulo
**      -- Exponentiation
```

#### Comparison

```pmpl
==      -- Equal
!=      -- Not equal
<       -- Less than
>       -- Greater than
<=      -- Less or equal
>=      -- Greater or equal
```

#### Logical

```pmpl
and     -- Logical AND
or      -- Logical OR
not     -- Logical NOT
xor     -- Bitwise XOR
```

#### Assignment

```pmpl
=       -- Assignment
```

---

### 8. Built-in Functions

```pmpl
print(expression)       -- Print without newline
println(expression)     -- Print with newline
```

---

## 🔧 Advanced Features

### Switch/Match Statements

```pmpl
switch value
    case 1:
        statement
    case 2:
        statement
    default:
        statement
end_switch
```

**Exit from case:**
```pmpl
switch x
    case 1:
        print("One")
        exit_switch     -- Break from switch
    case 2:
        print("Two")
end_switch
```

### Try-Catch (Experimental)

```pmpl
try
    risky_operation()
catch error
    handle_error(error)
end_try
```

---

## 📊 Complete Token Reference

### Keywords

| Keyword | Token | Usage |
|---------|-------|-------|
| `if` | TOKEN_IF | Conditional |
| `then` | TOKEN_THEN | If/while condition end |
| `else` | TOKEN_ELSE | Else clause |
| `else_if` | TOKEN_ELSE_IF | Else-if clause |
| `end_if` | TOKEN_END_IF | End if block |
| `while` | TOKEN_WHILE | While loop |
| `do` | TOKEN_DO | Loop body marker |
| `end_while` | TOKEN_END_WHILE | End while loop |
| `for` | TOKEN_FOR | For loop |
| `to` | TOKEN_TO | For range ascending |
| `downto` | TOKEN_DOWNTO | For range descending |
| `each` | TOKEN_EACH | For-each iterator |
| `in` | TOKEN_IN | For-each collection |
| `end_for` | TOKEN_END_FOR | End for loop |
| `function` | TOKEN_FUNCTION | Function declaration |
| `returns` | TOKEN_RETURNS | Return type |
| `return` | TOKEN_RETURN | Return statement |
| `end_function` | TOKEN_END_FUNCTION | End function |
| `exit` | TOKEN_EXIT | Exit block |
| `exit_if` | TOKEN_EXIT_IF | Exit if |
| `exit_for` | TOKEN_EXIT_FOR | Exit for loop |
| `exit_while` | TOKEN_EXIT_WHILE | Exit while loop |
| `exit_function` | TOKEN_EXIT_FUNCTION | Exit function |
| `exit_switch` | TOKEN_EXIT_SWITCH | Break switch |
| `continue` | TOKEN_CONTINUE | Continue loop (standalone) |
| `continue_for` | TOKEN_CONTINUE_FOR | Continue for |
| `continue_while` | TOKEN_CONTINUE_WHILE | Continue while |
| `debug` | TOKEN_DEBUG | Debug block start |
| `end_debug` | TOKEN_END_DEBUG | Debug block end |
| `enum` | TOKEN_ENUM | Enum declaration |
| `end_enum` | TOKEN_END_ENUM | End enum |
| `struct` | TOKEN_STRUCT | Struct declaration |
| `end_struct` | TOKEN_END_STRUCT | End struct |
| `true` | TOKEN_TRUE | Boolean true |
| `false` | TOKEN_FALSE | Boolean false |
| `and` | TOKEN_AND | Logical AND |
| `or` | TOKEN_OR | Logical OR |
| `not` | TOKEN_NOT | Logical NOT |
| `xor` | TOKEN_XOR | Bitwise XOR |
| `mod` | TOKEN_MOD | Modulo operator |
| `numeric` | TOKEN_NUMERIC | Numeric type |
| `string` | TOKEN_STRING_TYPE | String type |
| `boolean` | TOKEN_BOOLEAN | Boolean type |
| `array` | TOKEN_ARRAY | Array type |
| `list` | TOKEN_LIST | List type |
| `tuple` | TOKEN_TUPLE | Tuple type |
| `print` | TOKEN_PRINT | Print function |
| `println` | TOKEN_PRINTLN | Print line function |
| `import` | TOKEN_IMPORT | Import module |
| `module` | TOKEN_MODULE | Module declaration |

### Operators & Delimiters

| Symbol | Token | Type |
|--------|-------|------|
| `+` | TOKEN_PLUS | Arithmetic |
| `-` | TOKEN_MINUS | Arithmetic |
| `*` | TOKEN_MULTIPLY | Arithmetic |
| `/` | TOKEN_DIVIDE | Arithmetic |
| `%` | TOKEN_MOD | Arithmetic |
| `**` | TOKEN_POWER | Arithmetic |
| `==` | TOKEN_EQUAL | Comparison |
| `!=` | TOKEN_NOT_EQUAL | Comparison |
| `<` | TOKEN_LESS | Comparison |
| `>` | TOKEN_GREATER | Comparison |
| `<=` | TOKEN_LESS_EQUAL | Comparison |
| `>=` | TOKEN_GREATER_EQUAL | Comparison |
| `=` | TOKEN_ASSIGN | Assignment |
| `(` | TOKEN_LPAREN | Delimiter |
| `)` | TOKEN_RPAREN | Delimiter |
| `[` | TOKEN_LBRACKET | Delimiter |
| `]` | TOKEN_RBRACKET | Delimiter |
| `<` | TOKEN_LANGLE | Delimiter |
| `>` | TOKEN_RANGLE | Delimiter |
| `,` | TOKEN_COMMA | Delimiter |
| `:` | TOKEN_COLON | Delimiter |
| `;` | TOKEN_SEMICOLON | Delimiter |
| `.` | TOKEN_DOT | Delimiter |

---

## 🎨 Syntax Comparison Examples

### Example 1: If-Else

**C-style User Code:**
```c
if (x > 5) {
    printf("Big");
} else if (x > 2) {
    printf("Medium");
} else {
    printf("Small");
}
```

**Python-style User Code:**
```python
if x > 5:
    print("Big")
elif x > 2:
    print("Medium")
else:
    print("Small")
```

**Ruby-style User Code:**
```ruby
if x > 5
    puts "Big"
elsif x > 2
    puts "Medium"
else
    puts "Small"
end
```

**All normalize to PMPL:**
```pmpl
if x > 5 then
    print("Big")
else_if x > 2 then
    print("Medium")
else
    print("Small")
end_if
```

---

### Example 2: While Loop

**C-style:**
```c
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    i++;
}
```

**Python-style:**
```python
i = 0
while i < 10:
    print(i)
    i += 1
```

**PMPL:**
```pmpl
numeric i = 0
while i < 10 do
    print(i)
    i = i + 1
end_while
```

---

### Example 3: Function

**C-style:**
```c
int add(int a, int b) {
    return a + b;
}
```

**Python-style:**
```python
def add(a, b):
    return a + b
```

**PMPL:**
```pmpl
function add(numeric a, numeric b) returns numeric
    return a + b
end_function
```

---

## 🔍 Normalization Rules

The normalizer (`compiler/stage0/normalize/`) transforms user syntax to PMPL:

### Block Terminators

```
}           → end_if / end_while / end_for (context-based)
end if      → end_if
fi          → end_if
endif       → end_if
(dedent)    → end_if (Python-style)
```

### Keywords

```
else if     → else_if
elif        → else_if
elsif       → else_if
break       → exit
next        → continue (Ruby)
&&          → and
||          → or
!           → not
```

### Types

```
int         → numeric
float       → numeric
str         → string
bool        → boolean
```

---

## ⚙️ Implementation Guidelines

### Lexer Requirements

**Correct Implementation:**
```c
if (strcmp(lexeme, "end_if") == 0) {
    return create_token(TOKEN_END_IF);
}
```

**Incorrect (Old Pattern Matching):**
```c
if (tok->type == TOKEN_END) {
    Token* next = lexer_next();
    if (next->type == TOKEN_IF) {
        return TOKEN_END_IF;  // WRONG: Two tokens!
    }
}
```

### Parser Requirements

**Correct Switch-Case:**
```c
switch (token->type) {
    case TOKEN_END_IF:
        return;  // Simple!
    
    case TOKEN_ELSE_IF:
        parse_else_if();
        break;
}
```

**Avoid Pattern Matching:**
```c
// DON'T do this:
if (current == TOKEN_END && peek() == TOKEN_IF) {
    // Complex pattern matching
}
```

---

## 🧪 Testing & Validation

### Syntax Test Cases

All PMPL syntax must pass these test categories:

1. **Basic Statements**
   - Variable declarations
   - Assignments
   - Print statements

2. **Control Flow**
   - If/else_if/else chains
   - Nested conditionals
   - Early exit (exit_if)

3. **Loops**
   - While loops
   - For loops (to/downto)
   - For-each loops
   - Exit/continue variants

4. **Functions**
   - Declaration/definition
   - Parameters
   - Return values
   - Early return (exit_function)

5. **Structures**
   - Struct definitions
   - Member access
   - Nested structs

### Self-Hosting Test

PMPL compiler must compile PMPL code:

```pmpl
function parse_statement(Lexer lexer) returns Statement
    if current_token == TOKEN_IF then
        return parse_if(lexer)
    else_if current_token == TOKEN_WHILE then
        return parse_while(lexer)
    else_if current_token == TOKEN_FOR then
        return parse_for(lexer)
    end_if
    
    return null
end_function
```

---

## 📚 Related Documentation

- **Migration Guide:** `docs_tr/migration.md` - Multi-language normalization rules
- **Architecture:** `MELP_VISION.md` - Overall MELP design
- **Lexer Spec:** `compiler/stage0/modules/lexer/lexer.h` - Token definitions
- **Normalization:** `compiler/stage0/normalize/` - Syntax rules

---

## 📝 Version History

### 1.0 (December 14, 2025)
- Initial specification
- Block terminators with underscore notation
- Control flow keywords: `else_if`
- Loop control: `exit_X`, `continue_X`
- Complete token reference
- Self-hosting requirements

---

## 🤝 Contributing

When proposing new PMPL syntax:

1. **Underscore Rule:** Multi-word keywords use `_`
2. **Single Token:** Define corresponding `TOKEN_X` in lexer
3. **Parser Simplicity:** Ensure switch-case compatibility
4. **Normalization:** Add rules for user syntax → PMPL
5. **Self-Hosting:** Verify PMPL compiler can parse it
6. **Documentation:** Update this spec and examples

---

**Maintained by:** MELP Language Team  
**License:** Part of MELP Project  
**Status:** ✅ Stable - Production Ready

