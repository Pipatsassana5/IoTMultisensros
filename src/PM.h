#include <PMS.h>
#include <SoftwareSerial.h>

#define PMS_SET 14
#define PMS_RESET 12

EspSoftwareSerial::UART testSerial ;
PMS pms(testSerial);
PMS::DATA data;

uint16_t pm25_cf1;
uint16_t pm25_atm;

void pmSetup() {
  pinMode(PMS_RESET, OUTPUT);
  digitalWrite(PMS_RESET, HIGH); // เปิดเซนเซอร์ PMS
  
  digitalWrite(PMS_SET, HIGH); // เปิดเซนเซอร์ PMS
  delay(2000); // รอให้เซนเซอร์พร้อมทำงาน (2วินาที)
  
}

void pmLoop() {
  pms.requestRead();
  if (pms.readUntil(data, PMS::SINGLE_RESPONSE_TIME)) {
    pm25_cf1 = data.PM_SP_UG_2_5;
    pm25_atm = data.PM_AE_UG_2_5;
    Serial.print("PM2.5 (CF=1): ");
    Serial.print(pm25_cf1);
    Serial.print(" ug/m3");
   

    Serial.print(" ug/m3, PM2.5 (Atmospheric): ");
    Serial.print(pm25_atm);
    Serial.print(" ug/m3");
    
  } else {
    Serial.println("No data received from PMS sensor");
  }
}