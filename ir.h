#ifndef _IR_H_
#define _IR_H_

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "common.h"

class IR {
private:
  decode_results results;
  IRrecv* irrecv;
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
};


#endif  // _SERIAL_H_
