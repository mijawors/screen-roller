#include <Arduino.h>
#include <motors.h>
#include <ir_handler.h>
#include <ota_handler.h>
#include <config.h>

void setup() {
  Serial.begin(115200);
  delay(100);

  setupWiFiAndOTA();
  initPowerHandler();
  motorsSetup();
  irHandlerInit();
  handleWakeup();
}

void loop() {
  handleOTA();
  irHandlerLoop();
  handleInactivitySleep();
}