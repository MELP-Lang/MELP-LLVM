# MLP Runtime Kütüphaneleri

## 📚 Bileşenler

### 1. STO Runtime (Smart Type Optimization)
- Otomatik tip optimizasyonu
- Overflow tespiti ve BigDecimal'e yükseltme
- Small String Optimization (SSO)
- Bellek yönetimi

### 2. Memory Management
- Akıllı işaretçiler (smart pointers)
- Reference counting
- Ownership sistemi
- Bellek havuzları

### 3. Standard Library
- Temel veri yapıları
- String işlemleri
- Koleksiyonlar
- I/O fonksiyonları

## 🔧 Kullanım

Runtime kütüphaneleri, derlenmiş MLP programlarıyla otomatik olarak link edilir.

```bash
# Runtime'ı derle
cd runtime/sto
make

# Programınızla link et
ld program.o -L./runtime/sto -lsto_runtime -o program
```

## 📖 Dokümantasyon

Her alt dizinde detaylı README.md dosyaları bulunmaktadır:
- `sto/README.md` - STO runtime detayları
- `memory/README.md` - Bellek yönetimi
- `stdlib/README.md` - Standart kütüphane API'si
