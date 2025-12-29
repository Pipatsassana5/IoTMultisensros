#include <HTTPClient.h>

// เปลี่ยนเป็น URL ของไฟล์ PHP บน Server จริงของคุณ
const char* serverName = "http://ath108.com/iot/insert.php"; 

void sendDataToServer(float t, float h, int pm, int co2, float wind) {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    // เริ่มเชื่อมต่อ
    http.begin(serverName);
    
    // กำหนด Header ว่าเราจะส่งข้อมูลแบบ Form
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // จัดเตรียมข้อมูลที่จะส่ง (Key ต้องตรงกับ $_REQUEST ใน PHP)
    String httpRequestData = "temp=" + String(t)
                           + "&hum=" + String(h)
                           + "&pm25=" + String(pm)
                           + "&co2=" + String(co2)
                           + "&wind=" + String(wind);
                           
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