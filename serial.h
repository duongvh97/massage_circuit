#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "common.h"
#include <Arduino.h>
#include <Ticker.h>

enum SERIAL_STATE {
  WAITING_START_BYTE = 0,
  WAITING_RECEIVE_DATA_BYTE,
  WAITING_STOP_BYTE
};

class SerialCommand {
private:
  SERIAL_STATE state;
  bool isReceiveDataDone;
  bool isSetTimeout;
  char buffer;
  char dataReport[REPORT_DATA_SIZE];
  Ticker m_ticker_countdown;
  Ticker m_ticker_countdown_sec;

public:
  static SerialCommand* getInstance() {
    static SerialCommand obj;
    return &obj;
  };
  SerialCommand();
  void initSerial();
  void getSerialCommand();
  void handleSerialCommand();
  void serialLoop();
  void reportState();
  bool getSetTimeoutState();
  void setTimeoutState(const bool state);
  Ticker getTicker();
  Ticker getTickerCoutdownSec();
  void detachTicker();
};

static void turnOffDevice();
static void countdownSec(const int data);


#endif  // _SERIAL_H_
