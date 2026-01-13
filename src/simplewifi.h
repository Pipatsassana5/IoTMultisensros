#ifndef SIMPLEWIFI_H
#define SIMPLEWIFI_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

// --- กำหนดค่า ---
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer server(80);
Preferences preferences;

// ตัวแปรเก็บค่าชั่วคราว
String ssid = "";
String pass = "";
String ip = "";
String gateway = "";
String subnet = "";

// --- HTML หน้าตั้งค่า (แบบ Minimal) ---
const char* html_page = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<title>WiFi Setup</title>
<style>body{font-family:sans-serif;padding:20px;} input{width:100%;padding:10px;margin:5px 0;} button{width:100%;padding:10px;background:#4CAF50;color:white;border:none;}</style>
</head><body>
<h2>Manual WiFi Config</h2>
<form action="/save" method="POST">
 <label>SSID (WiFi Name):</label><br><input type="text" name="ssid" placeholder="Enter WiFi Name"><br>
 <label>Password:</label><br><input type="password" name="pass" placeholder="Enter Password"><br>
 <hr>
 <label>Static IP (Optional):</label><br><input type="text" name="ip" placeholder="192.168.1.200"><br>
 <label>Gateway:</label><br><input type="text" name="gw" placeholder="192.168.1.1"><br>
 <label>Subnet:</label><br><input type="text" name="sn" placeholder="255.255.255.0"><br>
 <br><button type="submit">Save & Restart</button>
</form>
</body></html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html_page);
}

void handleSave() {
  String s_ssid = server.arg("ssid");
  String s_pass = server.arg("pass");
  String s_ip = server.arg("ip");
  String s_gw = server.arg("gw");
  String s_sn = server.arg("sn");

  if (s_ssid != "") {
    preferences.begin("wifi-config", false);
    preferences.putString("ssid", s_ssid);
    preferences.putString("pass", s_pass);
    preferences.putString("ip", s_ip);
    preferences.putString("gw", s_gw);
    preferences.putString("sn", s_sn);
    preferences.end();
    
    String msg = "<h1>Saved!</h1><p>Device is restarting...</p>";
    server.send(200, "text/html", msg);
    delay(2000);
    ESP.restart();
  } else {
    server.send(200, "text/html", "Error: SSID is required. <a href='/'>Go Back</a>");
  }
}

void setupWiFi() {
  // 1. อ่านค่าที่บันทึกไว้
  preferences.begin("wifi-config", true);
  ssid = preferences.getString("ssid", "");
  pass = preferences.getString("pass", "");
  ip = preferences.getString("ip", "");
  gateway = preferences.getString("gw", "");
  subnet = preferences.getString("sn", "");
  preferences.end();

  Serial.println("Connecting to: " + ssid);

  // ตั้งค่า Static IP ถ้ามี
  if (ip != "" && gateway != "") {
    IPAddress _ip, _gw, _sn;
    if (_ip.fromString(ip) && _gw.fromString(gateway)) {
        if (!_sn.fromString(subnet)) _sn.fromString("255.255.255.0"); // Default subnet
        WiFi.config(_ip, _gw, _sn);
    }
  }

  // พยายามเชื่อมต่อ WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  unsigned long startAttempt = millis();
  // รอเชื่อมต่อ 10 วินาที ถ้าไม่ได้ ให้เข้าโหมดตั้งค่า (AP)
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());
  } else {
    // --- เข้าสู่โหมด AP เพื่อตั้งค่า ---
    Serial.println("\nConnection Failed. Starting AP Mode...");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    
    // ตั้งชื่อ WiFi สำหรับตั้งค่า
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("ESP32-Setup", "12345678"); // ชื่อ WiFi: ESP32-Setup, รหัส: 12345678

    // เริ่ม DNS Server (เพื่อให้เด้งเข้าหน้าเว็บอัตโนมัติ)
    dnsServer.start(DNS_PORT, "*", apIP);

    // ตั้งค่า Web Server
    server.on("/", handleRoot);
    server.on("/save", handleSave);
    server.onNotFound(handleRoot); // ใครพิมพ์ผิด ให้เด้งไปหน้าแรก
    server.begin();

    Serial.println("AP Mode Started. Please Connect to 'ESP32-Setup'");
    
    // Loop รอจนกว่าผู้ใช้จะตั้งค่าเสร็จ (ค้างหน้านี้เลย ไม่ไป main loop)
    while (true) {
      dnsServer.processNextRequest();
      server.handleClient();
      delay(10); 
    }
  }
}

#endif