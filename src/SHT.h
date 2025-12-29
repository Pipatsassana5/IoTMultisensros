#include <Wire.h>             
#include <Adafruit_SHT31.h>

float t,h;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void SHT_setup() {
Serial.println("SHT31 Test");
  if (!sht31.begin(0x44)) {   
    Serial.println("Couldn't find SHT31");
    while (1) delay(1); // หยุดทำงานถ้าหาเซนเซอร์ไม่เจอ
  }
  Serial.println("SHT31 Found!");
}

void SHT_loop() {
  t = sht31.readTemperature();
  h = sht31.readHumidity();

  if (!isnan(t)) { 
    Serial.print("Temp: "); Serial.print(t); Serial.print(" C, ");
    Serial.print("Hum: "); Serial.print(h); Serial.println(" %");
  } else {
    Serial.println("Failed to read from SHT31 sensor!");
  }
}