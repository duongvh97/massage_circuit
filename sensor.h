#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "Ticker.h"
#include "common.h"
#include <Arduino.h>

class Sensor {
private:
  bool m_sensorState;
  uint8_t count;
  Ticker m_ticker;
public:
  Sensor();

  static Sensor* getInstance() {
    static Sensor obj;
    return &obj;
  }
  void initSensor();
  void readSensorValue();
  bool getSensorState();
};

void sensorLoop();

#endif