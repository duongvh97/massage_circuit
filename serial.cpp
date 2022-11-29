#include "serial.h"
#include "gpio_controll.h"

SerialCommand::SerialCommand() {
  state = WAITING_START_BYTE;
  isReceiveDataDone = false;
  buffer = 0x00;
}

void SerialCommand::serialInit() {
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
        buffer = 0x00;
        break;
      case CMD_TURN_OFF_WARM_LIGHT:
        Serial.print("Receive CMD_TURN_OFF_WARM_LIGHT\n");
        buffer = 0x00;
        break;
      case CMD_TURN_ON_FAN:
        Serial.print("Receive CMD_TURN_ON_FAN\n");
        buffer = 0x00;
        break;
      case CMD_TURN_OFF_FAN:
        Serial.print("Receive CMD_TURN_OFF_FAN\n");
        buffer = 0x00;
        break;
      case CMD_REPORT_STATE:
        Serial.print("Receive CMD_REPORT_STATE\n");
        buffer = 0x00;
        break;
      case CMD_SET_TIME_OUT:
        Serial.print("Receive CMD_SET_TIME_OUT\n");
        buffer = 0x00;
        break;
      default:
        break;
    }
  }
}

void SerialCommand::serialLoop() {
  getSerialCommand();
  handleSerialCommand();
}
