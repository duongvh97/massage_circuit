#ifndef GPIO_CONTROLLER_H
#define GPIO_CONTROLLER_H

#include "common.h"
#include <Arduino.h>
#include <Ticker.h>
#include <bits/stdc++.h>

class GpioController {
private:
  int timeInterval;
  int currentMillis;
  uint8_t currentMotorSpeed;
  uint8_t currentFanSpeed;
  Ticker m_ticker;
  Ticker m_tickerFan;
  Ticker m_TickerWarmLight;
  Ticker m_countdownSec;
  bool warmLightState;

public:
  GpioController();
  static GpioController* getInstance() {
    static GpioController obj;
    return &obj;
  }
  void initGpio();
  void blinkLed();

  void setMotorSpeed(const uint8_t value);
  void setFanSpeed(const uint8_t value);

  void setCurrentMotorSpeed(const uint8_t value);
  uint8_t getCurrentMotorSpeed();

  void setCurrentFanSpeed(const uint8_t value);
  uint8_t getCurrentFanSpeed();

  void turnOffAll();

  void controllWarmLight(const bool value);
  bool getWarmLightState();

  Ticker getTicker();
  Ticker getTickerFan();
  Ticker getTickerWarmLight();
  Ticker getTickerCoutdownSec();
};

void adjustPWMMotor(const char value);
void adjustPWMFan(const char value);
void turnOffWarmLight();
void coundownSecWarmLight(const int data);

#endif