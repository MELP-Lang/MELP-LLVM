# Smart Type Optimization (STO) Prensipleri

**Tarih:** 25 Aralık 2025  
**Kaynak:** `docs/specs/kurallar_kitabı_v1.md`  
**Amaç:** MLP'nin mimari prensiplerinden STO'nun açıklaması

---

## 🎯 Temel Felsefe

MLP'de kullanıcıya sadece **2 temel tip** sunulur:
- `numeric` - Tüm sayılar (tam sayı, ondalık, büyük sayılar)
- `string` - Tüm metinler (kısa, uzun, sabit)

**Ancak arka planda**, compiler otomatik olarak en verimli dahili temsili seçer. Kullanıcı bunu bilmez ve bilmesine gerek yoktur.

---

## 📊 Neden Bu Yaklaşım?

**Gözlem 1:** Diğer dillerde BigDecimal aslında string-tabanlıdır  
**Gözlem 2:** MLP'de sadece numeric ve string var → aslında tek tip: "veri"  
**Gözlem 3:** "Ali" ile 10.000 sayfalık kitap aynı bellek stratejisini kullanmamalı  
**Gözlem 4:** Kullanıcı pragmatik katmanı görmüyor → arka plan optimizasyonu mümkün

---

## 🔄 MLP Derleme Zinciri ve STO'nun Yeri

**MLP = Multi Language Programming**

MLP'nin temel felsefesi: Kullanıcı istediği sözdiziminde (C, Python, kendi özel sözdizimi), istediği dilde (Türkçe, İngilizce, Hintçe, kendi özel dili) kod yazabilir.

```
Kullanıcı Kodu                    Kullanıcı Görür
     ↓
[diller.json + syntax.json]       Kullanıcı Görür ve Düzenleyebilir (*)
     ↓
Normalize → Pragmatik MLP         Kullanıcı Görmez (**)
     ↓
Pragmatik MLP (English Base)      Kullanıcı Görmez
     ↓
[Lexer → Parser]                  Kullanıcı Görmez
     ↓
[STO: Tip Analizi]  ←←←←←←←←←←←  BURADA OPTİMİZASYON YAPILIR
     ↓
[Codegen: Optimize Assembly]      Kullanıcı Görmez
     ↓
x86-64 Binary                     Kullanıcı Çalıştırır

(*) diller.json: Dil çevirileri (Türkçe "yazdır" → İngilizce "print")
    syntax.json: Sözdizimi varyasyonları (Python-like, C-like, custom)
    Kullanıcı bu dosyaları düzenleyerek kendi dilini/sözdizimini ekleyebilir!

(**) Normalize katmanı:
    - Kullanıcının sözdizimini alır
    - Kullanıcının dilini alır  
    - Her ikisini de Pragmatik MLP (İngilizce base) diline çevirir
    - Bundan sonra derleyici standart şekilde çalışır
```

---

## 📋 Dahili Tip Dönüşüm Tablosu

### Numeric İçin:

| Kullanıcı Yazar | Değer Aralığı | Dahili Temsil | Nerede? | Performans |
|-----------------|---------------|---------------|---------|------------|
| `numeric x = 42` | -2^63 to 2^63-1 | int64 | register/stack | ⚡ En hızlı |
| `numeric y = 3.14` | ~15 digit hassasiyet | double | xmm register | ⚡ Hızlı |
| `numeric z = 10^100` | Sınırsız | BigDecimal | heap | 🐢 Yavaş ama güvenli |

### String İçin:

| Kullanıcı Yazar | Uzunluk | Dahili Temsil | Nerede? | Performans |
|-----------------|---------|---------------|---------|------------|
| `string s = "Ali"` | ≤23 byte | SSO (inline) | stack | ⚡ En hızlı |
| `string t = "Uzun metin..."` | >23 byte | heap pointer | heap | 🔄 Normal |
| `string c = "Sabit"` | Sabit | .rodata | readonly | ⚡ Paylaşımlı |

