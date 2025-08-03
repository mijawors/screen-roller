#pragma once

struct StepperMotor {
  uint8_t in1, in2, in3, in4;

  void setup();
  void step1();
  void step2();
  void step3();
  void step4();
  void stepForward();
  void stepBackward();
  void rotate(int steps = 512);
  void rotateBackward(int steps = 512);
  void release();
};

extern StepperMotor motor1;
extern StepperMotor motor2;

void motorsSetup();