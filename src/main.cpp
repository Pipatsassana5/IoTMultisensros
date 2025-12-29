#include <Arduino.h>
#include "SHT.h"
#include "LCD.h"
#include "PM.h"
#include "CO2.h"


void setup() {
  delay(100); 

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
  testSerial.begin(9600,  EspSoftwareSerial::SWSERIAL_8N1, 16, 17,false, 95, 11);
  while (!Serial) delay(10); // รอให้ Serial Monitor พร้อม (สำคัญในบางบอร์ด)
  SHT_setup();
  LCD_setup();
  CO2_setup();
  pmSetup();
  
  
}

void loop() {
    SHT_loop();
    pmLoop();
    Co2_loop();
    LCD_loop(t, h, pm25_cf1, pm25_atm, CO2);

  delay(2000);
}