#include <WiFi.h>
#include <esp_now.h>

/* ===== DATA STRUCTURE (SAME FOR ALL) ===== */
typedef struct {
  uint8_t device_id;
  float temperature;
  uint32_t counter;
} espnow_data_t;

espnow_data_t rxData;

/* ===== RECEIVE CALLBACK (FIXED) ===== */
void onDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData,
                int len) {

  memcpy(&rxData, incomingData, sizeof(rxData));

  Serial.println(" Data Received");
  Serial.print("From MAC: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                info->src_addr[0], info->src_addr[1], info->src_addr[2],
                info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  Serial.print("Device ID: ");
  Serial.println(rxData.device_id);

  Serial.print("Temperature: ");
  Serial.println(rxData.temperature);

  Serial.print("Counter: ");
  Serial.println(rxData.counter);

  Serial.println("--------------------");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println(" ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  Serial.println(" ESP-NOW Receiver Ready");
}

void loop() {
  // nothing needed
}
