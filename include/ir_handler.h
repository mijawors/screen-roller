#pragma once

void irHandlerInit();
void irHandlerLoop();
void handleIrCommand(uint8_t code);
bool confirmWakeupWithIR();
void prepareIrForSleep();