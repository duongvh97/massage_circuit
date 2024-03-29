#include "serial.h"
#include "gpio_controll.h"
#include "sensor.h"
#include "ir.h"
#include "lcd.h"
#include "network.h"

static bool isShowPause = false;
void runDefault();
void run();

void setup() {
  SerialCommand::getInstance()->initSerial();
  GpioController::getInstance()->initGpio();
  Sensor::getInstance()->initSensor();
  IR::getInstance()->initIR();
  LCD::getInstance()->initLCD();
  Network::getInstance()->network_init();

  runDefault();
}

void loop() {
  run();
  Network::getInstance()->network_loop();
}

void runDefault() {
  if (Sensor::getInstance()->getSensorState()) {
    GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_2);
  }
}

void run() {
  if (!Sensor::getInstance()->getSensorState()) {
    SerialCommand::getInstance()->serialLoop();
    GpioController::getInstance()->blinkLed();
    LCD::getInstance()->lcdRefresh();
    IR::getInstance()->IRLoop();
    if (isShowPause) {
      GpioController::getInstance()->resume();
      if (SerialCommand::getInstance()->getSetTimeoutState()) {
        SerialCommand::getInstance()->resume();
      }

      if (IR::getInstance()->getSetTimeoutState()) {
        IR::getInstance()->resume();
      }
      LCD::getInstance()->reloadScreen();
      isShowPause = false;
    }
  } else {
    // if (!isShowPause) {
    //   isShowPause = true;
    //   if (SerialCommand::getInstance()->getSetTimeoutState()) {
    //     SerialCommand::getInstance()->pause();
    //   }

    //   if (IR::getInstance()->getSetTimeoutState()) {
    //     IR::getInstance()->pause();
    //   }

    //   GpioController::getInstance()->pause();

    //   LCD::getInstance()->lcdClear();
    //   LCD::getInstance()->lcdPrint(1, 4, "SYSTEM PAUSE!");
    // }
  }
}
