#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMac[] = {0xA4, 0xF0, 0x0F, 0x5C, 0x11, 0x78};

typedef struct struct_message {
  bool led;
} struct_message;

struct_message data;

void onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  delay(100);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  data.led = true;
}

void loop() {
  esp_now_send(receiverMac, (uint8_t *)&data, sizeof(data));
  delay(1000);
  data.led = !data.led;
}
