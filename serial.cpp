#include "HardwareSerial.h"
#include "serial.h"
#include "gpio_controll.h"
#include "sensor.h"
#include "lcd.h"
#include "ir.h"

static int timeSec = 0;

SerialCommand::SerialCommand() {
  state = WAITING_START_BYTE;
  isReceiveDataDone = false;
  buffer = 0x00;
  isSetTimeout = false;
}

void SerialCommand::initSerial() {
  Serial.begin(DEFAULT_BAUDRATE);
}

void SerialCommand::getSerialCommand() {
  while (Serial.available() > 0) {
    char temp = Serial.read();
    switch (state) {
      case WAITING_START_BYTE:
        if (temp == START_BYTE) {
          state = WAITING_RECEIVE_DATA_BYTE;
        }
        break;
      case WAITING_RECEIVE_DATA_BYTE:
        if (temp == STOP_BYTE) {
          state = WAITING_STOP_BYTE;
          break;
        }
        buffer = temp;
        state = WAITING_STOP_BYTE;
        break;
      case WAITING_STOP_BYTE:
        if (temp == STOP_BYTE) {
          state = WAITING_START_BYTE;
          isReceiveDataDone = true;
        }
        break;
    }
  }
}

void SerialCommand::handleSerialCommand() {
  if (isReceiveDataDone) {
    isReceiveDataDone = false;
    switch (buffer) {
      case CMD_TURN_OFF_ALL:
        Serial.print("Receive CMD_TURN_OFF_ALL\n");
        GpioController::getInstance()->turnOffAll();
        buffer = 0x00;
        break;
      case CMD_SET_MOTOR_SPEED_1:
        Serial.print("Receive CMD_SET_MOTOR_SPEED_1\n");
        GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_1);
        buffer = 0x00;
        break;
      case CMD_SET_MOTOR_SPEED_2:
        Serial.print("Receive CMD_SET_MOTOR_SPEED_2\n");
        GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_2);
        buffer = 0x00;
        break;
      case CMD_SET_MOTOR_SPEED_3:
        Serial.print("Receive CMD_SET_MOTOR_SPEED_3\n");
        GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_3);
        buffer = 0x00;
        break;
      case CMD_SET_MOTOR_SPEED_4:
        Serial.print("Receive CMD_SET_MOTOR_SPEED_4\n");
        GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_4);
        buffer = 0x00;
        break;
      case CMD_TURN_OFF_ALL_MOTOR:
        Serial.print("Receive CMD_TURN_OFF_ALL_MOTOR\n");
        GpioController::getInstance()->setMotorSpeed(PWM_OFF);
        buffer = 0x00;
        break;
      case CMD_TURN_ON_WARM_LIGHT:
        Serial.print("Receive CMD_TURN_ON_WARM_LIGHT\n");
        GpioController::getInstance()->controllWarmLight(TURN_ON);
        buffer = 0x00;
        break;
      case CMD_TURN_OFF_WARM_LIGHT:
        Serial.print("Receive CMD_TURN_OFF_WARM_LIGHT\n");
        GpioController::getInstance()->controllWarmLight(TURN_OFF);
        buffer = 0x00;
        break;
      case CMD_TURN_ON_FAN:
        Serial.print("Receive CMD_TURN_ON_FAN\n");
        GpioController::getInstance()->setFanSpeed(PWM_LEVEL_4);
        buffer = 0x00;
        break;
      case CMD_TURN_OFF_FAN:
        Serial.print("Receive CMD_TURN_OFF_FAN\n");
        GpioController::getInstance()->setFanSpeed(PWM_OFF);
        buffer = 0x00;
        break;
      case CMD_REPORT_STATE:
        reportState();
        buffer = 0x00;
        break;
      case CMD_SET_TIME_OUT:
        Serial.print("Receive CMD_SET_TIME_OUT\n");
        if (IR::getInstance()->getSetTimeoutState()) {
          IR::getInstance()->detachTicker();
        }
        if (!isSetTimeout) {
          m_ticker_countdown_sec.attach_ms(1000, countdownSec, DELAY_MINUTES_TURN_OFF_DEVICE * 60);
          m_ticker_countdown.attach(DELAY_MINUTES_TURN_OFF_DEVICE * 60, turnOffDevice);
          isSetTimeout = true;
          timeSec = DELAY_MINUTES_TURN_OFF_DEVICE * 60;
          buffer = 0x00;
        } else {
          detachTicker();
        }
        break;
      default:
        break;
    }
  }
}

