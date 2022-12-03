#include "ir.h"
#include "lcd.h"

IR::IR() {
  irrecv = new IRrecv(IRRev_Pin);
}

void IR::initIR() {
  irrecv->enableIRIn();
}

void IR::IRLoop() {
  if (irrecv->decode(&results)) {
    switch (results.value) {
      case CH1_KEY:
        Serial.println("CH1_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case CH2_KEY:
        Serial.println("CH2_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case CH3_KEY:
        Serial.println("CH3_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case PREV_KEY:
        Serial.println("PREV_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case NEXT_KEY:
        Serial.println("NEXT_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case PLAY_KEY:
        Serial.println("PLAY_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case VOL1_KEY:
        Serial.println("VOL1_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case VOL2_KEY:
        Serial.println("VOL2_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      case EQ_KEY:
        Serial.println("EQ_KEY");
        LCD::getInstance()->lcdPrint(2, 0, results.value);
        break;
      default:
        break;
    }
    irrecv->resume();
  }
}