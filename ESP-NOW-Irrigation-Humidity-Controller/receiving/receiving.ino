#include <esp_now.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define RELAY_PIN 5
#define MOISTURE_THRESHOLD 500

uint8_t secondDeviceAddress[] = {0x66, 0x55, 0x44, 0x33, 0x22, 0x11};

esp_now_peer_info_t peerInfo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  int soilMoisture = *((int*)data);

  lcd.clear();
  if (soilMoisture < MOISTURE_THRESHOLD) {
    lcd.print("Toprak kurumuş, sulama başlatılıyor...");
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
  } else {
    lcd.print("Toprak nem seviyesi uygun, sulama gerekmiyor.");
  }
}


void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW başlatma hatası");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, secondDeviceAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Eşleme hatası");
    return;
  }
}

void loop() {

}
