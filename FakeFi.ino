#include <WiFi.h>
#include <DNSServer.h>
extern "C" {
  #include "esp_wifi.h"
}

const char* apSsid = ""; //your wifi username
const char* apPass = ""; //your wifi password

// Manual AP IP settings
IPAddress apIP(192,168,255,255);
IPAddress apGateway(192,168,255,255);
IPAddress apSubnet(255,255,255,255);

DNSServer dnsServer;
const byte DNS_PORT = 53;

unsigned long lastClientReport = 0;
const unsigned long CLIENT_REPORT_INTERVAL = 5000; // ms
int lastClientCount = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_AP);

  if (!WiFi.softAPConfig(apIP, apGateway, apSubnet)) {
    Serial.println("[WARN] softAPConfig failed");
  }

  if (strlen(apPass) == 0) {
    WiFi.softAP(apSsid);
  } else {
    WiFi.softAP(apSsid, apPass);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("[INFO] SoftAP started. IP: ");
  Serial.println(myIP);

  dnsServer.start(DNS_PORT, "*", myIP);
  Serial.println("[INFO] DNS server started (wildcard -> AP IP)");
}

void loop() {
  dnsServer.processNextRequest();

  unsigned long now = millis();
  if (now - lastClientReport >= CLIENT_REPORT_INTERVAL) {
    reportConnectedClients();
    lastClientReport = now;
  }
}

String macToString(const uint8_t mac[6]) {
  char buf[18];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void reportConnectedClients() {
  wifi_sta_list_t wifi_sta_list;
  if (esp_wifi_ap_get_sta_list(&wifi_sta_list) == ESP_OK) {
    int count = wifi_sta_list.num;
    Serial.printf("[INFO] Connected stations: %d\n", count);

    for (int i = 0; i < count; i++) {
      wifi_sta_info_t sta = wifi_sta_list.sta[i];
      String mac = macToString(sta.mac);
      int rssi = sta.rssi; // signal strength in dBm
      Serial.printf("  - MAC: %s, RSSI: %d dBm\n", mac.c_str(), rssi);
    }

    if (count != lastClientCount) {
      if (count > lastClientCount) {
        Serial.printf("[EVENT] A client joined. Count: %d -> %d\n", lastClientCount, count);
      } else {
        Serial.printf("[EVENT] A client left. Count: %d -> %d\n", lastClientCount, count);
      }
      lastClientCount = count;
    }
  } else {
    Serial.println("[ERROR] Failed to get station list");
  }
}
