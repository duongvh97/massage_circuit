#include "core_esp8266_features.h"
#include "gpio_controll.h"
#include "serial.h"
#include "lcd.h"
#include "ir.h"

static int timeSec = 0;

GpioController::GpioController() {
  timeInterval = 200;
  currentMotorSpeed = 0;
  currentFanSpeed = 0;
  lastMotorSpeed = 0;
  lastFanSpeed = 0;
  currentMillis = millis();
  warmLightState = false;
  isPaused = false;
}

void GpioController::initGpio() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_Pin, OUTPUT);
  pinMode(Fan_Pin, OUTPUT);
  pinMode(Relay_1_Pin, OUTPUT);
  // pinMode(Relay_2_Pin, OUTPUT);
  analogWrite(PWM_Pin, PWM_OFF);
  analogWrite(Fan_Pin, PWM_OFF);
  digitalWrite(Relay_1_Pin, HIGH);
}

void GpioController::setCurrentMotorSpeed(const uint8_t value) {
  currentMotorSpeed = value;
}

uint8_t GpioController::getCurrentMotorSpeed() {
  return currentMotorSpeed;
}

void GpioController::setCurrentFanSpeed(const uint8_t value) {
  currentFanSpeed = value;
}

uint8_t GpioController::getCurrentFanSpeed() {
  return currentFanSpeed;
}

Ticker GpioController::getTicker() {
  return m_ticker;
}

Ticker GpioController::getTickerFan() {
  return m_tickerFan;
}

Ticker GpioController::getTickerWarmLight() {
  return m_TickerWarmLight;
}

Ticker GpioController::getTickerCoutdownSec() {
  return m_countdownSec;
}

void GpioController::blinkLed() {
  if (millis() - currentMillis >= timeInterval) {
    currentMillis = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void GpioController::setMotorSpeed(const uint8_t value) {
  if (currentMotorSpeed < value) {
    m_ticker.attach_ms(DELAY_OUTPUT_PWM, adjustPWMMotor, (char)value);

  } else if (currentMotorSpeed > value) {
    m_ticker.attach_ms(DELAY_OUTPUT_PWM, adjustPWMMotor, (char)value);
  }
}

void GpioController::setFanSpeed(const uint8_t value) {
  if (currentFanSpeed < value) {
    m_tickerFan.attach_ms(DELAY_OUTPUT_PWM_FAN, adjustPWMFan, (char)value);

  } else if (currentFanSpeed > value) {
    m_tickerFan.attach_ms(DELAY_OUTPUT_PWM_FAN, adjustPWMFan, (char)value);
  }
}

void GpioController::turnOffAll() {
  setMotorSpeed(PWM_OFF);
  setFanSpeed(PWM_OFF);
  controllWarmLight(TURN_OFF);

  if (SerialCommand::getInstance()->getSetTimeoutState()) {
    SerialCommand::getInstance()->detachTicker();
  }

  if (IR::getInstance()->getSetTimeoutState()) {
    IR::getInstance()->detachTicker();
  }
}

void GpioController::controllWarmLight(const bool value) {
  if (value) {
    digitalWrite(Relay_1_Pin, LOW);
    m_TickerWarmLight.attach(DELAY_MINUTES_TURN_OFF_WARMLIGHT * 60, turnOffWarmLight);
    m_countdownSec.attach(1, coundownSecWarmLight, DELAY_MINUTES_TURN_OFF_WARMLIGHT * 60);
    timeSec = DELAY_MINUTES_TURN_OFF_WARMLIGHT * 60;
    warmLightState = true;
  } else {
    digitalWrite(Relay_1_Pin, HIGH);
    m_countdownSec.detach();
    m_TickerWarmLight.detach();
    LCD::getInstance()->lcdPrintTimeoutWarmLight(0, 0);
    timeSec = 0;
    warmLightState = false;
  }
}

bool GpioController::getWarmLightState() {
  return warmLightState;
}

void adjustPWMMotor(const char value) {
  uint8_t currentSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
  if (currentSpeed < value) {
    currentSpeed++;
    GpioController::getInstance()->setCurrentMotorSpeed(currentSpeed);
  } else {
    currentSpeed--;
    GpioController::getInstance()->setCurrentMotorSpeed(currentSpeed);
  }
  analogWrite(PWM_Pin, currentSpeed);
  if (currentSpeed == value) {
    GpioController::getInstance()->getTicker().detach();
  }
}

void adjustPWMFan(const char value) {
  uint8_t currentSpeed = GpioController::getInstance()->getCurrentFanSpeed();
  if (currentSpeed < value) {
    currentSpeed++;
    GpioController::getInstance()->setCurrentFanSpeed(currentSpeed);
  } else {
    currentSpeed--;
    GpioController::getInstance()->setCurrentFanSpeed(currentSpeed);
  }
  analogWrite(Fan_Pin, currentSpeed);
  if (currentSpeed == value) {
    GpioController::getInstance()->getTickerFan().detach();
  }
}

void turnOffWarmLight() {
  GpioController::getInstance()->controllWarmLight(TURN_OFF);
  GpioController::getInstance()->getTickerWarmLight().detach();
}

void coundownSecWarmLight(const int data) {
  LCD::getInstance()->lcdPrintTimeoutWarmLight(1, --timeSec);

  if (timeSec == 0) {
    GpioController::getInstance()->getTickerCoutdownSec().detach();
  }
}

void GpioController::pause() {
  if (!isPaused && getWarmLightState()) {
    m_countdownSec.detach();
    m_TickerWarmLight.detach();
    controllWarmLight(TURN_OFF);
    isPaused = true;
  }
  lastMotorSpeed = getCurrentMotorSpeed();
  lastFanSpeed = getCurrentFanSpeed();
  setMotorSpeed(PWM_OFF);
  setFanSpeed(PWM_OFF);
}

void GpioController::resume() {
  if (isPaused && !getWarmLightState()) {
    m_TickerWarmLight.attach(timeSec, turnOffWarmLight);
    m_countdownSec.attach(1, coundownSecWarmLight, timeSec);
    controllWarmLight(TURN_ON);
    isPaused = false;
  }
  setMotorSpeed(lastMotorSpeed);
  setFanSpeed(lastFanSpeed);
}