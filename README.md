# 🌱 IoT Tabanlı Akıllı Bitki Sulama ve Takip Sistemi
 
platf![GİTHUB ](https://github.com/user-attachments/assets/3529a2e0-c871-4f34-900e-749ef5354e5c)

Bu proje, **ESP32** mikrodenetleyicisi ve **Blynk IoT** 
ormu kullanılarak geliştirilmiş, hibrit (Otomatik/Manuel) çalışan bir akıllı tarım prototipidir. Sistem, toprak nemini sürekli analiz ederek bitkinin su ihtiyacını karşılar. Veriler hem **Blynk Mobil Uygulaması** hem de **Blynk Web Console** üzerinden anlık olarak takip edilebilir.

## 🚀 Projenin Amacı
Geleneksel sulama yöntemlerindeki insan hatasını (unutma veya aşırı sulama) ortadan kaldırmak ve su tüketim verisini dijitalleştirerek uzaktan takibi ve kontrolü sağlamaktır.

## 🛠 Donanım Bileşenleri
Bu projede kullanılan malzemeler şunlardır:
* **Mikrodenetleyici:** ESP32 DevKit V1
* **Sensör:** Kapasitif Toprak Nem Sensörü (Analog)
* **Eyleyici:** 5V Röle Modülü
* **Su Pompası:** 5V Mini Dalgıç Su Pompası
* **Güç:** Micro USB Kablosu (Laptop veya Priz Adaptörü ile beslenir)
* **Bağlantı:** Breadboard (Devre Tahtası) ve Jumper Kablolar

## ⚙️ Özellikler ve Algoritma

### 1. Hibrit Çalışma Modu (V4 Switch)
Sistem iki farklı modda çalışacak şekilde programlanmıştır:
* **🤖 Tam Otomatik (Robot) Mod:** Nem oranı kritik seviyenin (%60) altına düştüğünde sistem otomatik olarak devreye girer, sessizce sulama yapar ve suyu kapatır. Kullanıcıyı bildirimle rahatsız etmez.
* **👤 Manuel (İnsan) Mod:** Otomatik sulama devre dışıdır. Toprak kuruduğunda kullanıcının telefonuna **"Toprak Kurudu!"** bildirimi gönderilir. Sulama kararı kullanıcıya bırakılır.

### 2. Su Tüketim Takibi (Flow Calculation)
Sistemde fiziksel bir akış sensörü yerine **Zaman Bazlı Algoritma** kullanılmıştır.
* *Formül:* `Harcanan Su = Pompa Çalışma Süresi (sn) x Debi (L/sn)`
* Veriler ESP32 yeniden başlatılsa bile sunucu üzerinde korunur (Persistent Memory).

### 3. Güvenlik ve Kararlılık
* **Spam Koruması:** Manuel modda bildirimler "Flag" yapısı ile sınırlandırılmıştır; kullanıcı müdahale edene kadar tekrar bildirim gitmez.
* **Cool-Down Süresi:** Otomatik modda olası sensör hatalarına karşı, iki sulama arasında en az **30 saniye** güvenlik beklemesi tanımlanmıştır.

## 📱 Blynk Arayüzü (Mobil & Web Console)
Sistem verileri **Blynk Console** (Web) ve Mobil Uygulama üzerinden şu pinlerle yönetilir:

* **V0:** Anlık Nem Grafiği (Canlı Veri)
* **V1:** Manuel Sulama Butonu (Bas-Çek)
* **V2:** Durum Göstergesi (Nem Düşükse: 🥀 Üzgün Çiçek / Normalse: 🌸 Mutlu Çiçek)
* **V3:** Toplam Harcanan Su Miktarı (Litre)
* **V4:** Otomatik Mod Anahtarı (Robot Modu Aç/Kapa)
--------------------------------------------------------------------------------------------
## 🔌 Donanım Bağlantı Şeması (Özet)
* **Güç:** Sistem gücünü ESP32'nin USB girişinden alır. `VIN` (5V) pini üzerinden Breadboard aracılığıyla Röle ve Pompaya güç dağıtılır.
* **Pompa:** Eksi (-) tetikleme mantığıyla Röle üzerinden anahtarlanır.
* **Sensör:** 3.3V ile beslenir, analog veriyi `GPIO 34` pininden okur.

## 🔧 Kurulum
1.  Repo'daki `.ino` dosyasını indirin.
2.  `BLYNK_TEMPLATE_ID` ve `AUTH_TOKEN` bilgilerinizi kendi projenize göre güncelleyin.
3.  Arduino IDE üzerinden ESP32 kartınıza yükleyin.

---

## 🔌 Donanım Bağlantıları (Custom Wiring)
<img width="1331" height="700" alt="Ekran görüntüsü 2026-01-13 134159" src="https://github.com/user-attachments/assets/984211f9-f83b-4460-b281-63fbf101be14" />
Bu projede bileşenler, ortak güç hattı (Breadboard Rails) üzerinden beslenmiş ve pompa **"Ground Switching" (Eksi Anahtarlama)** yöntemi ile bağlanmıştır.

### 1. Su Pompası Bağlantısı (Low-Side Switching)
Pompanın artısı sürekli güce bağlıdır, eksi ucu röle üzerinden anahtarlanır.
* **Pompa Kırmızı (+)** 👉 `ESP32 VN (5V)` 
* **Pompa Siyah (-)** 👉 `Röle NO (Normalde Açık)`

* **Röle COM (Ortak)** 👉 `ESP32 GND` (Devreyi tamamlayan uç)

### 2. Röle Modülü (Kontrol)
* **VCC** 👉 `Breadboard (+) Hattı`
* **GND** 👉 `ESP32 GND`
* **IN** 👉 `ESP32 Pin 27` (GPIO 27)

### 3. Toprak Nem Sensörü
* **VCC** 👉 `Breadboard (+) Hattı`
* **GND** 👉 `Breadboard (-) Hattı`
* **AOUT** 👉 `ESP32 Pin 34` (GPIO 34)

### 4. Güç Dağıtımı (Power Distribution)
* **ESP32 VIN (5V)** 👉 `Breadboard (+) Hattı`
* **ESP32 GND** 👉 `Breadboard (-) Hattı`
* **ESP32 3V3** 👉 `Breadboard (+) Hattı` 

---
**Geliştirici:** [Batuhan BOSTAN & Büşranur KARAATEŞ] - Yönetim Bilişim Sistemleri