void SerialCommand::detachTicker() {
  timeSec = 0;
  m_ticker_countdown.detach();
  m_ticker_countdown_sec.detach();
  setTimeoutState(false);
  LCD::getInstance()->lcdPrintTimeout(0, timeSec);
}

void SerialCommand::reportState() {
  /* STARTBYTE MT_SPEED F_SPEED WL_STATE SS_STATE STOPBYTE */
  uint8_t currentMotorSpeed = GpioController::getInstance()->getCurrentMotorSpeed();
  uint8_t currentFanSpeed = GpioController::getInstance()->getCurrentFanSpeed();
  bool currentWarmLightState = GpioController::getInstance()->getWarmLightState();
  bool currentSensorState = Sensor::getInstance()->getSensorState();

  dataReport[0] = START_BYTE;
  dataReport[5] = STOP_BYTE;
  switch (currentMotorSpeed) {
    case PWM_OFF:
      dataReport[1] = 0x00;
      break;
    case PWM_LEVEL_1:
      dataReport[1] = 0x01;
      break;
    case PWM_LEVEL_2:
      dataReport[1] = 0x02;
      break;
    case PWM_LEVEL_3:
      dataReport[1] = 0x03;
      break;
    case PWM_LEVEL_4:
      dataReport[1] = 0x04;
      break;
    default:
      break;
  }

  switch (currentFanSpeed) {
    case PWM_OFF:
      dataReport[2] = 0x00;
      break;
    case PWM_LEVEL_1:
      dataReport[2] = 0x01;
      break;
    case PWM_LEVEL_2:
      dataReport[2] = 0x02;
      break;
    case PWM_LEVEL_3:
      dataReport[2] = 0x03;
      break;
    case PWM_LEVEL_4:
      dataReport[2] = 0x04;
      break;
    default:
      break;
  }

  if (currentWarmLightState) {
    dataReport[3] = 0x01;
  } else {
    dataReport[3] = 0x00;
  }

  if (currentSensorState) {
    dataReport[4] = 0x01;
  } else {
    dataReport[4] = 0x00;
  }

  for (uint8_t i = 0; i < REPORT_DATA_SIZE; i++) {
    Serial.print(dataReport[i]);
    dataReport[i] = 0;
  }
}

void SerialCommand::serialLoop() {
  getSerialCommand();
  handleSerialCommand();
}

Ticker SerialCommand::getTicker() {
  return m_ticker_countdown;
}

Ticker SerialCommand::getTickerCoutdownSec() {
  return m_ticker_countdown_sec;
}

bool SerialCommand::getSetTimeoutState() {
  return isSetTimeout;
}

void SerialCommand::setTimeoutState(const bool state) {
  isSetTimeout = state;
}

static void turnOffDevice() {
  GpioController::getInstance()->turnOffAll();
  SerialCommand::getInstance()->setTimeoutState(false);
  SerialCommand::getInstance()->getTicker().detach();
  timeSec = 0;
  IR::getInstance()->getTickerCoutdownSec().detach();
  LCD::getInstance()->lcdPrintTimeout(0, timeSec);
}

static void countdownSec(const int data) {
  LCD::getInstance()->lcdPrintTimeout(1, --timeSec);
  if (timeSec == 0) {
    SerialCommand::getInstance()->getTickerCoutdownSec().detach();
    LCD::getInstance()->lcdPrintTimeout(0, timeSec);
  }
}

void SerialCommand::pause() {
  m_ticker_countdown.detach();
  m_ticker_countdown_sec.detach();
}

void SerialCommand::resume() {
  m_ticker_countdown.attach(timeSec, turnOffDevice);
  m_ticker_countdown_sec.attach_ms(1000, countdownSec, timeSec);
}
