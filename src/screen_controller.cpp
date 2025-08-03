#include <Arduino.h>
#include <Preferences.h>
#include <motors.h>
#include <screen_controller.h>

namespace {
  Preferences prefs;
  bool extended = false;
}

void initScreenController() {
  prefs.begin("motorState", true);
  extended = prefs.getBool("extended", false);
  prefs.end();
}

bool isScreenExtended() {
  return extended;
}

void extendScreen() {
  Serial.println("⚙️ Extending...");

  motor2.rotate();
  motor2.release();

  delay(2000);
  for (int i = 0; i < 2; i++) motor1.rotate();
  motor1.release();

  delay(2000);
  motor2.rotateBackward();
  motor2.release();

  extended = true;
  prefs.begin("motorState", false);
  prefs.putBool("extended", true);
  prefs.end();

  Serial.println("✅ Extended.");
}

void retractScreen() {
  Serial.println("⚙️ Retracting...");

  motor2.rotateBackward();
  motor2.release();

  delay(2000);
  for (int i = 0; i < 2; i++) motor1.rotateBackward();
  motor1.release();

  delay(2000);
  motor2.rotate();
  motor2.release();

  extended = false;
  prefs.begin("motorState", false);
  prefs.putBool("extended", false);
  prefs.end();

  Serial.println("✅ Retracted.");
}

void toggleScreen() {
  if (isScreenExtended()) {
    retractScreen();
  } else {
    extendScreen();
  }
}

void emergencyExtend() {
  Serial.println("⛔ Emergency extending...");
  motor1.stepForward();
  motor1.release();
}

void emergencyRetract() {
  Serial.println("⛔ Emergency retracting...");
  motor1.stepBackward();
  motor1.release();
}