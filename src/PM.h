/* #include <PMS.h>


HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA data;

uint16_t pm25_cf1;
uint16_t pm25_atm;

void pmSetup() {
 
  pmsSerial.begin(9600, SERIAL_8N1, 16, 17);
  pms.wakeUp();
  delay(3000); 
  pms.activeMode();
}

void pmLoop() {
  if (pms.read(data)) {
    Serial.println("---------------------------------------");
    Serial.print("PM 1.0 (CF=1): "); Serial.print(data.PM_SP_UG_1_0); Serial.println(" ug/m3");
    Serial.print("PM 2.5 (CF=1): "); Serial.print(data.PM_SP_UG_2_5); Serial.println(" ug/m3");
    Serial.print("PM 10.0 (CF=1): "); Serial.print(data.PM_SP_UG_10_0); Serial.println(" ug/m3");
    
    Serial.print("PM 1.0 (Atm): "); Serial.print(data.PM_AE_UG_1_0); Serial.println(" ug/m3");
    Serial.print("PM 2.5 (Atm): "); Serial.print(data.PM_AE_UG_2_5); Serial.println(" ug/m3");
    Serial.print("PM 10.0 (Atm): "); Serial.print(data.PM_AE_UG_10_0); Serial.println(" ug/m3"); 
    Serial.println("---------------------------------------");
  } else {
    Serial.println("No data received from PMS sensor");
  }
} */

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
  
  testSerial.begin(9600,  EspSoftwareSerial::SWSERIAL_8N1, 16, 17,false, 95, 11);
  pms.wakeUp();
  delay(3000); 
  pms.activeMode();
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