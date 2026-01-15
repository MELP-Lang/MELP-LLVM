# PMLP1 SÖZDİZİMİ - SIMPLIFIED (STAGE 1)

**Versiyon:** 1.0  
**Tarih:** 12 Ocak 2026  
**Stage:** Stage 1 = PMLP0 + Normalizer Infrastructure  
**Durum:** Simplified Approach (ÜA Approved)

---

## 🎯 KARAR: PMLP1 = PMLP0 (STAGE 1)

**Rationale:**
```
Self-hosting FIRST, features LATER!
```

**ÜA Approval:** ✅ Onaylandı (12 Ocak 2026)

**Yaklaşım:**
- ✅ PMLP1 = PMLP0 (functionally identical)
- ✅ Normalizer module (pass-through, identity function)
- ✅ Timeline: 8 weeks → 6 weeks (risk azalması)
- ✅ Feature creep riskini minimize et

---

## 📊 STAGE 1: PMLP1 = PMLP0

**✅ SYNTAX REFERANSI:** [pmlp0_sozdizimi.md](pmlp0_sozdizimi.md)

### Kritik Syntax Kuralları (PMLP0'dan)

**1. Semicolon (`;`) SADECE AYIRICI:**
```pmlp0
-- ✅ DOĞRU: Parametre ayırıcı
function test(numeric a; numeric b) as numeric
    return a + b
end_function

-- ✅ DOĞRU: Array/List elemanları (son eleman SONRA ; YOK)
numeric[] arr = [1; 2; 3]
list data = (5; "Ali"; true)

-- ✅ DOĞRU: Değişken tanımlama (semicolon YOK)
numeric x = 42
string name = "Ali"

-- ❌ YANLIŞ: Satır sonu semicolon
numeric y = 10;    -- YANLIŞ!
```

**2. Blok Sonlandırıcılar:**
```pmlp0
end_if
end_while
end_for
end_function
end_struct
```

**3. Türk Sayı Formatı:**
```pmlp0
numeric x = 3,14         -- virgül = ondalık
numeric y = 1.000.000    -- nokta = binlik
```

### Neden Bu Karar?

**1. PMLP0 Yeterli:**
- ✅ Turing-complete
- ✅ Stage 0 compiler'ı C'de 6500 satır → PMLP0'da ~8000 satır
- ✅ Struct, file I/O, import/export mevcut
- ✅ Control flow, functions, collections mevcut

**2. Risk Yönetimi:**
- ⚠️ Feature creep = biggest risk
- ⚠️ 6 feature eklemek = scope 2x artıyor
- ✅ Minimal scope = success guarantee

**3. Pragmatik Yaklaşım:**
- ✅ Self-hosting priority (önce derlesin kendini!)
- ✅ Features daha sonra eklenebilir (Stage 2+)
- ✅ Infrastructure hazır (normalizer module)

**4. Timeline:**
- ❌ 8 weeks (6 feature ile)
- ✅ 6 weeks (PMLP0 ile)

---

## 🏗️ NORMALIZER MODULE (STAGE 1)

**Amaç:** Future-proof architecture (Stage 2+ için hazırlık)

**Stage 1 Implementation:**
```pmlp0
-- normalizer.mlp (Stage 1 - Pass-through)

function normalize(source: string) as string
    -- Stage 1: Identity function (no transformation)
    -- Simply validates and returns source unchanged
    as source
end_function

function validate_syntax(source: string) as boolean
    -- Optional: Basic syntax check before passing through
    -- Stage 1: Minimal validation (always true)
    as true
end_function
```

**Önemli:**
- ✅ PMLP0 ile yazılmış (Stage 1 compiler PMLP0 ile yazılıyor!)
- ✅ Pass-through olsa da test edilmeli
- ✅ Module interface tanımlanmış (input: string, output: string)
- ✅ Stage 2'de buraya transformations eklenecek

**Architecture (Stage 1):**
```
PMLP0/PMLP1 Source → Normalizer (pass-through) → Lexer → Parser → Codegen → LLVM IR
```

**Architecture (Stage 2+):**
```
PMLP0 Source → Normalizer (pass-through) → Lexer → Parser → Codegen → LLVM IR
PMLP1 Source → Normalizer (transform) → PMLP0 → Lexer → Parser → Codegen → LLVM IR
```

