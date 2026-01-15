# MELP String Support Guide

**Status:** 85% Complete (Phase 17)  
**Updated:** 13 Aralık 2025 (YZ_65)

---

## 📖 String Features Overview

### ✅ Currently Working

#### 1. String Literals
```mlp
print("Hello World")
print("UTF-8: Merhaba Dünya 🎉")
print("Escape sequences: \n\t\\\"")
```

#### 2. String Variables
```mlp
string greeting = "Hello"
string name = "MELP"
print(greeting)
print(name)
```

#### 3. String Function Parameters
```mlp
function greet(string message) returns numeric
    print(message)
    return 0
end

function main() returns numeric
    greet("Hello from function!")
    return 0
end
```

#### 4. String Arguments (Literals)
```mlp
function show(string first, string second) returns numeric
    print(first)
    print(second)
    return 0
end

function main() returns numeric
    show("Hello", "World")
    return 0
end
```

#### 5. String Arguments (Variables)
```mlp
function greet(string msg) returns numeric
    print(msg)
    return 0
end

function main() returns numeric
    string message = "MELP is awesome!"
    greet(message)  # ✅ Works!
    return 0
end
```

#### 6. Mixed Arguments
```mlp
function log(string prefix, string data, string suffix) returns numeric
    print(prefix)
    print(data)
    print(suffix)
    return 0
end

function main() returns numeric
    string userName = "Alice"
    string action = "logged in"
    
    log("[INFO]", userName, action)           # All literals
    log("User:", userName, "- status: OK")    # Mixed
    return 0
end
```

---

## ❌ Not Yet Implemented

### String Operations

#### Concatenation
```mlp
# ❌ Not yet working
string greeting = "Hello"
string name = "MELP"
string message = greeting + " " + name  # TODO: YZ_66
```

#### Comparison
```mlp
# ❌ Not yet working
string name = "MELP"
if name == "MELP" then  # TODO: YZ_66
    print("Match!")
end if
```

#### String Methods
```mlp
# ❌ Not yet working
string text = "Hello World"
numeric len = length(text)              # TODO
string sub = substring(text, 0, 5)      # TODO
numeric pos = indexOf(text, "World")    # TODO
```

---

## 🔧 LLVM IR Internals

### String Literal
```llvm
@.str.1 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
```

### String Variable Declaration
```llvm
%greeting_ptr = alloca i8*, align 8
%tmp1 = getelementptr inbounds [6 x i8], [6 x i8]* @.str.1, i64 0, i64 0
store i8* %tmp1, i8** %greeting_ptr, align 8
```

### String Variable Usage (as argument)
```llvm
%tmp2 = load i8*, i8** %greeting_ptr, align 8
%tmp3 = call i64 @greet(i8* %tmp2)
```

### Function with String Parameter
```llvm
define i64 @greet(i8* %msg) {
entry:
  call void @mlp_println_string(i8* %msg)
  ret i64 0
}
```

---

## 📝 Type System

### Variable Types

```c
// Internal representation
typedef enum {
    VAR_NUMERIC,   // i64 in LLVM
    VAR_STRING,    // i8* in LLVM
    VAR_BOOLEAN,   // i64 (0 or 1) in LLVM
    VAR_ARRAY,
    VAR_LIST,
    VAR_TUPLE
} VariableType;
```

### LocalVariable Registry

```c
typedef struct LocalVariable {
    char* name;
    int is_numeric;      // 1 = numeric/boolean, 0 = string
    int is_array;
    int stack_offset;
    struct LocalVariable* next;
} LocalVariable;
```

**Type Mapping:**
- `VAR_STRING` → `is_numeric = 0` → LLVM type: `i8*`
- `VAR_NUMERIC` → `is_numeric = 1` → LLVM type: `i64`
- `VAR_BOOLEAN` → `is_numeric = 1` → LLVM type: `i64`

---

## 🧪 Testing

### Compile with LLVM Backend
```bash
cd compiler/stage0/modules/functions
./functions_compiler --backend=llvm -c input.mlp output.ll
```

### Compile LLVM IR to Binary
```bash
clang output.ll \
  -L../../../../runtime/stdlib -lmlp_stdlib \
  -L../../../../runtime/sto -lsto_runtime \
  -o program
```

### Run
```bash
LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/sto ./program
```

---

## 📚 Example Programs

### Example 1: Simple Greeting
```mlp
function main() returns numeric
    string name = "MELP"
    print("Hello")
    print(name)
    return 0
end
```

**Output:**
```
Hello
MELP
```

### Example 2: Function with String Parameters
```mlp
function greet(string greeting, string name) returns numeric
    print(greeting)
    print(name)
    return 0
end

function main() returns numeric
    greet("Welcome to", "MELP Language")
    return 0
end
```

**Output:**
```
Welcome to
MELP Language
```

### Example 3: Mixed Literal and Variable
```mlp
function log(string level, string message) returns numeric
    print(level)
    print(message)
    return 0
end

function main() returns numeric
    string error = "File not found"
    log("[ERROR]", error)
    log("[INFO]", "System started")
    return 0
end
```

**Output:**
```
[ERROR]
File not found
[INFO]
System started
```

---

## 🐛 Known Limitations

1. **No String Concatenation:** Cannot use `+` operator with strings yet
2. **No String Comparison:** Cannot use `==`, `!=` with strings yet
3. **No String Methods:** No length(), substring(), indexOf() etc.
4. **No String Interpolation:** No `"Hello ${name}"` syntax
5. **No String Mutation:** Strings are immutable (by design)
6. **No Heap Allocation:** All string literals are compile-time constants
7. **No String Return Values:** Functions cannot return strings yet

---

## 🎯 Roadmap

### Phase 17 Remaining (15%)

- [ ] String concatenation (`x + y`)
- [ ] String comparison (`x == y`, `x != y`)
- [ ] String methods (length, substring, indexOf)
- [ ] String return values from functions
- [ ] String arrays (`array<string>`)

### Future Enhancements

- [ ] String interpolation
- [ ] Regular expressions
- [ ] String builder (efficient concatenation)
- [ ] Unicode normalization
- [ ] String formatting

---

## 📖 Related Documentation

- **YZ/YZ_61.md** - Phase 15 & 17 Part 1 (Print statement, string literals)
- **YZ/YZ_62.md** - Phase 17 Part 2 (String variables)
- **YZ/YZ_63.md** - Phase 17 Part 3 (String parameters)
- **YZ/YZ_64.md** - Phase 17 Part 4 (String literal arguments)
- **YZ/YZ_65.md** - Phase 17 Part 5 (String variable arguments)
- **TODO.md** - Current project status
- **NEXT_AI_START_HERE.md** - Next session guide

---

**Last Updated:** 13 Aralık 2025 by YZ_65  
**Status:** Phase 17 - 85% Complete
