#ifndef GPIO_CONTROLLER_H
#define GPIO_CONTROLLER_H

#include "common.h"
#include <Arduino.h>
#include <Ticker.h>

class GpioController {
private:
  int timeInterval;
  int currentMillis;
  uint8_t currentMotorSpeed;
  uint8_t currentFanSpeed;
  uint8_t mPWMFor;
  Ticker m_ticker;
  Ticker m_tickerFan;

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

  Ticker getTicker();
  Ticker getTickerFan();
  uint8_t getPWMFor();
};

void adjustPWM(const char value);
#endif