#include <ESP8266WiFi.h>
#include <espnow.h>

/* ===== RECEIVER MAC ===== */
uint8_t receiverMAC[] = {0xA4, 0xF0, 0x0F, 0x5C, 0x11, 0x78}; // ESP32 MAC

typedef struct {
  uint8_t device_id;
  float temperature;
  uint32_t counter;
} espnow_data_t;

espnow_data_t txData;
uint32_t count = 0;

void onDataSent(uint8_t *mac, uint8_t status) {
  Serial.println(status == 0 ? " Send OK" : " Send Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.print("ESP8266 MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_register_send_cb(onDataSent);

  txData.device_id = 3;  // change per sender
}

void loop() {
  txData.temperature = random(180, 320) / 10.0;
  txData.counter = count++;

  esp_now_send(receiverMAC, (uint8_t *)&txData, sizeof(txData));
  delay(2000);
}