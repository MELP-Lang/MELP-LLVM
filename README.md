# MELP - Modern Expression Language Platform

**Versiyon:** Stage 2 Bootstrap (Production Ready)  
**Tarih:** 16 Ocak 2026  
**Durum:** ✅ STABİL

MELP, LLVM backend'li modern, statik tipli bir programlama dilidir. Netlik, güvenlik ve performans için tasarlanmıştır.

---

## 🚀 Hızlı Başlangıç

### Gereksinimler

```bash
# Debian/Ubuntu
sudo apt install clang llvm

# macOS
brew install llvm

# Fedora
sudo dnf install clang llvm
```

### İlk Programınızı Derleyin

```bash
# 1. MELP kodu yazın (ornek.mlp)
cat > ornek.mlp << 'EOF'
function main() as numeric
    return 42
end_function
EOF

# 2. LLVM IR'ye derleyin
./compiler/stage2/stage2_bootstrap ornek.mlp -o ornek.ll

# 3. Çalıştırılabilir dosya oluşturun
clang ornek.ll -o ornek

# 4. Çalıştırın
./ornek
echo $?  # Çıktı: 42
```

---

## 📦 İçerik

### Stage 2 Compiler (Production)

**Konum:** `compiler/stage2/`  
**Binary:** `stage2_bootstrap` (62KB)  
**Testler:** 115/115 geçti ✅

**Desteklenen Özellikler:**
- ✅ Değişkenler (numeric, boolean)
- ✅ İfadeler (aritmetik, karşılaştırma, mantıksal)
- ✅ Kontrol akışı (if-else, while döngüleri)
- ✅ Fonksiyonlar (parametreler, return değerleri)
- ✅ Rekürsif fonksiyonlar (karşılıklı rekürsyon dahil)
- ✅ Forward fonksiyon çağrıları
- ✅ Statik tip kontrolü
- ✅ LLVM IR backend

