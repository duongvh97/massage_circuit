#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "serial.h"
#include "gpio_controll.h"

void setup() {
  SerialCommand::getInstance()->serialInit();
  GpioController::getInstance()->initGpio();
}

void loop() {
  SerialCommand::getInstance()->serialLoop();
  GpioController::getInstance()->blinkLed();
}
