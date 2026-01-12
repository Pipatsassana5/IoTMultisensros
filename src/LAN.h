
#include <WiFi.h>
#include <EthernetESP32.h> 

#define MySerial Serial
#define LED_PIN 2  // กำหนดขา LED ที่จะควบคุม

// Ethernet Setup
byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x32, 0x33 };
IPAddress ip(192, 168, 2, 48);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

// Driver for W5500: (CS_PIN, RESET_PIN)
// ใช้การตั้งค่าเดิมของคุณที่ Ping ผ่าน (CS=5, RST=-1)
W5500Driver eth_driver(5, -1); 



// --- Setup ---
void LANSETUP(){
  WiFi.mode(WIFI_OFF);
  
  Ethernet.init(eth_driver); 
  
  MySerial.println("Connecting Ethernet...");
  Ethernet.begin(mac, ip, gateway, subnet);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    MySerial.println("W5500 not found. Check wiring!");
    while (true) delay(1); 
  }
  
  MySerial.println(Ethernet.localIP());
}
