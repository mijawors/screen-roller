#include <Arduino.h>
#include <IRremote.hpp>
#include <Preferences.h>
#include <esp_sleep.h>

#include <motors.h>
#include <ir_handler.h>
#include <ota_handler.h>
#include <config.h>

using namespace Config::V1;

unsigned long lastIrActivity = 0;

// ---------------- HELPERS ---------------------

void goToSleep() {
  delay(100);
  IrReceiver.end();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); // IR pin LOW
  Serial.println("Going to sleep... press a button on the remote (e.g., *)");
  delay(100);
  digitalWrite(Pins::LED, LOW);
  esp_deep_sleep_start();
}

void initHardware() {
  pinMode(Pins::LED, OUTPUT);
  digitalWrite(Pins::LED, LOW);
  IrReceiver.begin(Pins::IR_RECEIVE, ENABLE_LED_FEEDBACK);
  motorsSetup();
  initIrHandler();
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
        lastIrActivity = millis();
        return true;
      }
    }
    delay(10);
  }

  return false;
}

void handleWakeup() {
  auto reason = esp_sleep_get_wakeup_cause();

  if (reason == ESP_SLEEP_WAKEUP_EXT0) {
    if (!confirmWakeupWithIR()) {
      Serial.println("⏳ No confirmation – going back to sleep.");
      digitalWrite(Pins::LED, LOW);
      goToSleep();
    } else {
      Serial.println("⏳ Waiting for command...");
    }
  } else {
    Serial.println("😴 Cold boot – going to sleep...");
    digitalWrite(Pins::LED, LOW);
    goToSleep();
  }
}

// ---------------- SETUP ---------------------

void setup() {
  Serial.begin(115200);
  delay(100);

  setupWiFiAndOTA();
  initHardware();
  handleWakeup();
}

// ---------------- LOOP ---------------------

void loop() {
  handleOTA();

  if (IrReceiver.decode()) {
    handleIrCommand(IrReceiver.decodedIRData.command);
    IrReceiver.resume();
  }

  if (millis() - lastIrActivity > Timeouts::INACTIVITY_TIMEOUT) {
    Serial.println("⏲️ No activity – entering sleep mode automatically.");
    goToSleep();
  }
}