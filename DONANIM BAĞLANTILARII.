## 🔌 Donanım Bağlantıları (Custom Wiring)

Bu projede bileşenler, ortak güç hattı (Breadboard Rails) üzerinden beslenmiş ve pompa **"Ground Switching" (Eksi Anahtarlama)** yöntemi ile bağlanmıştır.

### 1. Su Pompası Bağlantısı (Low-Side Switching)
Pompanın artısı sürekli güce bağlıdır, eksi ucu röle üzerinden anahtarlanır.
* **Pompa Kırmızı (+)** 👉 `ESP32 VIN (5V)` 
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
* **ESP32 VN (5V)** 👉 `Breadboard (+) Hattı`
* **ESP32 GND** 👉 `Breadboard (-) Hattı`
* **ESP32 3V3** 👉 `Breadboard (+) Hattı`
