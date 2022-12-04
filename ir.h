#ifndef _IR_H_
#define _IR_H_

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <Ticker.h>
#include "common.h"

class IR {
private:
  decode_results results;
  IRrecv* irrecv;
  Ticker m_ticker_countdown;
  Ticker m_ticker_countdown_sec;
  bool isSetTimeout;
public:
  IR();
  ~IR() {
    irrecv = NULL;
    delete irrecv;
  }
    static IR* getInstance() {
    static IR obj;
    return &obj;
  }
  void initIR();
  void IRLoop();
  void setTimeoutState(const bool state);
  bool getSetTimeoutState();
  Ticker getTickerCoutdown();
  Ticker getTickerCoutdownSec();
  void detachTicker();
};

static void turnOffDevice();
static void countdownSec(const int data);

#endif  // _SERIAL_H_
