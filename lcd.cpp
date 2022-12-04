#include "lcd.h"
#include "gpio_controll.h"
#include "sensor.h"

LiquidCrystal_I2C m_LCD(0x27, 20, 4);

void LCD::initLCD() {
  m_LCD.begin();
  m_LCD.backlight();
  m_LCD.clear();
  lastMotorSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
  lastFanSpeed = GpioController::getInstance()->getCurrentFanSpeed();
  lastWarmLightState = GpioController::getInstance()->getWarmLightState();
  lastSensorState = Sensor::getInstance()->getSensorState();
  lcdPrintMotorSpeed(lastMotorSpeed);
  lcdPrintFanSpeed(lastFanSpeed);
  lcdPrintSensor(lastSensorState);
  lcdPrintWarmLightState(lastWarmLightState);
}

void LCD::lcdPrint(const uint8_t row, const uint8_t col, const char* data) {
  m_LCD.setCursor(col, row);
  m_LCD.printstr(data);
}
void LCD::lcdPrint(const uint8_t row, const uint8_t col, const uint8_t data) {
  m_LCD.setCursor(col, row);
  m_LCD.print(data);
}

void LCD::lcdPrintMotorSpeed(const uint8_t data) {
  lcdPrint(0, 0, "Motor:");
  switch (data) {
    case PWM_OFF:
      m_LCD.print("OFF");
      break;
    case PWM_LEVEL_1:
      m_LCD.print("1  ");
      break;
    case PWM_LEVEL_2:
      m_LCD.print("2  ");
      break;
    case PWM_LEVEL_3:
      m_LCD.print("3  ");
      break;
    case PWM_LEVEL_4:
      m_LCD.print("4  ");
      break;
  }
}

void LCD::lcdPrintFanSpeed(const uint8_t data) {
  lcdPrint(1, 0, "Fan:");
  if (data > 0) {
    m_LCD.print("ON ");
  } else {
    m_LCD.print("OFF");
  }
}

void LCD::lcdPrintSensor(const bool data) {
  lcdPrint(2, 0, "Sensor:");
  if (data) {
    m_LCD.print("ON ");
  } else {
    m_LCD.print("OFF");
  }
}

void LCD::lcdPrintWarmLightState(const bool data) {
  lcdPrint(3, 0, "Light:");
  if (data) {
    m_LCD.print("ON ");
  } else {
    m_LCD.print("OFF");
  }
}

void LCD::lcdPrintTimeout(const bool state, const int data) {
  lcdPrint(1, 11, "Sec:");
  m_LCD.print(data);
  m_LCD.print("   ");
  lcdPrint(0, 11, "TDev:");
  if (state) {
    m_LCD.print("ON ");
  } else {
    m_LCD.print("OFF");
  }
}
void LCD::lcdPrintTimeoutWarmLight(const bool state, const int data) {
  lcdPrint(3, 11, "Sec:");
  m_LCD.print(data);
  m_LCD.print("  ");
  lcdPrint(2, 11, "TLig:");
  if (state) {
    m_LCD.print("ON ");
  } else {
    m_LCD.print("OFF");
  }
}

void LCD::lcdRefresh() {
  uint8_t currentMotorSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
  uint8_t currentFanSpeed = GpioController::getInstance()->getCurrentFanSpeed();
  bool currentWarmLightState = GpioController::getInstance()->getWarmLightState();
  bool currentSensorState = Sensor::getInstance()->getSensorState();

  if (currentMotorSpeed != lastMotorSpeed) {
    lastMotorSpeed = currentMotorSpeed;
    lcdPrintMotorSpeed(currentMotorSpeed);
  }

  if (currentFanSpeed != lastFanSpeed) {
    lastFanSpeed = currentFanSpeed;
    lcdPrintFanSpeed(currentFanSpeed);
  }

  if (currentSensorState != lastSensorState) {
    lastSensorState = currentSensorState;
    lcdPrintSensor(currentSensorState);
  }

  if (currentWarmLightState != lastWarmLightState) {
    lastWarmLightState = currentWarmLightState;
    lcdPrintWarmLightState(currentWarmLightState);
  }
}

void LCD::lcdClear() {
  m_LCD.clear();
}

void LCD::reloadScreen() {
  uint8_t currentMotorSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
  uint8_t currentFanSpeed = GpioController::getInstance()->getCurrentFanSpeed();
  bool currentWarmLightState = GpioController::getInstance()->getWarmLightState();
  bool currentSensorState = Sensor::getInstance()->getSensorState();
  m_LCD.clear();
  lcdPrintMotorSpeed(currentMotorSpeed);
  lcdPrintFanSpeed(currentFanSpeed);
  lcdPrintSensor(currentSensorState);
  lcdPrintWarmLightState(currentWarmLightState);
}