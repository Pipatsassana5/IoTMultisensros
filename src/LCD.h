#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

// กำหนดขาที่ต่อกับ ESP32
#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI   23  // ต่อเข้าขา SDA ของจอ
#define TFT_SCLK   18  // ต่อเข้าขา SCL ของจอ

// สร้าง Object จอ
// หมายเหตุ: จอจีนบางรุ่นมี "Tab" สีต่างกัน (Green/Red/Black) 
// ถ้าสีเพี้ยน หรือภาพล้นจอ เดี๋ยวเรามาแก้ตรง initR กัน
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void LCD_setup() {
 Serial.print("Hello! ST7735 TFT Test");

  // เริ่มต้นจอ (ลองเลือกเปิดทีละบรรทัด ถ้าภาพไม่ขึ้น)
  
  // แบบที่ 1: จอส่วนใหญ่ใช้แบบนี้ (Black Tab)
  tft.initR(INITR_BLACKTAB);      
  
  // แบบที่ 2: ถ้าสีเพี้ยน ลองอันนี้ (Green Tab)
  // tft.initR(INITR_GREENTAB); 
  
  // แบบที่ 3: ถ้าภาพเลื่อน/ล้น (Red Tab)
  // tft.initR(INITR_REDTAB);   

  tft.fillScreen(ST7735_BLACK); // ล้างจอเป็นสีดำ (ถ้าจอดำได้แสดงว่าสำเร็จ!)
  
  // แสดงข้อความ
  tft.setRotation(1); // หมุนแนวนอน (0-3)
  
}

void LCD_showIP(IPAddress ip) {
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("IP:");
  tft.print(ip);
}


void LCD_loop(float temperature, float humidity, uint16_t pm25_cf1, uint16_t pm25_atm, int CO2, float windSpeed) {
  // ล้างพื้นที่แสดงผลเก่า
  tft.fillRect(0, 60, tft.width(), 40, ST7735_BLACK);
  
  // แสดงค่าอุณหภูมิ
  tft.setTextColor(ST7735_CYAN, ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.print("Temp:");
  tft.print(temperature);
  tft.println("C");
  
  // แสดงค่าความชื้น
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setCursor(10, 50);
  tft.print("Hum:");
  tft.print(humidity);
  tft.println("%");
    // แสดงค่า PM2.5
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 70);
    tft.print("PM2.5 CF=1:");
    tft.print(pm25_cf1);
    tft.println("ug/m3");
    tft.setCursor(10, 85);
    tft.print("PM2.5 Atm:");
    tft.print(pm25_atm);
    tft.println("ug/m3");

    //Co2
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 100);
    tft.print("CO2:");
    tft.print(CO2);
    tft.println("ppm");

    // Wind Speed
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 115);
    tft.print("Wind:");
    tft.print(windSpeed);
    tft.println("m/s");
}

void LCD_test(){
tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
}
