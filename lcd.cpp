#include "lcd.h"

LiquidCrystal_I2C m_LCD(0x27, 20, 4);

void LCD::initLCD() {
  m_LCD.begin();
  m_LCD.backlight();
}

void LCD::lcdPrint(const uint8_t row, const uint8_t col, const char* data) {
  m_LCD.setCursor(col, row);
  m_LCD.printstr(data);
}
void LCD::lcdPrint(const uint8_t row, const uint8_t col, const uint8_t data) {
  m_LCD.setCursor(col, row);
  m_LCD.print(data);
}