# TTO Runtime (Transparent Type Optimization)

## 🎯 Amaç

MLP'nin otomatik tip optimizasyonu için runtime desteği sağlar.

## 📦 Özellikler

### Phase 1: Overflow Detection
```c
// INT64 taşma kontrolü
bool sto_would_overflow_add(int64_t a, int64_t b);
bool sto_safe_add_i64(int64_t a, int64_t b, int64_t* result);
```

### Phase 2: BigDecimal Library
```c
// Sınırsız hassasiyetli sayılar
BigDecimal* bigdec_from_i64(int64_t value);
BigDecimal* bigdec_add(BigDecimal* a, BigDecimal* b);
```

### Phase 3: Small String Optimization (SSO)
```c
// ≤23 byte string'ler stack'te
SSOString* sso_create(const char* str);
const char* sso_data(SSOString* str);
```

## 🔨 API

### Overflow Detection
- `sto_would_overflow_add(a, b)` - Toplama taşar mı?
- `sto_would_overflow_sub(a, b)` - Çıkarma taşar mı?
- `sto_would_overflow_mul(a, b)` - Çarpma taşar mı?

### BigDecimal Operations
- `bigdec_add(a, b)` - Toplama
- `bigdec_sub(a, b)` - Çıkarma
- `bigdec_mul(a, b)` - Çarpma
- `bigdec_div(a, b)` - Bölme
- `bigdec_compare(a, b)` - Karşılaştırma

### SSO String
- `sso_create(str)` - String oluştur
- `sso_data(str)` - String verisi
- `sso_concat(a, b)` - Birleştir
- `sso_free(str)` - Serbest bırak

## 📊 Performans

| Operasyon | INT64 | BigDecimal | Oran |
|-----------|-------|------------|------|
| Toplama | ~1ns | ~50ns | 50x |
| Çarpma | ~2ns | ~100ns | 50x |
| Bellek | Stack | Heap | - |

**Sonuç**: %99.9 durumda INT64 kullanılır, sadece overflow durumunda BigDecimal'e geçiş yapılır.

## 🧪 Test

```bash
cd tto
make test
./test_tto_runtime
```

## 📖 Örnek

```mlp
-- MLP kodu
sayi x = 9223372036854775800  -- INT64_MAX - 7
sayi y = 100

-- Derleyici otomatik tespit eder:
sayi toplam = x + y  -- Overflow! → BigDecimal'e yükselt
```

Üretilen kod:
```c
if (tto_would_overflow_add(x, y)) {
    BigDecimal* big_x = bigdec_from_i64(x);
    BigDecimal* big_y = bigdec_from_i64(y);
    BigDecimal* result = bigdec_add(big_x, big_y);
    // ...
}
```

---

*Detaylı implementasyon: sto_runtime.c (260+ satır)*
