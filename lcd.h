#ifndef _LCD_H_
#define _LCD_H_

#include "iostream"
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LCD {
private:
  uint8_t lastMotorSpeed;
  uint8_t lastFanSpeed;
  bool lastWarmLightState;
  bool lastSensorState;
public:
  static LCD* getInstance() {
    static LCD obj;
    return &obj;
  }
  LCD() {
    lastMotorSpeed = 0;
    lastFanSpeed = 0;
    lastWarmLightState = false;
    lastSensorState = false;
  }
  void initLCD();
  void lcdPrint(const uint8_t row, const uint8_t col, const char* data);
  void lcdPrint(const uint8_t row, const uint8_t col, const uint8_t data);
  void lcdPrintMotorSpeed(const uint8_t data);
  void lcdPrintFanSpeed(const uint8_t data);
  void lcdPrintWarmLightState(const bool data);
  void lcdPrintSensor(const bool data);
  void lcdPrintTimeout(const bool state, const int data);
  void lcdPrintTimeoutWarmLight(const bool state, const int data);
  void lcdRefresh();
};

#endif  // _SERIAL_H_