---

## 🚀 FUTURE: STAGE 2+ FEATURES (ROADMAP)

**Stage 2'de Eklenebilecek Features:**

### 1. Default Parameters (`:=`)
```pmlp1
function create_array(size: numeric; fill_value := 0) as numeric[]
    -- Implementation
end_function

-- Calls
create_array(10)       -- fill_value = 0 (default)
create_array(10; 5)    -- fill_value = 5
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
create_array(10)

-- PMLP0 output (after normalization):
create_array(10; 0)  -- Default value injected
```

---

### 2. Named Parameters
```pmlp1
Window w = create_window(width: 800; height: 600; visible: false)
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
create_window(height: 600; width: 800)  -- Order doesn't matter!

-- PMLP0 output (after normalization):
create_window(800; 600)  -- Positional order restored
```

---

### 3. Ref Parameters (`ref`, `const ref`)
```pmlp1
function process_large(ref data: LargeStruct) as none
    data.field = 42  -- Modifies original
end_function

function read_only(const ref data: LargeStruct) as numeric
    as data.field  -- No modification allowed
end_function
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
process_large(ref my_data)

-- PMLP0 output (after normalization):
-- Pass pointer, dereference inside function
-- (Implementation detail in codegen)
```

---

### 4. Struct Methods (Syntax Sugar)
```pmlp1
struct Point
    numeric x
    numeric y
    
    function distance(other: Point) as numeric
        -- self implicit first parameter
        numeric dx = self.x - other.x
        numeric dy = self.y - other.y
        as sqrt(dx * dx + dy * dy)
    end_function
end_struct

-- Usage
Point p1 = Point{10; 20}
Point p2 = Point{30; 40}
numeric d = p1.distance(p2)  -- Method syntax!
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
p1.distance(p2)

-- PMLP0 output (after normalization):
Point_distance(p1; p2)  -- Free function call
```

---

### 5. Enhanced Import (Selective)
```pmlp1
from lexer import Token, tokenize, TokenType
from parser import Parser, parse_file
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
from lexer import Token, tokenize

-- PMLP0 output (after normalization):
import lexer
-- Access: lexer.Token, lexer.tokenize
-- (Selective imports → namespace access)
```

---

### 6. Enum Types
```pmlp1
enum TokenType
    NUMBER
    STRING
    IDENTIFIER
    OPERATOR
end_enum

function get_token_name(type: TokenType) as string
    if type == TokenType.NUMBER then
        as "Number"
    else if type == TokenType.STRING then
        as "String"
    end_if
end_function
```

**Normalizer Transformation:**
```pmlp0
-- PMLP1 input:
TokenType.NUMBER

-- PMLP0 output (after normalization):
0  -- Enum → numeric constant mapping
```

---

## 📐 PMLP0 COMPATIBILITY

**Stage 1'de PMLP1 = PMLP0:**
- ✅ Her PMLP0 kodu Stage 1 compiler'da çalışır
- ✅ Normalizer pass-through (no transformation)
- ✅ Backward compatible %100

**Stage 2+'da:**
- ✅ Her PMLP0 kodu hala çalışır (normalizer pass-through)
- ✅ PMLP1 syntax → PMLP0 dönüşümü (normalizer transforms)
- ✅ Backward compatible %100

---

## 📊 PMLP0 vs PMLP1 COMPARISON

| Feature | PMLP0 (Stage 0-1) | PMLP1 (Stage 2+) |
|---------|-------------------|------------------|
| Functions | ✅ Basic | ✅ + Default params |
| Parameters | ✅ Positional | ✅ + Named |
| Pass-by | ✅ Value only | ✅ + Ref/const ref |
| Struct | ✅ Data only | ✅ + Methods (sugar) |
| Import | ✅ Full module | ✅ + Selective |
| Enum | ❌ | ✅ Type-safe constants |
| Generics | ❌ | ❌ (Stage 3+) |
| Lambda | ❌ | ❌ (Stage 3+) |
| Pattern Match | ❌ | ❌ (Stage 3+) |
| Try-Catch | ❌ | ❌ (Stage 3+) |

---

## 🎓 DESIGN PRINCIPLES

