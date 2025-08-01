#include <Arduino.h>

#include <motors.h>
#include <config.h>

void motorsSetup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
}

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

// ------ SECOND MOTOR (RAIL) ------

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