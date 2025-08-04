#include <Arduino.h>
#include <IRremote.hpp>
#include <config.h>
#include <screen_controller.h>
#include <ir_handler.h>
#include <power_handler.h>

using namespace Config::V1;

extern unsigned long lastIrActivity;

void irHandlerInit() {
  IrReceiver.begin(Pins::IR_RECEIVE, ENABLE_LED_FEEDBACK);
  initScreenController();
}

void irHandlerLoop() {
  if (IrReceiver.decode()) {
    handleIrCommand(IrReceiver.decodedIRData.command);
    IrReceiver.resume();
  }
}

void prepareIrForSleep() {
  IrReceiver.end();
}

bool confirmWakeupWithIR() {
  Serial.println("📡 Woken up by IR – 15 seconds to confirm with `*`...");
  delay(300);
  IrReceiver.begin(Pins::IR_RECEIVE, ENABLE_LED_FEEDBACK);
  delay(200);

  unsigned long start = millis();
  while (millis() - start < 15000) {
    digitalWrite(Pins::LED, millis() % 500 < 250 ? HIGH : LOW);
    if (IrReceiver.decode()) {
      uint8_t code = IrReceiver.decodedIRData.command;
      Serial.printf("➡️ IR code: 0x%X\n", code);
      IrReceiver.resume();

      if (code == 0x16) { // '*' button
        Serial.println("✅ Confirmed with `*` button. Starting...");
        digitalWrite(Pins::LED, HIGH);
        updateActivity();
        return true;
      }
    }
    delay(10);
  }

  return false;
}

void handleIrCommand(uint8_t code) {
  updateActivity();

  switch (code) {
    case 0x16:  // '*'
      Serial.println("😴 Going to sleep – press * to wake up");
      prepareIrForSleep();
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
      esp_deep_sleep_start();
      break;

    case 0x45:  // '1'
      toggleScreen();
      break;

    case 0x18:  // ▼
      while (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.command != 0x18) break;
        emergencyRetract();
        IrReceiver.resume();
        delay(100);
      }
      break;

    case 0x52:  // ▲
      while (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.command != 0x52) break;
        emergencyExtend();
        IrReceiver.resume();
        delay(100);
      }
      break;

    default:
      Serial.print("⚠️ Unknown code: 0x");
      Serial.println(code, HEX);
      break;
  }
}