#pragma once

void irHandlerInit();
void irHandlerLoop();
void handleIrCommand(uint8_t code);
bool confirmWakeupWithIR();
void prepareIrForSleep();
void initPowerHandler();
void handleWakeup();
void goToSleep();
void handleInactivitySleep();
void updateActivity();