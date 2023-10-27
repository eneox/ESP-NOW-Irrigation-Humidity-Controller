#include <esp_now.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define SOIL_MOISTURE_PIN A0


uint8_t firstDeviceAddress[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

esp_now_peer_info_t peerInfo;

void sendData(int soilMoisture) {
  esp_err_t result = esp_now_send(firstDeviceAddress, (uint8_t*)&soilMoisture, sizeof(soilMoisture));
  if (result == ESP_OK) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Error sending data");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb([](const uint8_t *mac, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  });

  memcpy(peerInfo.peer_addr, firstDeviceAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  sendData(soilMoisture);
  delay(5000); 
}
