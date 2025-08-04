#include <Arduino.h>
#include <esp_sleep.h>
#include <config.h>
#include <ir_handler.h>
#include <power_handler.h>

using namespace Config::V1;

namespace {
  unsigned long lastIrActivity = 0;
}

void updateActivity() {
  lastIrActivity = millis();
}

void goToSleep() {
  delay(100);
  prepareIrForSleep();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); // IR pin LOW
  Serial.println("😴 Going to sleep... press a button on the remote (e.g., *)");
  delay(100);
  digitalWrite(Pins::LED, LOW);
  esp_deep_sleep_start();
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

void initPowerHandler() {
  pinMode(Pins::LED, OUTPUT);
  digitalWrite(Pins::LED, LOW);
}

void handleInactivitySleep() {
  if (millis() - lastIrActivity > Timeouts::INACTIVITY_TIMEOUT) {
    Serial.println("⏲️ No activity – entering sleep mode automatically.");
    goToSleep();
  }
}