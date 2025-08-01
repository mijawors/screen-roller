#include <Arduino.h>

#include <motors.h>
#include <config.h>

using namespace Config::V1;

void motorsSetup() {
  pinMode(MotorPins::IN1, OUTPUT);
  pinMode(MotorPins::IN2, OUTPUT);
  pinMode(MotorPins::IN3, OUTPUT);
  pinMode(MotorPins::IN4, OUTPUT);

  pinMode(MotorPins::IN5, OUTPUT);
  pinMode(MotorPins::IN6, OUTPUT);
  pinMode(MotorPins::IN7, OUTPUT);
  pinMode(MotorPins::IN8, OUTPUT);
}

// ------ MAIN MOTOR ------

void step1() {
  digitalWrite(MotorPins::IN1, HIGH);
  digitalWrite(MotorPins::IN2, LOW);
  digitalWrite(MotorPins::IN3, LOW);
  digitalWrite(MotorPins::IN4, LOW);
  delay(2);
}

void step2() {
  digitalWrite(MotorPins::IN1, LOW);
  digitalWrite(MotorPins::IN2, HIGH);
  digitalWrite(MotorPins::IN3, LOW);
  digitalWrite(MotorPins::IN4, LOW);
  delay(2);
}

void step3() {
  digitalWrite(MotorPins::IN1, LOW);
  digitalWrite(MotorPins::IN2, LOW);
  digitalWrite(MotorPins::IN3, HIGH);
  digitalWrite(MotorPins::IN4, LOW);
  delay(2);
}

void step4() {
  digitalWrite(MotorPins::IN1, LOW);
  digitalWrite(MotorPins::IN2, LOW);
  digitalWrite(MotorPins::IN3, LOW);
  digitalWrite(MotorPins::IN4, HIGH);
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
  digitalWrite(MotorPins::IN1, LOW);
  digitalWrite(MotorPins::IN2, LOW);
  digitalWrite(MotorPins::IN3, LOW);
  digitalWrite(MotorPins::IN4, LOW);
}

// ------ SECOND MOTOR (RAIL) ------

void stepA() {
  digitalWrite(MotorPins::IN5, HIGH);
  digitalWrite(MotorPins::IN6, LOW);
  digitalWrite(MotorPins::IN7, LOW);
  digitalWrite(MotorPins::IN8, LOW);
  delay(2);
}

void stepB() {
  digitalWrite(MotorPins::IN5, LOW);
  digitalWrite(MotorPins::IN6, HIGH);
  digitalWrite(MotorPins::IN7, LOW);
  digitalWrite(MotorPins::IN8, LOW);
  delay(2);
}

void stepC() {
  digitalWrite(MotorPins::IN5, LOW);
  digitalWrite(MotorPins::IN6, LOW);
  digitalWrite(MotorPins::IN7, HIGH);
  digitalWrite(MotorPins::IN8, LOW);
  delay(2);
}

void stepD() {
  digitalWrite(MotorPins::IN5, LOW);
  digitalWrite(MotorPins::IN6, LOW);
  digitalWrite(MotorPins::IN7, LOW);
  digitalWrite(MotorPins::IN8, HIGH);
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
  digitalWrite(MotorPins::IN5, LOW);
  digitalWrite(MotorPins::IN6, LOW);
  digitalWrite(MotorPins::IN7, LOW);
  digitalWrite(MotorPins::IN8, LOW);
}