#include <Arduino.h>
#include <motors.h>
#include <config.h>

using namespace Config::V1;

StepperMotor motor1 = {MotorPins::IN1, MotorPins::IN2, MotorPins::IN3, MotorPins::IN4};
StepperMotor motor2 = {MotorPins::IN5, MotorPins::IN6, MotorPins::IN7, MotorPins::IN8};

void motorsSetup() {
  motor1.setup();
  motor2.setup();
}

void StepperMotor::setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void StepperMotor::step1() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2);
}

void StepperMotor::step2() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2);
}

void StepperMotor::step3() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2);
}

void StepperMotor::step4() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2);
}

void StepperMotor::stepForward() {
  step1(); step2(); step3(); step4();
}

void StepperMotor::stepBackward() {
  step4(); step3(); step2(); step1();
}

void StepperMotor::rotate(int steps) {
  for (int i = 0; i < steps; i++) {
    stepForward();
  }
}

void StepperMotor::rotateBackward(int steps) {
  for (int i = 0; i < steps; i++) {
    stepBackward();
  }
}

void StepperMotor::release() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}