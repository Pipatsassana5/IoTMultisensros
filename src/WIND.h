#include <Wire.h>   
#include <Arduino.h>

#define FS3000_ADDR 0x28  // ที่อยู่ I2C มาตรฐานของ FS3000


// ตารางเทียบค่า (Lookup Table) จาก Datasheet รุ่น 1005
long raw_data[]   = {409, 915, 1522, 2066, 2523, 2908, 3256, 3572, 3686};
float speed_mps[] = {0.00, 1.07, 2.01, 3.00, 3.97, 4.96, 5.98, 6.99, 7.23};
int data_points = 9; // จำนวนจุดข้อมูลในตาราง

unsigned long raw;
float windSpeed;

// --- ฟังก์ชันอ่านค่าดิบจาก I2C ---
unsigned long readFS3000() {
  Wire.requestFrom(FS3000_ADDR, 5); // ขอข้อมูล 5 Byte
  
  if (Wire.available() != 5) return 0; // ถ้ามาไม่ครบ 5 กลับบ้าน
  
  byte checksum = Wire.read();
  byte data_high = Wire.read();
  byte data_low = Wire.read();
  byte generic_sum1 = Wire.read();
  byte generic_sum2 = Wire.read();
  
  // รวมค่า High + Low เป็นตัวเลขเดียว
  unsigned long raw_value = (data_high << 8) | data_low;
  return raw_value;
}

// --- ฟังก์ชันคำนวณความเร็วลม (Linear Interpolation) ---
float calculateWindSpeed(unsigned long raw) {
  // 1. ถ้าน้อยกว่าค่าต่ำสุด ให้เป็น 0
  if (raw <= raw_data[0]) return 0.0;
  
  // 2. ถ้ามากกว่าค่าสูงสุด ให้เอาค่าสูงสุดเลย
  if (raw >= raw_data[data_points - 1]) return speed_mps[data_points - 1];

  // 3. เทียบช่วง (Interpolate)
  for (int i = 0; i < data_points - 1; i++) {
    if (raw >= raw_data[i] && raw <= raw_data[i+1]) {
      // สูตรเทียบบัญญัติไตรยางศ์ในช่วงนั้นๆ
      float ratio = (float)(raw - raw_data[i]) / (raw_data[i+1] - raw_data[i]);
      return speed_mps[i] + (ratio * (speed_mps[i+1] - speed_mps[i]));
    }
  }
  return 0.0;
}

void Loop_WindSensor() {
   raw = readFS3000();
  
  if (raw > 0) {
     windSpeed = calculateWindSpeed(raw);
    
    Serial.print("Raw: ");
    Serial.print(raw);
    Serial.print("\tSpeed: ");
    Serial.print(windSpeed);
    Serial.println(" m/s");
  } else {
    Serial.println("Sensor Error!");
  }

}

void resetI2C() {
  pinMode(21, OUTPUT); // SDA
  pinMode(22, OUTPUT); // SCL
  digitalWrite(21, HIGH);
  digitalWrite(22, HIGH);
  // ส่งสัญญาณ Clock 9 ครั้ง เพื่อเคลียร์ Bus
  for(int i=0; i<9; i++) {
    digitalWrite(22, LOW); delayMicroseconds(10);
    digitalWrite(22, HIGH); delayMicroseconds(10);
  }
  // สั่ง Stop condition
  digitalWrite(21, LOW); delayMicroseconds(10);
  digitalWrite(22, LOW); delayMicroseconds(10);
  digitalWrite(22, HIGH); delayMicroseconds(10);
  digitalWrite(21, HIGH); delayMicroseconds(10);
}