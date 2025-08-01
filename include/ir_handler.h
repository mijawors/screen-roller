#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include <Arduino.h>

void handleIrCommand(uint8_t code);
void setIsExtended(bool state);
bool getIsExtended();
void initIrHandler();

#endif