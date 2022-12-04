#include "HardwareSerial.h"
#include "ir.h"
#include "lcd.h"
#include "gpio_controll.h"
#include "serial.h"

uint8_t tablePWMSpeed[5] = { PWM_OFF, PWM_LEVEL_1, PWM_LEVEL_2, PWM_LEVEL_3, PWM_LEVEL_4 };
static int timeSec = 0;

IR::IR() {
  irrecv = new IRrecv(IRRev_Pin);
  isSetTimeout = false;
}

void IR::initIR() {
  irrecv->enableIRIn();
}

void IR::IRLoop() {
  if (irrecv->decode(&results)) {
    uint8_t currentMotorSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
    uint8_t currentFanSpeed = GpioController::getInstance()->getCurrentFanSpeed();
    bool currentWarmLightState = GpioController::getInstance()->getWarmLightState();
    switch (results.value) {
      case CH1_KEY:
        // Turn on/off device
        GpioController::getInstance()->turnOffAll();
        break;
      case CH2_KEY:
        // Turn on/off fan
        if (currentFanSpeed == PWM_LEVEL_4) {
          GpioController::getInstance()->setFanSpeed(PWM_OFF);
        } else {
          GpioController::getInstance()->setFanSpeed(PWM_LEVEL_4);
        }
        break;
      case CH3_KEY:
        // Turn on/off warmlight
        if (currentWarmLightState) {
          GpioController::getInstance()->controllWarmLight(TURN_OFF);
        } else {
          GpioController::getInstance()->controllWarmLight(TURN_ON);
        }
        break;
      case PREV_KEY:
        // Increase motor speed
        for (uint8_t i = 0; i < sizeof(tablePWMSpeed) / sizeof(uint8_t); i++) {
          if (currentMotorSpeed == tablePWMSpeed[i]) {
            if (i < 4) {
              GpioController::getInstance()->setMotorSpeed(tablePWMSpeed[i + 1]);
            }
          }
        }
        break;
      case NEXT_KEY:
        // Decrease motor speed
        for (uint8_t i = 0; i < sizeof(tablePWMSpeed) / sizeof(uint8_t); i++) {
          if (currentMotorSpeed == tablePWMSpeed[i]) {
            if (i > 0) {
              GpioController::getInstance()->setMotorSpeed(tablePWMSpeed[i - 1]);
            }
          }
        }
        break;
      case PLAY_KEY:
        // Set timeout 15p
        if (SerialCommand::getInstance()->getSetTimeoutState()) {
          SerialCommand::getInstance()->detachTicker();
        }

        if (!isSetTimeout) {
          m_ticker_countdown_sec.attach_ms(1000, countdownSec, DELAY_MINUTES_TURN_OFF_DEVICE * 60);
          m_ticker_countdown.attach(DELAY_MINUTES_TURN_OFF_DEVICE * 60, turnOffDevice);
          timeSec = DELAY_MINUTES_TURN_OFF_DEVICE * 60;
          isSetTimeout = true;
        }
        else {
          detachTicker();
          isSetTimeout = false;
        }

        break;
      case VOL1_KEY:
        break;
      case VOL2_KEY:
        break;
      case EQ_KEY:
        break;
      default:
        break;
    }
    irrecv->resume();
  }
}

Ticker IR::getTickerCoutdown() {
  return m_ticker_countdown;
}

Ticker IR::getTickerCoutdownSec() {
  return m_ticker_countdown_sec;
}

void IR::setTimeoutState(const bool state) {
  isSetTimeout = state;
}

bool IR::getSetTimeoutState() {
  return isSetTimeout;
}

void IR::detachTicker() {
  timeSec = 0;
  m_ticker_countdown.detach();
  m_ticker_countdown_sec.detach();
  setTimeoutState(false);
  LCD::getInstance()->lcdPrintTimeout(0, timeSec);
}

static void turnOffDevice() {
  GpioController::getInstance()->turnOffAll();
  IR::getInstance()->setTimeoutState(false);
  IR::getInstance()->getTickerCoutdown().detach();
  timeSec = 0;
  IR::getInstance()->getTickerCoutdownSec().detach();
  LCD::getInstance()->lcdPrintTimeout(0, timeSec);
}

static void countdownSec(const int data) {
  LCD::getInstance()->lcdPrintTimeout(1, --timeSec);
  if (timeSec == 0) {
    IR::getInstance()->getTickerCoutdownSec().detach();
    LCD::getInstance()->lcdPrintTimeout(0, timeSec);
  }
}