**Stage 3'te Gelecek (~3 hafta içinde):**
- Diziler/Listeler
- String'ler
- Struct'lar
- Dosya I/O
- Standart kütüphane
- Self-hosting (MELP ile yazılmış MELP compiler'ı)

---

## 📚 Dokümantasyon

### Türkçe
- [MELP Felsefesi](belgeler/MELP_PHILOSOPHY.md) - Tasarım felsefesi
- [MELP Referans](belgeler/MELP_REFERENCE.md) - Dil rehberi
- [PMLP0 Sözdizimi](belgeler/pmlp0_sozdizimi.md) - Syntax klavuzu
- [Mimari](belgeler/ARCHITECTURE.md) - Mimari genel bakış
- [SAO İlkeleri](belgeler/SAO_PRINCIPLES.md) - Simple, Autonomous, Optimizable

### İngilizce
- [LLVM IR Guide](docs/LLVM_IR_GUIDE.md) - LLVM backend detayları
- [PMPL Reference](docs/PMPL_REFERENCE.md) - Dil referansı

---

## 🎯 Dil Örnekleri

### Merhaba Dünya (Return Değeri)

```melp
function main() as numeric
    return 0
end_function
```

### Değişkenler ve Aritmetik

```melp
function hesapla() as numeric
    var numeric x
    var numeric y
    x = 10
    y = 20
    return x + y
end_function

function main() as numeric
    return hesapla()
end_function
```

### Kontrol Akışı

```melp
function mutlak_deger(numeric x) as numeric
    if x < 0
        return -x
    else
        return x
    end_if
end_function

function main() as numeric
    return mutlak_deger(-42)
end_function
```

### Döngüler

```melp
function faktoriyel(numeric n) as numeric
    var numeric sonuc
    var numeric i
    sonuc = 1
    i = 1
    
    while i <= n
        sonuc = sonuc * i
        i = i + 1
    end_while
    
    return sonuc
end_function

function main() as numeric
    return faktoriyel(5)
end_function
```

### Rekürsif Fonksiyonlar

```melp
function fibonacci(numeric n) as numeric
    if n <= 1
        return n
    else
        return fibonacci(n - 1) + fibonacci(n - 2)
    end_if
end_function

function main() as numeric
    return fibonacci(10)
end_function
```

### Karşılıklı Rekürsyon

```melp
function cift_mi(numeric n) as numeric
function tek_mi(numeric n) as numeric

function cift_mi(numeric n) as numeric
    if n == 0
        return 1
    else
        return tek_mi(n - 1)
    end_if
end_function

function tek_mi(numeric n) as numeric
    if n == 0
        return 0
    else
        return cift_mi(n - 1)
    end_if
end_function

function main() as numeric
    return cift_mi(42)
end_function
```

---

## 🔧 Kaynak Koddan Derleme

### Compiler'ı Derleyin

```bash
cd compiler/stage2
./build_bootstrap.sh
```

Bu komut `stage2_bootstrap` binary'sini (62KB) oluşturur.

### Testleri Çalıştırın

```bash
cd tests/stage2
./run_integration_tests.sh      # 18/18 test
./run_bootstrap_validation.sh   # 10/10 test
```

---

## 🏗️ Proje Yapısı

```
MELP-LLVM/
├── compiler/
│   └── stage2/              # Production compiler (62KB)
│       ├── c_helpers/       # Modüler bileşenler
│       ├── stage2_bootstrap # Compiler binary
│       └── README.md        # Derleme talimatları
├── runtime/
│   ├── stdlib/              # Standart kütüphane (Stage 3)
│   └── sto/                 # Static Type Optimizer
├── examples/
│   ├── basics/              # Temel örnekler
│   └── hello/               # Merhaba dünya örnekleri
├── tests/
│   └── stage2/              # Compiler test suite
├── docs/                    # İngilizce dokümantasyon
├── belgeler/                # Türkçe dokümantasyon
└── scripts/                 # Build araçları
```

---

## 🧪 Test Etme

### Entegrasyon Testleri (18 test)

```bash
cd tests/stage2
./run_integration_tests.sh
```

**Test Kapsamı:**
- Basit return değerleri
- Değişkenler ve aritmetik
- If-else ifadeleri
- While döngüleri
- Çok fonksiyonlu programlar
- Rekürsyon
- Karşılıklı rekürsyon
- Tip kontrolü
- Hata yakalama

### Bootstrap Doğrulama (10 test)

```bash
cd tests/stage2
./run_bootstrap_validation.sh
```

End-to-end derleme pipeline'ını doğrular (MELP → LLVM IR → Çalıştırılabilir).

---

## 🎓 Dil Tasarımı

### Felsefe

MELP, **SAO (Simple, Autonomous, Optimizable)** ilkelerini takip eder:

1. **Simple (Basit):** Minimal syntax, net semantik
2. **Autonomous (Özerk):** Modüler mimari, peer-to-peer bileşenler
3. **Optimizable (Optimize Edilebilir):** LLVM backend dünya standartlarında optimizasyon sağlar

### Tip Sistemi

- **Statik tipleme:** Tüm tipler derleme zamanında bilinir
- **Tip çıkarımı:** Stage 3'te gelecek
- **Tip güvenliği:** Otomatik tip dönüşümü yok
- **Numeric:** 64-bit signed integer (`i64`)
- **Boolean:** True/false değerleri

### Sözdizimi

MELP netlik için açık anahtar kelimeler kullanır:

```melp
function isim(tip parametre) as donus_tipi
    var tip degisken_adi
    if kosul
        ...
    else
        ...
    end_if
    while kosul
        ...
    end_while
    return deger
end_function
```

---

## 🔬 Teknik Detaylar

### Compiler Pipeline

```
Kaynak Kod (.mlp)
    ↓
[Lexer] → Token'lar
    ↓
[Parser] → Abstract Syntax Tree (AST)
    ↓
[Semantic Analysis] → Doğrulanmış AST
    ↓
[Code Generation] → LLVM IR (.ll)
    ↓
[LLVM Toolchain] → Çalıştırılabilir
```

### Mimari

**Tasarım:** AUTONOMOUS peer-to-peer modüller
- Merkezi orchestrator yok
- Her modül kendi başına çalışır
- Bileşenler arası temiz arayüzler

**Modüller:**
- `lexer/` - Tokenization (18 test)
- `parser/` - AST oluşturma (20 test)
- `semantic/` - Tip kontrolü (28 test)
- `codegen/` - LLVM IR üretimi (31 test)

### Performans

- **Derleme Zamanı:** Küçük programlar için <1ms
- **Binary Boyutu:** 62KB compiler
- **Çalışma Zamanı:** Native makine kodu (LLVM optimizasyonları)

---

## 📈 Yol Haritası

### ✅ Stage 2 (Tamamlandı - 16 Ocak 2026)
- Modern bootstrap compiler
- LLVM IR backend
- Çekirdek dil özellikleri
- Production ready

### 🔄 Stage 3 (Sırada - ~3 hafta)
- **Self-hosting:** MELP ile yazılmış MELP compiler
- **Standart kütüphane:** String'ler, diziler, I/O
- **Tip çıkarımı:** Otomatik tip belirleme
- **Modül sistemi:** Import/export

### 🚀 Stage 4 (Gelecek)
- Gelişmiş optimizasyonlar
- Çoklu backend'ler
- Paket yöneticisi
- Tooling ekosistemi

---

## 🤝 Katkıda Bulunma

MELP aktif geliştirme aşamasındadır. Beklediğimiz katkılar:
- Hata raporları
- Özellik talepleri
- Dokümantasyon iyileştirmeleri
- Kod katkıları

Geliştirme kuralları için `belgeler/` dizinine bakın.

---

## 📄 Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

## 🔗 Kaynaklar

### Örnekler

`examples/` dizinine bakın:
- `examples/basics/` - Temel dil özellikleri
- `examples/hello/` - Merhaba dünya varyantları

### Testler

`tests/stage2/` dizinine bakın:
- Entegrasyon testleri
- Bootstrap doğrulama
- Hata yakalama testleri

### Dokümantasyon

- **Türkçe:** `belgeler/`
- **İngilizce:** `docs/`

---

## 🐛 Bilinen Kısıtlamalar (Stage 2)

### Henüz Uygulanmadı

- ❌ Diziler/Listeler
- ❌ String'ler
- ❌ Struct'lar
- ❌ Dosya I/O
- ❌ Standart kütüphane fonksiyonları
- ❌ Modül import'ları

**Bu özellikler Stage 3'te gelecek!**

### Mevcut Kapsam

Stage 2, **numeric hesaplamalar**a odaklanır:
- Matematik algoritmaları (faktöriyel, fibonacci)
- Rekürsif fonksiyonlar
- Kontrol akışı pattern'leri
- Tip kontrollü numeric işlemler

---

## 💡 Örnek Programlar

### Faktöriyel (İteratif)

```melp
function faktoriyel(numeric n) as numeric
    var numeric sonuc
    var numeric i
    sonuc = 1
    i = 1
    while i <= n
        sonuc = sonuc * i
        i = i + 1
    end_while
    return sonuc
end_function

function main() as numeric
    return faktoriyel(10)  # 3628800
end_function
```

### Fibonacci (Rekürsif)

```melp
function fib(numeric n) as numeric
    if n <= 1
        return n
    else
        return fib(n - 1) + fib(n - 2)
    end_if
end_function

function main() as numeric
    return fib(15)  # 610
end_function
```

### EBOB (Euclidean Algoritması)

```melp
function ebob(numeric a; numeric b) as numeric
    var numeric temp
    while b != 0
        temp = b
        b = a % b
        a = temp
    end_while
    return a
end_function

function main() as numeric
    return ebob(48, 18)  # 6
end_function
```

---

## 🎯 Hızlı Referans

### Derleme İş Akışı

```bash
# Adım 1: MELP → LLVM IR
./compiler/stage2/stage2_bootstrap girdi.mlp -o cikti.ll

# Adım 2: LLVM IR → Çalıştırılabilir
clang cikti.ll -o program

# Adım 3: Çalıştır
./program
```

### Sık Karşılaşılan Sorunlar

**S: "stage2_bootstrap: command not found"**  
C: Tam yol kullanın veya PATH'e ekleyin:
```bash
export PATH=$PATH:$(pwd)/compiler/stage2
```

**S: "clang: command not found"**  
C: LLVM toolchain kurun:
```bash
sudo apt install clang llvm
```

**S: "Parse failed" hataları**  
C: Syntax'ı kontrol edin - MELP satır sonlarını gerektirir:
```melp
# YANLIŞ:
return 42 end_function

# DOĞRU:
return 42
end_function
```

---

**MELP - Basit. Özerk. Optimize Edilebilir.** 🚀

*Son Güncelleme: 16 Ocak 2026*  
*Stage 2 Bootstrap - Production Ready*