### 1. Backward Compatibility
```
Every PMLP0 code MUST work in PMLP1 compiler (all stages)
```

### 2. Normalizer as Transformer
```
PMLP1 = Syntax Sugar
PMLP0 = Core Language
Normalizer = PMLP1 → PMLP0 transformer
```

### 3. Incremental Features
```
Stage 1: Infrastructure (normalizer)
Stage 2: Features (default params, ref)
Stage 3: Advanced (generics, lambda)
```

### 4. No Core Changes
```
PMLP0 parser NEVER changes!
All new syntax handled in normalizer
```

---

## 🚦 STAGE ROADMAP

### Stage 0 (C Implementation):
- ✅ PMLP0 support only
- ✅ No normalizer
- ✅ 26 core modules

### Stage 1 (PMLP0 Self-Hosting):
- ✅ PMLP0 = PMLP1 (functionally identical)
- ✅ Normalizer module (pass-through)
- ✅ 14 modules (~8K lines)
- ✅ Timeline: 6 weeks

### Stage 2 (PMLP1 Features):
- ✅ Default parameters
- ✅ Named parameters
- ✅ Ref parameters
- ✅ Struct methods
- ✅ Enhanced import
- ✅ Enum types
- ✅ Normalizer transforms (PMLP1 → PMLP0)

### Stage 3+ (Advanced Features):
- Generics/Templates
- Pattern matching
- Lambda functions
- Try-catch-finally
- Operator overloading
- Union types

---

## ✅ STAGE 1 DELIVERABLES

**Documentation:**
- ✅ `belgeler/pmlp1_sozdizimi.md` (this file - simplified)
- ✅ `STAGE1_ARCHITECTURE.md` (14 modules + normalizer)
- ✅ `YZ_15_COMPLETION_REPORT.md` (planning summary)

**Code (Stage 1 Implementation):**
- `normalizer.mlp` (~100 lines, pass-through)
- 13 other modules (lexer, parser, codegen, etc.)

**Tests:**
- Normalizer pass-through test (PMLP0 input → PMLP0 output)
- Backward compatibility test (all Stage 0 tests pass)
- Self-hosting test (Stage 1 compiles itself)

---

## 🎯 SUCCESS CRITERIA

**Stage 1 tamamlandı diyebilmek için:**

1. ✅ Stage 1 compiler PMLP0 ile yazılmış
2. ✅ Stage 1 compiler kendini derleyebiliyor (self-hosting!)
3. ✅ Stage 1 compiler tüm Stage 0 testlerini geçiyor (backward compatible)
4. ✅ Normalizer module implemented (pass-through only)
5. ✅ Timeline: 6 weeks içinde tamamlandı

**Stage 2'ye geçiş kriterleri:**
1. ✅ Stage 1 production-ready
2. ✅ Feature prioritization yapıldı (hangi feature önce?)
3. ✅ Normalizer transformation strategy belirlendi

---

## 📖 KAYNAKLAR

**Referanslar:**
- `belgeler/pmlp0_sozdizimi.md` - PMLP0 base specification
- `belgeler/MELP_PHILOSOPHY.md` - Design principles (6 TEMEL ESAS)
- `STAGE1_ARCHITECTURE.md` - Stage 1 compiler design
- `POC_RESULTS.md` - Stage 1 path decision rationale

**Stage 0 Implementation:**
- `compiler/stage0/` - C implementation (reference)
- 26 core modules (~6500 lines C)

---

## 🎉 SONUÇ

**PMLP1 Simplified Approach:**
```
Stage 1: PMLP1 = PMLP0 + Normalizer (pass-through)
         Self-hosting öncelikli
         Timeline: 6 weeks
         Risk: MINIMAL

Stage 2+: PMLP1 features ekle
          Normalizer transforms implement et
          Incremental development
```

**ÜA Approval:** ✅ Onaylandı (12 Ocak 2026)

**Next:** YZ_16 (Stage 0 Analysis) → YZ_17-22 (Stage 1 Implementation)

---

**Belge Tarihi:** 12 Ocak 2026  
**Hazırlayan:** YZ_15  
**Durum:** ✅ COMPLETE (Simplified)

🚀 **Pragmatic, Minimal, Successful!**
