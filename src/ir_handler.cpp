#include <Arduino.h>
#include <Preferences.h>
#include <esp_sleep.h>

#include <motors.h>
#include <ir_handler.h>

namespace {
  Preferences prefs;
  bool isExtended = false;
}

extern unsigned long lastIrActivity;

void initIrHandler() {
  prefs.begin("motorState", true);
  isExtended = prefs.getBool("extended", false);
  prefs.end();
}

bool getIsExtended() {
  return isExtended;
}

void setIsExtended(bool state) {
  isExtended = state;
}

void goToSleep();

void extendArm() {
  Serial.println("⚙️ Extending...");
  makeSecondMotorRotation();
  releaseMotor2();

  delay(2000);
  for (int i = 0; i < 2; i++) makeOneRotation();
  releaseMotor();

  delay(2000);
  makeSecondMotorRotationBackward();
  releaseMotor2();

  isExtended = true;
  Serial.println("✅ Extended. Saving state...");

  prefs.begin("motorState", false);
  prefs.putBool("extended", true);
  prefs.end();
}

void retractArm() {
  Serial.println("⚙️ Retracting...");
  makeSecondMotorRotationBackward();
  releaseMotor2();

  delay(2000);
  for (int i = 0; i < 2; i++) makeOneRotationBackward();
  releaseMotor();

  delay(2000);
  makeSecondMotorRotation();
  releaseMotor2();

  isExtended = false;
  Serial.println("✅ Retracted. Saving state...");

  prefs.begin("motorState", false);
  prefs.putBool("extended", false);
  prefs.end();
}

void handleIrCommand(uint8_t code) {
  lastIrActivity = millis();

  switch (code) {
    case 0x16:  // '*'
      Serial.println("😴 Going to sleep – press * to wake up");
      goToSleep();
      break;

    case 0x45:  // '1'
      if (!isExtended) {
        extendArm();
      } else {
        retractArm();
      }
      break;

    case 0x18:  // ▼
      Serial.println("⛔ Emergency retracting...");
      makeOneStepBackward();
      releaseMotor();
      break;

    case 0x52:  // ▲
      Serial.println("⛔ Emergency extending...");
      makeOneStep();
      releaseMotor();
      break;

    default:
      Serial.print("⚠️ Unknown code: 0x");
      Serial.println(code, HEX);
      break;
  }

  Serial.println("⏳ Waiting for the next command...");
}