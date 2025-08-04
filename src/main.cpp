#include <Arduino.h>
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
  prepareIrForSleep();  // <-- z ir_handler.h
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  Serial.println("Going to sleep... press a button on the remote (e.g., *)");
  delay(100);
  digitalWrite(Pins::LED, LOW);
  esp_deep_sleep_start();
}

void initHardware() {
  pinMode(Pins::LED, OUTPUT);
  digitalWrite(Pins::LED, LOW);
  motorsSetup();
  irHandlerInit();  // zamiast IrReceiver.begin()
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
  irHandlerLoop();  // <-- nowy
  if (millis() - lastIrActivity > Timeouts::INACTIVITY_TIMEOUT) {
    Serial.println("⏲️ No activity – entering sleep mode automatically.");
    goToSleep();
  }
}