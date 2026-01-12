/* BLYNK AYARLARI */
#define BLYNK_TEMPLATE_ID "BURAYA_KENDI_ID_YAZIN"
#define BLYNK_TEMPLATE_NAME "BURAYA_KENDI_PROJENİZİN_İSMİNİ_YAZINIZ"
#define BLYNK_AUTH_TOKEN "BURAYA_KENDI_TOKENINIZI_YAZIN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// --- AĞ AYARLARI ---
char ssid[] = "BURAYA_KENDI_KİŞİSEL_BAĞLANTI_NOKTANIZI_YAZIN";        
char pass[] = "BURAYA_KENDI_ŞİFRENİZİ_YAZIN";   

// --- DONANIM PINLERI ---
#define SENSOR_PIN 34  // Nem Sensörü
#define POMPA_PIN 27   // Röle Tetikleyici

// --- RÖLE AYARI ---
const bool ROLE_DUZ_MANTIK = false; // Low Trigger ise false

BlynkTimer timer;

// DEĞİŞKENLER
float harcananSu = 0.0;
int sensorDegeri = 0;
int nemYuzdesi = 0;
bool otomatikMod = false; 
bool bildirimAtildi = false; // SPAM KORUMASI İÇİN EKLENDİ

// --- KALIBRASYON ---
float saniyelikDebi = 0.02; // LİTRE/SANİYE

// --- OTOMATİK SULAMA AYARLARI ---
const int KuruDeger = 4095;    
const int IslakDeger = 1400;   
const int UyariSiniri = 60;    
const int OtoSulamaSuresi = 3; 

unsigned long sonSulamaZamani = 0; 

// Röle Kontrol Fonksiyonu
void pompaKontrol(bool acikMi) {
  if (ROLE_DUZ_MANTIK) {
    digitalWrite(POMPA_PIN, acikMi ? HIGH : LOW);
  } else {
    digitalWrite(POMPA_PIN, acikMi ? LOW : HIGH); 
  }
}

// ---------------------------------------------------------
// HAFIZA (V3)
// ---------------------------------------------------------
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V3); // Su verisini çek
  Blynk.syncVirtual(V4); // Otomatik mod durumunu çek
}

BLYNK_WRITE(V3) {
  harcananSu = param.asFloat(); 
  Blynk.virtualWrite(V3, harcananSu); 
}

// ---------------------------------------------------------
// OTOMATİK MOD ANAHTARI (V4)
// ---------------------------------------------------------
BLYNK_WRITE(V4) {
  int dur = param.asInt();
  if (dur == 1) {
    otomatikMod = true;
    Serial.println("MOD: OTOMATIK (Sessiz Calisma)");
  } else {
    otomatikMod = false;
    Serial.println("MOD: MANUEL (Bildirim Acik)");
  }
}

// ---------------------------------------------------------
// MANUEL SULAMA (V1)
// ---------------------------------------------------------
unsigned long manuelBaslangic = 0;

BLYNK_WRITE(V1) {
  int butonDurumu = param.asInt(); 
  
  if (butonDurumu == 1) {
    manuelBaslangic = millis(); 
    pompaKontrol(true); 
    Serial.println("Manuel: Pompa AÇILDI.");
  } else {
    pompaKontrol(false); 
    
    unsigned long sureMs = millis() - manuelBaslangic;
    float sureSn = sureMs / 1000.0;
    float eklenenSu = sureSn * saniyelikDebi;
    
    harcananSu += eklenenSu;
    Blynk.virtualWrite(V3, harcananSu);
    
    Serial.print("Manuel Sulama Bitti. Eklenen: ");
    Serial.println(eklenenSu);
  }
}

// ---------------------------------------------------------
// ANA KURULUM
// ---------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(POMPA_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pompaKontrol(false); 

  Serial.println("\n--- SISTEM BASLATILIYOR ---");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Baglandi!");
  Blynk.config(BLYNK_AUTH_TOKEN);
  
  timer.setInterval(1000L, sensorIzleme); 
}

void loop() {
  Blynk.run();
  timer.run();
}

// ---------------------------------------------------------
// AKILLI SENSÖR VE KARAR MEKANİZMASI (DÜZENLENDİ)
// ---------------------------------------------------------
void sensorIzleme() {
  sensorDegeri = analogRead(SENSOR_PIN);
  nemYuzdesi = map(sensorDegeri, KuruDeger, IslakDeger, 0, 100);
  nemYuzdesi = constrain(nemYuzdesi, 0, 100);

  Blynk.virtualWrite(V0, nemYuzdesi); 

  // --- SENARYO 1: OTOMATİK MOD (SESSİZ SULAMA) ---
  if (nemYuzdesi < UyariSiniri && otomatikMod == true && (millis() - sonSulamaZamani > 30000)) {
    
    Serial.println("OTO MOD: Toprak kuru, sulama yapiliyor...");
    
    // BURADAN BLYNK.LOGEVENT'İ SİLDİM (Sessiz Mod)
    
    pompaKontrol(true);
    delay(OtoSulamaSuresi * 1000); 
    pompaKontrol(false);
    
    float otoHarcanan = OtoSulamaSuresi * saniyelikDebi;
    harcananSu += otoHarcanan;
    Blynk.virtualWrite(V3, harcananSu); 
    
    sonSulamaZamani = millis();
    Serial.println("OTO MOD: Sulama bitti.");
    
  } 
  
  // --- SENARYO 2: MANUEL MOD (BİLDİRİMLİ) ---
  else if (nemYuzdesi < UyariSiniri && otomatikMod == false) {
    
    Blynk.virtualWrite(V2, 1); // Üzgün Çiçek
    
    // SADECE MANUEL MODDA VE DAHA ÖNCE ATILMADIYSA BİLDİRİM AT
    if (bildirimAtildi == false) {
       Blynk.logEvent("su_uyarisi", "Hocam Manuel Moddayiz ve Su Bitti! Yetis!");
       bildirimAtildi = true; // Bayrağı kaldır ki spam yapmasın
       Serial.println("UYARI: Bildirim gonderildi!");
    }
    
  } 
  
  // --- SENARYO 3: NEM NORMAL (RESET) ---
  else if (nemYuzdesi >= UyariSiniri) {
    Blynk.virtualWrite(V2, 2); // Mutlu Çiçek
    bildirimAtildi = false; // Toprak ıslandı, bayrağı indir. Kuruyunca tekrar atabilir.
  }
}
