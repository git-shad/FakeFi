# FakeFi

[![ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

**FakeFi** is an ESP32 project that creates a **fake WiFi Access Point (AP)**.  
It sets up a custom SSID, runs a DNS server, and reports connected clients with their **MAC addresses** and **signal strength (RSSI)**.  

---

## ✨ Features

- ✅ Create a custom WiFi Access Point (AP).  
- ✅ Wildcard DNS server (redirect all domains to AP IP).  
- ✅ Logs client **MAC addresses** and **RSSI** in real time.  
- ✅ Detects when devices **join** or **leave** the AP.  

---

## 📦 Requirements

- ESP32 board  
- Arduino IDE or PlatformIO  
- Libraries:
  - `WiFi.h`
  - `DNSServer.h`
  - `esp_wifi.h` (built-in for ESP32)  

---

## ⚙️ Setup

1. **Clone this repository:**

   ```bash
   git clone https://github.com/git-shad/FakeFi.git
   cd FakeFi
   
2. **Open the project** in Arduino IDE or PlatformIO.

3. **Configure your WiFi credentials** in the code:

   ```cpp
   const char* apSsid = "YourWiFiSSID"; 
   const char* apPass = "YourWiFiPassword"; 
   ```

   > Leave `apPass` empty (`""`) for an **open network**.

4. **Upload to your ESP32 board.**

5. Open **Serial Monitor** (baud `115200`) to see logs of connected devices.

---

## 🖥️ Example Output

```text
[INFO] SoftAP started. IP: 192.168.255.255
[INFO] DNS server started (wildcard -> AP IP)
[INFO] Connected stations: 1
  - MAC: 3C:71:BF:AA:12:34, RSSI: -45 dBm
[EVENT] A client joined. Count: 0 -> 1
```

---

## 📂 Source

```cpp
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
```

---

## ⚠️ Disclaimer

This project is for **educational purposes only**.
Do **not** use it for malicious activity, impersonating real networks, or violating laws.
You are solely responsible for how you use this project.

---

## 📜 License

MIT License © 2025 [Your Name]

