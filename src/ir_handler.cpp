#include <Arduino.h>
#include <ir_handler.h>
#include <screen_controller.h>

extern unsigned long lastIrActivity;

void goToSleep();

void initIrHandler() {
  initScreenController();
}

void handleIrCommand(uint8_t code) {
  lastIrActivity = millis();

  switch (code) {
    case 0x16:  // '*'
      Serial.println("😴 Going to sleep – press * to wake up");
      goToSleep();
      break;

    case 0x45:  // '1'
      toggleScreen();
      break;

    case 0x18:  // ▼
      emergencyRetract();
      break;

    case 0x52:  // ▲
      emergencyExtend();
      break;

    default:
      Serial.print("⚠️ Unknown code: 0x");
      Serial.println(code, HEX);
      break;
  }
}