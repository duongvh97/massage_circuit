#include "Esp.h"
#include "network.h"
#include "serial.h"
#include "ir.h"
#include "gpio_controll.h"
#include "lcd.h"
#include "common.h"
#include <Arduino.h>
#include <Ticker.h>

WiFiServer server(80);
bool isSetTimeout;
Ticker m_ticker_countdown;
Ticker m_ticker_countdown_sec;
static int timeSec = 0;

static void turnOffDevice() {
  GpioController::getInstance()->turnOffAll();
  SerialCommand::getInstance()->setTimeoutState(false);
  SerialCommand::getInstance()->getTicker().detach();
  timeSec = 0;
  SerialCommand::getInstance()->getTickerCoutdownSec().detach();
  LCD::getInstance()->lcdPrintTimeout(0, timeSec);
}

static void countdownSec(const int data) {
  LCD::getInstance()->lcdPrintTimeout(1, --timeSec);
  if (timeSec == 0) {
    SerialCommand::getInstance()->getTickerCoutdownSec().detach();
    LCD::getInstance()->lcdPrintTimeout(0, timeSec);
  }
}

String request = "";
String html = R"***(
<!DOCTYPE html>
<html>
  <head>    
    <title>MASSAGE CIRCUIT</title>
  </head>
  <body>
    <h1>MASSAGE CIRCUIT</h1>
    <form id='SP1' action='SET_SP1'><input class='button' type='submit' value='SPEED 1' ></form><br>
    <form id='SP2' action='SET_SP2'><input class='button' type='submit' value='SPEED 2' ></form><br>
    <form id='SP3' action='SET_SP3'><input class='button' type='submit' value='SPEED 3' ></form><br>
    <form id='SP4' action='SET_SP4'><input class='button' type='submit' value='SPEED 4' ></form><br>
    <form id='SP5' action='TURN_OFF_MOTOR'><input class='button' type='submit' value='TURN OFF MOTOR' ></form><br>
    <form id='SP6' action='TURN_OFF_FAN'><input class='button' type='submit' value='TURN OFF FAN' ></form><br>
    <form id='SP7' action='TURN_ON_FAN'><input class='button' type='submit' value='TURN ON FAN' ></form><br>
    <form id='SP8' action='TURN_OFF_LIGHT'><input class='button' type='submit' value='TURN OFF LIGHT' ></form><br>
    <form id='SP9' action='TURN_ON_LIGHT'><input class='button' type='submit' value='TURN ON LIGHT' ></form><br>
    <form id='SP10' action='TIMER'><input class='button' type='submit' value='TIMER' ></form><br>
    <form id='SP11' action='TURN_OFF_DEVICE'><input class='button' type='submit' value='TURN OFF DEVICE' ></form><br>
  </body>
</html>
)***";

WiFiClient client;

void Network::network_init() {
  bool status = WiFi.softAP("MASSAGE CIRCUIT", "123456789");
  if (status) {
    Serial.println("Create AP success!");
    server.begin();
  }
}

void Network::network_loop() {
  // Check if a client has connected
  client = server.available();
  if (!client) { return; }
  request = client.readStringUntil('\r');
  if (request.indexOf("SET_SP1") > 0) {
    Serial.print("Receive CMD_SET_MOTOR_SPEED_1\n");
    GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_1);
  } else if (request.indexOf("SET_SP2") > 0) {
    Serial.print("Receive CMD_SET_MOTOR_SPEED_2\n");
    GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_2);
  } else if (request.indexOf("SET_SP3") > 0) {
    Serial.print("Receive CMD_SET_MOTOR_SPEED_3\n");
    GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_3);
  } else if (request.indexOf("SET_SP4") > 0) {
    Serial.print("Receive CMD_SET_MOTOR_SPEED_4\n");
    GpioController::getInstance()->setMotorSpeed(PWM_LEVEL_4);
  } else if (request.indexOf("TURN_OFF_MOTOR") > 0) {
    Serial.print("Receive CMD_TURN_OFF_ALL_MOTOR\n");
    GpioController::getInstance()->setMotorSpeed(PWM_OFF);
  } else if (request.indexOf("TURN_OFF_FAN") > 0) {
    Serial.print("Receive CMD_TURN_OFF_FAN\n");
    GpioController::getInstance()->setFanSpeed(PWM_OFF);
  } else if (request.indexOf("TURN_ON_FAN") > 0) {
    Serial.print("Receive CMD_TURN_ON_FAN\n");
    GpioController::getInstance()->setFanSpeed(PWM_LEVEL_4);
  } else if (request.indexOf("TURN_OFF_LIGHT") > 0) {
    Serial.print("Receive CMD_TURN_OFF_WARM_LIGHT\n");
    GpioController::getInstance()->controllWarmLight(TURN_OFF);
  } else if (request.indexOf("TURN_ON_LIGHT") > 0) {
    Serial.print("Receive CMD_TURN_ON_WARM_LIGHT\n");
    GpioController::getInstance()->controllWarmLight(TURN_ON);
  } else if (request.indexOf("TIMER") > 0) {
    Serial.print("Receive CMD_SET_TIME_OUT\n");
    if (IR::getInstance()->getSetTimeoutState()) {
      IR::getInstance()->detachTicker();
    }
    if (!isSetTimeout) {
      m_ticker_countdown_sec.attach_ms(1000, countdownSec, DELAY_MINUTES_TURN_OFF_DEVICE * 60);
      m_ticker_countdown.attach(DELAY_MINUTES_TURN_OFF_DEVICE * 60, turnOffDevice);
      isSetTimeout = true;
      timeSec = DELAY_MINUTES_TURN_OFF_DEVICE * 60;
    } else {
      timeSec = 0;
      m_ticker_countdown.detach();
      m_ticker_countdown_sec.detach();
      isSetTimeout = false;
      LCD::getInstance()->lcdPrintTimeout(0, timeSec);
    }
  } else if(request.indexOf("TURN_OFF_DEVICE") > 0) {
    Serial.print("Receive CMD_TURN_OFF_ALL\n");
    GpioController::getInstance()->turnOffAll();
  }

  // Read the first line of the request
  request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  client.print(html);
}