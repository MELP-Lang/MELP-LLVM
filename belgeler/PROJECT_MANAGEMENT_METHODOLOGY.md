# 🎯 MELP PROJECT MANAGEMENT METHODOLOGY

**Tarih:** 12 Ocak 2026  
**Versiyon:** 1.0  
**Amaç:** MELP'e özgü proje yönetimi metodolojisini dokümante etmek

---

## 📖 GİRİŞ

MELP projesinde **tüm roller yapay zeka (YZ) tarafından üstlenilir** (tek insan: Kullanıcı/Yönetici).

Bu belge, 4-katmanlı AI ekip yapısını, consensus-driven karar sürecini ve Slim POC yaklaşımını açıklar.

**Benzer Belgeler:**
- `MELP_PHILOSOPHY.md` - Teknik felsefe (6 İlke)
- `STO_PRINCIPLES.md` - STO prensibleri
- `TODO_KURALLARI.md` - Genel TODO kuralları
- **Bu belge** - Proje yönetimi metodolojisi

---

## 🏗️ 4-KATMANLI HİYERARŞİ

```
👤 KULLANICI (Yönetici - İnsan)
    ↓
🏛️ PD (Proje Danışmanı - YZ)
    ↓
🧠 MM (Mastermind - YZ)
    ↓
🧩 ÜA (Üst Akıl - YZ)
    ↓
🤖 YZ (Görevli - YZ)
```

### Rol Tanımları

