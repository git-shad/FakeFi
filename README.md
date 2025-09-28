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
   git clone https://github.com/your-username/FakeFi.git
   cd FakeFi
