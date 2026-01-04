#include <HTTPClient.h>

// เปลี่ยนเป็น URL ของไฟล์ PHP บน Server จริงของคุณ
const char* serverName = "http://ath108.com/iot/insert.php"; 

void sendDataToServer(float temperature, float humidity, uint16_t pm25_cf1, uint16_t pm25_atm, int CO2, float windSpeed) {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    // เริ่มเชื่อมต่อ
    http.begin(serverName);
    
    // กำหนด Header ว่าเราจะส่งข้อมูลแบบ Form
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // จัดเตรียมข้อมูลที่จะส่ง (Key ต้องตรงกับ $_REQUEST ใน PHP)
    String httpRequestData = "temp=" + String(temperature)
                           + "&hum=" + String(humidity)
                           + "&pm25_cf1=" + String(pm25_cf1)
                           + "&pm25_atm=" + String(pm25_atm)
                           + "&co2=" + String(CO2)
                           + "&wind=" + String(windSpeed);
                           
    // ส่งคำสั่ง POST
    int httpResponseCode = http.POST(httpRequestData);
    
    // ตรวจสอบผลลัพธ์
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("✅ Server Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println("   Message: " + response);
    }
    else {
      Serial.print("❌ Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    
    // จบการทำงาน
    http.end();
  }
  else {
    Serial.println("⚠️ WiFi Disconnected");
  }
}