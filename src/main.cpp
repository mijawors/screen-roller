#include <Arduino.h>
#include <IRremote.h>
#include <Preferences.h>
#include <esp_sleep.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

// mian motor
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

// rail motor
#define IN5 12
#define IN6 14
#define IN7 27
#define IN8 26

#define IR_RECEIVE_PIN 13

#define LED_PIN 25

Preferences prefs;
bool isExtended = false;

unsigned long lastIrActivity = 0;
const unsigned long INACTIVITY_TIMEOUT = 60000;

// ---------------- SLEEP MODE ---------------------

void goToSleep() {
  delay(100);
  IrReceiver.end(); // zatrzymaj IR
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0); // LOW na GPIO13 wybudza
  Serial.println("Usypiam... naciśnij przycisk na pilocie (np. *)");
  delay(100);  // ważne, by zdążył wysłać tekst przez UART
  digitalWrite(LED_PIN, LOW);
  esp_deep_sleep_start();
}

// ---------------- SETUP -------------------

void setup() {
  Serial.begin(115200);
  delay(100);  // ważne – daj czas na USB/UART start

  WiFi.begin("ssid", "password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono z WiFi!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.begin();
  Serial.println("✅ OTA gotowe. Czekam na kod...");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IN5, OUTPUT); 
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT); 
  pinMode(IN8, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  prefs.begin("motorState", true);
  isExtended = prefs.getBool("extended", false);
  prefs.end();

  esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();

  if (wakeupReason == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("📡 Wybudzenie przez IR – 15 sekund na potwierdzenie `*`...");
    Serial.println(WiFi.localIP());
    delay(300);  // pozwól sygnałowi opaść
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    unsigned long startTime = millis();
    bool confirmed = false;

    while (millis() - startTime < 15000) { // 15 sekund na potwierdzenie
      digitalWrite(LED_PIN, millis() % 500 < 250 ? HIGH : LOW);
      if (IrReceiver.decode()) {
        unsigned long code = IrReceiver.decodedIRData.command;
        Serial.print("➡️ IR kod: 0x");
        Serial.println(code, HEX);

        IrReceiver.resume();

        if (code == 0x16) {  // * przycisk
          Serial.println("✅ Potwierdzono przyciskiem `*`. Uruchamiam.");
          confirmed = true;
          digitalWrite(LED_PIN, HIGH);
          lastIrActivity = millis();
          break;
        }
      }

      delay(10); // małe opóźnienie, żeby nie zjadać CPU
    }

    if (!confirmed) {
      Serial.println("⏳ Brak potwierdzenia – wracam spać.");
      digitalWrite(LED_PIN, LOW);
      goToSleep();
    } else {
      Serial.println("⏳ Oczekiwanie na polecenie...");
    }

  } else {
    Serial.println("😴 Uruchomienie zimne – usypiam...");
    digitalWrite(LED_PIN, LOW);
    goToSleep();
  }
}

// ----------------- MOTOR 1 ------------------

void step1() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(2);
}

void step2() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(2);
}

void step3() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2);
}

void step4() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(2);
}

void makeOneStep() {
  step1(); step2(); step3(); step4();
}

void makeOneStepBackward() {
  step4(); step3(); step2(); step1();
}

void makeOneRotation() {
  for (int i = 0; i < 512; i++) {
    makeOneStep();
  }
}

void makeOneRotationBackward() {
  for (int i = 0; i < 512; i++) {
    makeOneStepBackward();
  }
}

void releaseMotor() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}

// ----------------- MOTOR 2 ------------------

void stepA() {
  digitalWrite(IN5, HIGH); 
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  
  digitalWrite(IN8, LOW);
  delay(2);
}

void stepB() {
  digitalWrite(IN5, LOW); 
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); 
  digitalWrite(IN8, LOW);
  delay(2);
}

void stepC() {
  digitalWrite(IN5, LOW); 
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); 
  digitalWrite(IN8, LOW);
  delay(2);
}

void stepD() {
  digitalWrite(IN5, LOW); 
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); 
  digitalWrite(IN8, HIGH);
  delay(2);
}

void makeSecondMotorRotation() {
  for (int i = 0; i < 512; i++) {
    stepA(); stepB(); stepC(); stepD();
  }
}

void makeSecondMotorRotationBackward() {
  for (int i = 0; i < 512; i++) {
    stepD(); stepC(); stepB(); stepA();
  }
}

void releaseMotor2() {
  digitalWrite(IN5, LOW); 
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); 
  digitalWrite(IN8, LOW);
}

// ---------------- LOOP ---------------------

void loop() {
  ArduinoOTA.handle();
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.command;
    Serial.print("Odebrano IR kod: 0x");
    Serial.println(code, HEX);
    lastIrActivity = millis();

    if (code == 0x16) { // Przycisk '*'
      Serial.println("😴 Usypiam urządzenie – naciśnij * aby wybudzić");
      goToSleep();
    }

    else if (code == 0x45) { // Przycisk '1'
      if (!isExtended) {
        Serial.println("⚙️ Rozwijanie...");

        makeSecondMotorRotation();      // silnik 2 – obrót w przód
        releaseMotor2();

        delay(2000);

        for (int i = 0; i < 2; i++) {
          makeOneRotation();           // główny silnik – rozwijanie
        }
        releaseMotor();

        delay(2000);

        makeSecondMotorRotationBackward(); // silnik 2 – powrót
        releaseMotor2();

        isExtended = true;
        lastIrActivity = millis();
        Serial.println("✅ Rozwinięte. Zapisuję stan...");
      } else {
        Serial.println("⚙️ Zwijanie...");

        makeSecondMotorRotationBackward(); // silnik 2 – obrót w tył
        releaseMotor2();

        delay(2000);

        for (int i = 0; i < 2; i++) {
          makeOneRotationBackward();   // główny silnik – zwijanie
        }
        releaseMotor();

        delay(2000);

        makeSecondMotorRotation();     // silnik 2 – powrót
        releaseMotor2();

        isExtended = false;
        lastIrActivity = millis();
        Serial.println("✅ Zwinięte. Zapisuję stan...");
      }

      // Zapisz nowy stan
      prefs.begin("motorState", false);
      prefs.putBool("extended", isExtended);
      prefs.end();
    }

    else if (code == 0x18) { // ▼ – awaryjne zwijanie
      Serial.println("⛔ Awaryjne zwijanie...");
      makeOneStepBackward();
      releaseMotor();
    }

    else if (code == 0x52) { // ▲ – awaryjne rozwijanie
      Serial.println("⛔ Awaryjne rozwijanie...");
      makeOneStep();
      releaseMotor();
    }

    IrReceiver.resume();
    Serial.println("⏳ Oczekiwanie na kolejne polecenie...");
  }

  // Jeśli brak aktywności IR przez X sekund – uśpij
  if (millis() - lastIrActivity > INACTIVITY_TIMEOUT) {
    Serial.println("⏲️ Brak aktywności – usypiam automatycznie.");
    goToSleep();
  }
}