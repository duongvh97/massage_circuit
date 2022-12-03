#include "core_esp8266_features.h"
#include "gpio_controll.h"
#include "serial.h"

GpioController::GpioController() {
  timeInterval = 1000;
  currentMotorSpeed = 0;
  currentFanSpeed = 0;
  mPWMFor = PWM_FOR_NONE;
  currentMillis = millis();
}

void GpioController::initGpio() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_Pin, OUTPUT);
  pinMode(Fan_Pin, OUTPUT);
  pinMode(Relay_1_Pin, OUTPUT);
  // pinMode(Relay_2_Pin, OUTPUT);
  analogWrite(PWM_Pin, PWM_OFF);
  analogWrite(Fan_Pin, PWM_OFF);
  digitalWrite(Relay_1_Pin, LOW);
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

uint8_t GpioController::getPWMFor() {
  return mPWMFor;
}

void GpioController::blinkLed() {
  if (millis() - currentMillis >= timeInterval) {
    currentMillis = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void GpioController::setMotorSpeed(const uint8_t value) {
  mPWMFor = PWM_FOR_MOTOR;
  Serial.print("Set PWM value: ");
  Serial.println(value);

  if (currentMotorSpeed < value) {
    m_ticker.attach_ms(DELAY_OUTPUT_PWM, adjustPWM, (char)value);

  } else if (currentMotorSpeed > value) {
    m_ticker.attach_ms(DELAY_OUTPUT_PWM, adjustPWM, (char)value);
  }
}

void GpioController::setFanSpeed(const uint8_t value) {
  mPWMFor = PWM_FOR_FAN;
  Serial.print("Set PWM Fan value: ");
  Serial.println(value);

  if (currentFanSpeed < value) {
    m_tickerFan.attach_ms(DELAY_OUTPUT_PWM, adjustPWM, (char)value);

  } else if (currentFanSpeed > value) {
    m_tickerFan.attach_ms(DELAY_OUTPUT_PWM, adjustPWM, (char)value);
  }
}

void adjustPWM(const char value) {
  uint8_t pwmType = GpioController::getInstance()->getPWMFor();
  if (pwmType == PWM_FOR_MOTOR) {
    uint8_t currentSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
    if (currentSpeed < value) {
      currentSpeed++;
      GpioController::getInstance()->setCurrentMotorSpeed(currentSpeed);
    } else {
      currentSpeed--;
      GpioController::getInstance()->setCurrentMotorSpeed(currentSpeed);
    }
    analogWrite(PWM_Pin, currentSpeed);
    Serial.println(currentSpeed);
    if (currentSpeed == value) {
      GpioController::getInstance()->getTicker().detach();
    }
  } else if (pwmType == PWM_FOR_FAN) {
    uint8_t currentSpeed = GpioController::getInstance()->getCurrentFanSpeed();
    if (currentSpeed < value) {
      currentSpeed++;
      GpioController::getInstance()->setCurrentFanSpeed(currentSpeed);
    } else {
      currentSpeed--;
      GpioController::getInstance()->setCurrentFanSpeed(currentSpeed);
    }
    analogWrite(PWM_Pin, currentSpeed);
    Serial.println(currentSpeed);
    if (currentSpeed == value) {
      GpioController::getInstance()->getTickerFan().detach();
    }
  }
}
