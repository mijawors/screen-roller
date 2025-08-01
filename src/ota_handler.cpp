#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>

#include <ota_handler.h>
#include <config.h>

using namespace Secrets;

void setupWiFiAndOTA() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.begin();
  Serial.println("✅ OTA ready. Waiting for upload...");
}

void handleOTA() {
  ArduinoOTA.handle();
}