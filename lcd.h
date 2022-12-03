#ifndef _LCD_H_
#define _LCD_H_

#include "iostream"
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LCD {
private:

public:
    static LCD* getInstance() {
    static LCD obj;
    return &obj;
  }
  void initLCD();
  void lcdPrint(const uint8_t row, const uint8_t col, const char* data);
  void lcdPrint(const uint8_t row, const uint8_t col, const uint8_t data);

};

#endif  // _SERIAL_H_