**SSO = Small String Optimization:** Kısa stringler heap allocation olmadan doğrudan stack'te saklanır.

---

## 🔧 Compile-Time Analiz Algoritması

```
function analyze_numeric(value):
    if value tam_sayı AND -2^63 ≤ value ≤ 2^63-1:
        return INT64          -- Register'da tutulacak
    else if value ondalık AND digits ≤ 15:
        return DOUBLE         -- XMM register'da tutulacak
    else:
        return BIGDECIMAL     -- Heap'te tutulacak

function analyze_string(value):
    if is_constant(value):
        return RODATA_STRING  -- .rodata section'da
    else if length(value) ≤ 23:
        return SSO_STRING     -- Stack'te inline
    else:
        return HEAP_STRING    -- Heap'te, pointer stack'te
```

---

## ⚠️ Runtime Overflow Handling

```mlp
numeric x = 9223372036854775807  -- Max int64
x = x + 1                         -- OVERFLOW!

-- Otomatik promote:
-- 1. Overflow detect edilir
-- 2. x BigDecimal'e dönüştürülür  
-- 3. İşlem BigDecimal ile devam eder
-- 4. Kullanıcı hiçbir şey farketmez
```

---

## 🚨 STO İHLAL ÖRNEKLERİ

### ❌ İhlal 1: Kullanıcıya Dahili Tipleri Açığa Çıkarmak

```mlp
-- YANLIŞ!
function get_type(numeric x) returns string
    if x is int64 then              -- ❌ int64 kullanıcıya görünmemeli!
        return "integer"
    else if x is double then        -- ❌ double kullanıcıya görünmemeli!
        return "float"
    end_if
end_function
```

**Neden İhlal:** STO'nun amacı dahili optimizasyonu kullanıcıdan gizlemek. Kullanıcı sadece `numeric` görmelidir.

### ❌ İhlal 2: Tip Dönüşümü Zorunluluğu

```mlp
-- YANLIŞ!
numeric x = to_int64(42)           -- ❌ Kullanıcı dönüşüm yapmamalı!
numeric y = to_double(3.14)        -- ❌ Compiler otomatik seçmeli!
```

**Neden İhlal:** Compiler zaten en verimli temsili seçer. Kullanıcıdan explicit dönüşüm istemek STO'ya aykırıdır.

### ❌ İhlal 3: Tip-Spesifik Fonksiyonlar

```mlp
-- YANLIŞ!
function add_int64(numeric a, numeric b) returns numeric
    -- int64 için optimize edilmiş toplama
end_function

function add_bigdecimal(numeric a, numeric b) returns numeric
    -- BigDecimal için toplama
end_function
```

**Neden İhlal:** Kullanıcı sadece genel `add` görmeli. Compiler hangi dahili temsili kullanacağına karar verir.

### ✅ Doğru Kullanım

```mlp
-- DOĞRU!
numeric x = 42                     -- ✅ Compiler int64 seçer
numeric y = 3.14                   -- ✅ Compiler double seçer  
numeric z = 10^100                 -- ✅ Compiler BigDecimal seçer

numeric result = x + y + z         -- ✅ Compiler otomatik promote yapar
```

---

## 🎯 ÖZET: STO Prensipleri

1. **Kullanıcı Sadece `numeric` ve `string` Görür**
   - Dahili temsiller (int64, double, BigDecimal, SSO) gizlidir

2. **Compiler Otomatik Optimize Eder**
   - Değer aralığı analizi
   - En verimli temsili seçer
   - Runtime'da gerekirse promote eder

3. **Kullanıcı Dönüşüm Yapmaz**
   - `to_int64()`, `is_double()` gibi fonksiyonlar yoktur
   - Kullanıcı sadece değerle ilgilenir

4. **Performans Şeffaftır**
   - Küçük değerler hızlı (register)
   - Büyük değerler güvenli (heap)
   - Kullanıcı fark etmez

---

**Versiyon:** 1.0  
**Son Güncelleme:** 25 Aralık 2025
