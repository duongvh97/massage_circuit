#include "sensor.h"
#include "lcd.h"
bool currentSsValue;

Sensor::Sensor() {
  m_sensorState = 1;
  count = 0;
}

void Sensor::initSensor() {
  pinMode(Sensor_Pin, INPUT);
  m_ticker.attach_ms(DELAY_READ_SENSOR, sensorLoop);
}

bool Sensor::getSensorState() {
  return m_sensorState;
}

void Sensor::readSensorValue() {
  currentSsValue = digitalRead(Sensor_Pin);
  if (!currentSsValue) {
    count++;
    if (count == TIMES_TO_READ_SENSOR) {
      count = 0;
      m_sensorState = 1;
    }
  } else {
    count = 0;
    m_sensorState = 0;
  }
}

void sensorLoop() {
  Sensor::getInstance()->readSensorValue();
}
