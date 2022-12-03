#include "serial.h"
#include "gpio_controll.h"
#include "sensor.h"
#include "ir.h"
#include "lcd.h"

void setup() {
  SerialCommand::getInstance()->initSerial();
  GpioController::getInstance()->initGpio();
  Sensor::getInstance()->initSensor();
  IR::getInstance()->initIR();
  LCD::getInstance()->initLCD();
  LCD::getInstance()->lcdPrint(0, 0, "Duong");
  LCD::getInstance()->lcdPrint(1, 0, "Huyen");
}

void loop() {
  SerialCommand::getInstance()->serialLoop();
  GpioController::getInstance()->blinkLed();
  IR::getInstance()->IRLoop();
}
