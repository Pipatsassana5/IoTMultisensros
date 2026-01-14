
#include "SHT.h"
#include "LCD.h"
#include "PM.h"
#include "CO2.h"
#include "simplewifi.h"
#include "sendata.h"


void setup() {

 
  delay(2000); 

  // 2. กำหนดขา RESET ของจอเป็น Output
  pinMode(TFT_RST, OUTPUT);
  
  // 3. สั่ง Reset จอแบบ Manual (ตบให้ตื่น)
  digitalWrite(TFT_RST, HIGH);
  delay(50);
  digitalWrite(TFT_RST, LOW);  // ดึงขา Reset ลงกราวด์ (สั่งรีเซ็ต)
  delay(100);                  // ค้างไว้แป๊บนึง
  digitalWrite(TFT_RST, HIGH); // ปล่อยขา Reset (เริ่มทำงาน)
  delay(100);                  // รอให้ชิปจอพร้อมรับคำสั่ง
  
  Serial.begin(115200); 
  
  Wire.begin(21,22);
  Wire.setClock(100000); 

  setupWiFi();
  SHT_setup();
  LCD_setup();
  CO2_setup();
  
 // delay(5000);
  pmSetup();
}

void loop() {



  SHT_loop();
    pmLoop();
    Co2_loop();
   Loop_WindSensor();
    LCD_showIP(WiFi.localIP());
    LCD_loop(t, h, pm25_cf1, pm25_atm, CO2, windSpeed);

    
      if(WiFi.status() == WL_CONNECTED){
      sendDataToServer(t, h, pm25_cf1, pm25_atm, CO2, windSpeed);
    }else{
      Serial.println("⚠️ WiFi Disconnected");
    }
    
  delay(500); 
}