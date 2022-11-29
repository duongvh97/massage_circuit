#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "common.h"
#include <Arduino.h>

enum SERIAL_STATE {
  WAITING_START_BYTE = 0,
  WAITING_RECEIVE_DATA_BYTE,
  WAITING_STOP_BYTE
};

class SerialCommand {
private:
  SERIAL_STATE state;
  bool isReceiveDataDone;
  char buffer;

public:
  static SerialCommand* getInstance() {
    static SerialCommand obj;
    return &obj;
  };
  SerialCommand();
  void serialInit();
  void getSerialCommand();
  void handleSerialCommand();
  void serialLoop();
};




#endif  // _SERIAL_H_
