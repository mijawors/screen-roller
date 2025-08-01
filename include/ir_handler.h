#pragma once

#include <Arduino.h>

void handleIrCommand(uint8_t code);
void setIsExtended(bool state);
bool getIsExtended();
void initIrHandler();