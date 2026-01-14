#include "MHZ19.h"

#define RX_PIN 26  // ต่อกับขา TX ของ MH-Z19
#define TX_PIN 27  // ต่อกับขา RX ของ MH-Z19

MHZ19 myMHZ19;
HardwareSerial mySerial(1); // ใช้ Hardware Serial 1
int CO2 = 0;

void CO2_setup() {

  // เริ่มต้น Serial 1 ที่ขา 26, 27
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  // เริ่มต้นเซนเซอร์
  myMHZ19.begin(mySerial);
  
  // ปิดระบบ Auto Calibration (แนะนำให้ปิด ถ้าไม่ได้วาง outdoor ตลอดเวลา)
  myMHZ19.autoCalibration(false);

  Serial.println("MH-Z19 Warming up... (waiting 3 mins is recommended)");
}

void Co2_loop() {
   CO2 = myMHZ19.getCO2();

  Serial.print("CO2: ");
  Serial.print(CO2);
  Serial.println(" ppm");
  
  // เช็ค Error (ปกติค่าจะอยู่ที่ 400-5000)
  if(CO2 == 0) {
      Serial.println("⚠️ Error: Check Wiring (RX/TX crossed?)");
  }

}