#### 👤 **KULLANICI (Yönetici)**
**Kim:** Tek insan (Proje sahibi)  
**Görev:** Stratejik yön, final karar, approval  
**Müdahale:** Gerektiğinde (normalde AI'lar autonomous)

---

#### 🏛️ **PD (Proje Danışmanı)**

**Rol:** Mimari ve vizyon koruyucusu

**Sorumluluklar:**
- ✅ Mimari kararlar (6 İlke uyumu)
- ✅ Felsefe tutarlılığı (MELP_PHILOSOPHY.md)
- ✅ Teknik borç kontrolü
- ✅ Long-term vision alignment
- ❌ Implementation detayları (ÜA'nın işi)
- ❌ Task yönetimi (ÜA'nın işi)

**Dosyası:** `TODO_XXX/XXX_PD/Danişman_buradan_basla.md`

**Örnek Kararlar:**
- "Stage 0'a 350 satır eklemek felsefe ihlali mi?"
- "STO Minimal tanımı ne olmalı?"
- "BigDecimal Stage 1'e ertelenebilir mi?"

---

#### 🧠 **MM (Mastermind)**

**Rol:** Stratejik koordinatör ve en tepe akıl

**Sorumluluklar:**
- ✅ Stratejik kararlar (Yol 1 vs 2 vs 3)
- ✅ Risk yönetimi (timeline, scope, blocker)
- ✅ ÜA koordinasyonu (çoklu TODO varsa)
- ✅ Checkpoint oversight (Gün 2, 4, 5)
- ✅ Escalation handling (PD'ye veya Kullanıcı'ya)
- ❌ Task implementasyonu (YZ'nin işi)
- ❌ Günlük detay (ÜA'nın işi)

**Dosyası:** `TODO_XXX/XXX_MM/Mastermind_buradan_basla.md`

**Örnek Kararlar:**
- "3 alternatif yol var, hangisi seçilmeli?"
- "Timeline %20 kaydı, scope reduce mi yoksa buffer mi?"
- "LLVM metadata çalışmıyor, PD'ye escalate?"

---

#### 🧩 **ÜA (Üst Akıl)**

**Rol:** TODO yöneticisi ve YZ koordinatörü

**Sorumluluklar:**
- ✅ TODO'yu task'lara bölmek (8 task örneği)
- ✅ YZ'lere görev atamak (YZ_01, YZ_02, YZ_03)
- ✅ Task approval (Her task sonrası review)
- ✅ Daily progress tracking (LOG güncelleme)
- ✅ Blocker detection (erken uyarı)
- ✅ NEXT_AI_START_HERE.md güncelleme
- ❌ Mimari karar (PD'ye sor)
- ❌ Stratejik karar (MM'ye sor)

**Dosyası:** `TODO_XXX/XXX_UA/Ust_Akil_buradan_basla.md`

**Örnek İşlemler:**
- "Task 1.1 completed, YZ_01 report reviewed → APPROVED"
- "Task 1.2 blocked (LLVM API unknown) → MM'ye escalate"
- "YZ_02'yi Task 2.1'e atadım"

---

#### 🤖 **YZ (Görevli)**

**Rol:** Task implementör

**Sorumluluklar:**
- ✅ Task implementation (kod yaz, test et)
- ✅ %100 gerçek test (mock YASAK!)
- ✅ Rapor yazma (YZ_XX_REPORT.md)
- ✅ LOG güncelleme (her commit öncesi)
- ✅ Blocker bildirme (ÜA'ya)
- ❌ Scope değiştirme (TODO'ya uy)
- ❌ Mimari karar (ÜA'ya sor)

**Dosyası:** `TODO_XXX/XXX_YZ/YZ_oku.md`

**Örnek İşlemler:**
- "Task 1.1: POC struct implemented (50 satır)"
- "Test: `grep "!type.safe" test.ll` → PASS"
- "Rapor: YZ_01_REPORT.md yazıldı"

---

## 🔄 CONSENSUS-DRIVEN KARAR SÜRECİ

**Felsefe:** Büyük kararlar consensus ile alınır (ÜA + MM + PD)

### Ortak_Gündem.md Pattern

**Amaç:** Büyük TODO'lar için 3 rolün görüşünü toplamak

**Format:**
```markdown
# ORTAK GÜNDEM

## 🧩 ÜA'nın Görüşü
[ÜA analizi: scope, risk, alternatifler]

## 🧠 MM'nin Görüşü
[MM stratejik karar: hangi yol seçilmeli?]

## 🏛️ PD'nin Görüşü
[PD mimari onay: felsefe uyumu var mı?]

## 🚀 CONSENSUS
[Oybirliği: Yol 3 seçildi, TODO revize edildi]
```

**Ne Zaman Kullanılır:**
- ✅ Büyük scope değişikliği (7 gün → 5 gün)
- ✅ Stratejik yol seçimi (POC yapalım mı, atlayalım mı?)
- ✅ Felsefe ihlali riski (Stage 0'a 350 satır eklemek?)
- ❌ Küçük task detayları (ÜA karar verir)
- ❌ Implementation bugs (YZ halleder)

**Örnek: Slim POC Kararı (11 Ocak 2026)**
- ÜA: "TODO scope çok büyük, 3 alternatif yol analiz ettim"
- MM: "Yol 3 (Slim POC) stratejik olarak optimal"
- PD: "Yol 3 mimari olarak uyumlu, onaylıyorum"
- **Sonuç:** Consensus %100 → TODO revize edildi (350 satır → 280 satır)

---

## 📊 SLIM POC YAKLAŞIMI

**Tanım:** "Proof of Concept" konseptini minimal scope ile validate etme

**Ne Zaman Kullanılır:**
- Yeni bir konsept test edilecek (B+C Hybrid gibi)
- Risk yüksek (LLVM metadata çalışmaz mı?)
- Zaman kısıtlı (7 gün çok uzun)
- Teknik borç riski (Stage 0 disposable)

### Slim POC Prensipleri

#### 1. **Minimal Scope**
```
Original TODO: 350 satır, 7 gün, Full STO
Slim POC: 280 satır, 5 gün, Type abstraction only

NOT INCLUDED (Stage 1'e defer):
- BigDecimal auto-promotion
- SSO (Small String Optimization)
- Token Borrowing noalias
- Array benchmark
```

**Kural:** Sadece konsept validasyonu için gereken minimum implement edilir.

#### 2. **Explicit Deferred List**
Her Slim POC TODO'sunda "NOT INCLUDED" bölümü zorunlu:
- Ne implement edilMEYECEK?
- Neden erteleniyor?
- Nereye erteleniyor (Stage 1, Stage 2)?

**Benefit:** Scope creep önlenir, mission drift engellenir.

#### 3. **Early Checkpoints**
```
Gün 2 EOD: Critical validation (metadata çalışıyor mu?)
  ↓
  ✅ PASS → Devam
  ❌ FAIL → DURDUR, PD'ye escalate
  
Gün 4: Midpoint review (progress OK?)
  ↓
  ✅ On track → Devam
  ⚠️ Slipping → Scope reduce
  
Gün 5: Decision point (benchmark results)
  ↓
  ≥20% boost → B+C Hybrid Stage 1
  <20% boost → Normal Stage 1
```

**Kural:** Erken failure detection, geç olmadan pivot.

#### 4. **Data-Driven Decision**
```
Hedef: Stage 1 path seçimi
Metrik: Performance boost %
Threshold: ≥20% → Hybrid, <20% → Normal
```

**Kural:** Spekülasyon değil, benchmark data ile karar.

---

## 🎯 CHECKPOINT SİSTEMİ

**Tanım:** Task'lar arası onay noktaları (ÜA approval)

### Task Approval Pattern

**Her task sonrası:**
```
1. YZ task'ı tamamlar
2. YZ rapor yazar (YZ_XX_REPORT.md)
3. YZ LOG'a kayıt eder
4. YZ ÜA onayı bekler
   ↓
5. ÜA raporu okur
6. ÜA test sonuçlarını doğrular
7. ÜA karar verir:
   - ✅ APPROVED → Next task
   - ⚠️ REVISIONS NEEDED → Fix + resubmit
   - ❌ BLOCKED → MM'ye escalate
```

**Success Criteria Check:**
- TODO'daki her task için success criteria var
- ÜA her task sonrası kontrol eder
- Eğer criteria karşılanmadıysa → Revisions

**Örnek:**
```markdown
## Task 1.1 Success Criteria (TODO'dan)
- [ ] poc/ klasörü oluşturuldu
- [ ] sto_hints.h/c compile ediyor
- [ ] Makefile çalışıyor
- [ ] No warnings, no errors

## ÜA Review (YZ_01 Report'tan)
- [x] poc/ oluşturuldu ✅
- [x] Header compile ediyor ✅
- [x] Makefile OK ✅
- [x] No errors ✅

→ APPROVED, Task 1.2'ye geç
```

---

## 🚨 ESCALATION MEKANİZMASI

**Tanım:** Blocker'ları doğru seviyeye yükseltme

### Escalation Chain

```
YZ blocker tespit eder
  ↓
ÜA'ya bildirir (raporda)
  ↓
ÜA kategorize eder:
  - Technical → YZ'ye hint/doc verir
  - Scope unclear → MM'ye escalate
  - Architecture → PD'ye escalate
  - Strategic → MM + PD'ye escalate
```

### Escalation Tipleri

#### Tip 1: Technical Blocker
**Örnek:** "LLVM API nasıl kullanılır bilmiyorum"

**Çözüm:**
- ÜA research yapar
- ÜA documentation verir
- YZ retry eder

**Escalate edilMEZ** (ÜA halleder)

---

#### Tip 2: Scope Blocker
**Örnek:** "TODO 'minimal STO' diyor ama BigDecimal gerekli mi?"

**Çözüm:**
- ÜA → MM'ye escalate
- MM → PD'ye danışır
- PD karar verir: "BigDecimal Stage 1'e defer"
- TODO revize edilir

**Escalate edilİR** (MM + PD kararı)

---

#### Tip 3: Timeline Blocker
**Örnek:** "Gün 3, ama Task 1.3 henüz bitmedi"

**Çözüm:**
- ÜA → MM'ye escalate
- MM karar verir:
  - Option A: +2 gün buffer
  - Option B: Scope reduce (benchmark skip)
  - Option C: Abandon, Normal Stage 1

**Escalate edilİR** (MM stratejik karar)

---

#### Tip 4: Architecture Blocker
**Örnek:** "POC Stage 0'da mı yoksa Stage 1'de mi olmalı?"

**Çözüm:**
- ÜA → MM → PD'ye escalate
- PD mimari karar verir
- Ortak_Gündem.md'de dokümante edilir

**Escalate edilİR** (PD mimari karar)

---

## 📝 LOG & RAPOR SİSTEMİ

### LOG Sistemi (TODO_XXX_LOG.md)

**Amaç:** "Kara kutu" - Kim, ne zaman, ne yaptı?

**Format:**
```markdown
| ID | Tarih/Saat | Rol | Tip | Detay | Sonuç |
|----|------------|-----|-----|-------|-------|
| 001 | 11 Oc 14:30 | MM_01 | DECISION | TODO Slim POC'ye revize | ✅ |
| 002 | 11 Oc 15:00 | YZ_01 | ACTION | Task 1.1 başladı | 🔄 |
| 003 | 11 Oc 18:30 | YZ_01 | SUCCESS | Task 1.1 tamamlandı | ✅ |
```

**Log Tipleri:**
- **DECISION** - Önemli karar
- **ACTION** - İşlem/değişiklik
- **SUCCESS** - Başarılı sonuç
- **ERROR** - Hata tespit
- **TEST** - Test çalıştırma
- **INFO** - Bilgilendirme

**Kural:** Log yazmadan commit YASAK!

---

### Rapor Sistemi (YZ_XX_REPORT.md)

**Amaç:** Her task'ın detaylı dokümantasyonu

**Şablon:** `YZ_REPORT_TEMPLATE.md`

**İçerik:**
```markdown
# YZ_XX TASK REPORT

**Task:** Task X.X - [Task Adı]
**Süre:** [X saat]
**Durum:** [✅/🔄/❌]

## YAPILAN İŞLER
- [Dosyalar, fonksiyonlar, değişiklikler]

## TEST SONUÇLARI
- [Test komutları, çıktılar, PASS/FAIL]

## SUCCESS CRITERIA
- [TODO'dan kopyala, karşılandı mı?]

## BLOCKERLAR
- [Varsa, nasıl çözüldü?]

## NEXT STEPS
- [Sıradaki task]

## ÜA APPROVAL SECTION
- [ ] ✅ APPROVED
- [ ] ⚠️ REVISIONS NEEDED
- [ ] ❌ BLOCKED
```

**Kural:** Her task sonrası rapor zorunlu, ÜA approval gerekli.

---

## 🔗 NEXT_AI_START_HERE.md PATTERN

**Amaç:** Yeni YZ'nin hemen işe başlaması için hazır talimat

**Dosya:** `TODO_XXX/XXX_YZ/NEXT_AI_START_HERE.md`

**İçerik:**
```markdown
# NEXT AI START HERE

**Son Güncelleme:** [Tarih]
**Durum:** [🟢 AKTIF / ⏸️ BEKLEMEDE]
**Sorumlu:** [YZ_XX]

## SENİN GÖREVİN
**Task:** Task X.X - [Task Adı]
**Süre:** [X saat]
**Hedef:** [Ne yapılacak?]

## ADIMLAR
1. Kendini tanıt
2. TODO'yu oku (Task X.X)
3. Kurallari oku (TODO_KURALLARI.md)
4. Task'i uygula
5. Test et (%100 gerçek!)
6. Rapor yaz
7. ÜA onayı bekle

## İLGİLİ DOSYALAR
- [TODO linki]
- [Rehber linkler]
```

**Güncelleme:** ÜA her task sonrası günceller (next task info)

**Benefit:** Context loss minimum, YZ hemen productive.

---

## 🎓 ÖĞRENME & İTERASYON

### Retrospective Pattern

**Her TODO sonrası:**
```markdown
# TODO RETROSPECTIVE

## Ne İyi Gitti? ✅
- [Başarılar]

## Ne Zor Oldu? ⚠️
- [Challenges]

## Öğrenilenler 💡
- [Lessons learned]

## Next TODO İçin 🚀
- [Improvements]
```

**Dosya:** `TODO_XXX_RETROSPECTIVE.md`

**Kullanım:** Bir sonraki TODO planlarken referans.

---

## 📚 REFERANSLAR

**Temel Belgeler:**
- `MELP_PHILOSOPHY.md` - 6 İlke, teknik felsefe
- `STO_PRINCIPLES.md` - STO prensibleri
- `TODO_KURALLARI.md` - Genel TODO kuralları
- **Bu belge** - Proje yönetimi metodolojisi

**Örnek TODO'lar:**
- `TODO_Stage 0 - Slim POC` - Consensus + Checkpoint örneği
- `Ortak_Gündem.md` - 3 rol consensus örneği

**Şablonlar:**
- `YZ_REPORT_TEMPLATE.md` - Rapor şablonu
- `NEXT_AI_START_HERE.md` - Görev talimatı şablonu

---

## 🎯 ÖZET

**MELP'in Benzersiz Metodolojisi:**

1. **4 Katmanlı Hiyerarşi** - PD/MM/ÜA/YZ autonomous roller
2. **Consensus-Driven** - Büyük kararlar oybirliğiyle (Ortak_Gündem)
3. **Slim POC Yaklaşımı** - Minimal scope, erken validation
4. **Checkpoint Sistemi** - Her task ÜA approval gerekir
5. **Escalation Mekanizması** - Blocker'lar doğru seviyeye yükselir
6. **LOG & Rapor** - Her şey dokümante, şeffaf
7. **NEXT_AI_START_HERE** - Context loss önlenir

**Sonuç:** AI ekip, insan müdahalesi minimumla, autonomous ve şeffaf çalışır.

---

**Versiyon:** 1.0  
**Son Güncelleme:** 12 Ocak 2026  
**Hazırlayan:** MM_01 (Mastermind)  
**Onay:** Consensus (ÜA + MM + PD pattern'den türetildi)
