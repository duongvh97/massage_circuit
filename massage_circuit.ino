#include "serial.h"
#include "gpio_controll.h"
#include "sensor.h"
#include "ir.h"
#include "lcd.h"

void runDefault();

void setup() {
  SerialCommand::getInstance()->initSerial();
  GpioController::getInstance()->initGpio();
  Sensor::getInstance()->initSensor();
  IR::getInstance()->initIR();
  LCD::getInstance()->initLCD();  
}

void loop() {
  SerialCommand::getInstance()->serialLoop();
  GpioController::getInstance()->blinkLed();
  LCD::getInstance()->lcdRefresh();
  IR::getInstance()->IRLoop();
}

void runDefault() {
  
}
