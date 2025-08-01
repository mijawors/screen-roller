#include <Arduino.h>
#include <IRremote.h>
#include <Preferences.h>
#include <esp_sleep.h>

#include <motors.h>
#include <ir_handler.h>
#include <ota_handler.h>
#include <config.h>

Preferences prefs;
bool isExtended = false;

unsigned long lastIrActivity = 0;

// ---------------- SLEEP MODE ---------------------

void goToSleep() {
  delay(100);
  IrReceiver.end(); // stop IR
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); // LOW on GPIO13 will wake up
  Serial.println("Going to sleep... press a button on the remote (e.g., *)");
  delay(100);  // important to allow UART to send the message
  digitalWrite(LED_PIN, LOW);
  esp_deep_sleep_start();
}

// ---------------- SETUP -------------------

void setup() {
  Serial.begin(115200);
  delay(100);  // important – give USB/UART time to initialize

  setupWiFiAndOTA();

  motorsSetup();
  initIrHandler();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  prefs.begin("motorState", true);
  isExtended = prefs.getBool("extended", false);
  prefs.end();

  esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();

  if (wakeupReason == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("📡 Woken up by IR – 15 seconds to confirm with `*`...");
    delay(300);  // allow signal to settle
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    unsigned long startTime = millis();
    bool confirmed = false;

    while (millis() - startTime < 15000) { // 15 seconds to confirm
      digitalWrite(LED_PIN, millis() % 500 < 250 ? HIGH : LOW);
      if (IrReceiver.decode()) {
        unsigned long code = IrReceiver.decodedIRData.command;
        Serial.print("➡️ IR code: 0x");
        Serial.println(code, HEX);

        IrReceiver.resume();

        if (code == 0x16) {  // * button
          Serial.println("✅ Confirmed with `*` button. Starting...");
          confirmed = true;
          digitalWrite(LED_PIN, HIGH);
          lastIrActivity = millis();
          break;
        }
      }

      delay(10); // small delay to prevent CPU overuse
    }

    if (!confirmed) {
      Serial.println("⏳ No confirmation – going back to sleep.");
      digitalWrite(LED_PIN, LOW);
      goToSleep();
    } else {
      Serial.println("⏳ Waiting for command...");
    }

  } else {
    Serial.println("😴 Cold boot – going to sleep...");
    digitalWrite(LED_PIN, LOW);
    goToSleep();
  }
}

// ---------------- LOOP ---------------------

void loop() {
  handleOTA();

  if (IrReceiver.decode()) {
    handleIrCommand(IrReceiver.decodedIRData.command);
    IrReceiver.resume();
  }

  // If no IR activity for X milliseconds – go to sleep
  if (millis() - lastIrActivity > INACTIVITY_TIMEOUT) {
    Serial.println("⏲️ No activity – entering sleep mode automatically.");
    goToSleep();
  }
}