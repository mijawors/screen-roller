#pragma once

#include <secrets.h>

namespace Config {
  inline namespace V1 {

    struct MotorPins {
      static constexpr int IN1 = 16;
      static constexpr int IN2 = 17;
      static constexpr int IN3 = 18;
      static constexpr int IN4 = 19;

      static constexpr int IN5 = 12;
      static constexpr int IN6 = 14;
      static constexpr int IN7 = 27;
      static constexpr int IN8 = 26;
    };

    struct Pins {
      static constexpr int IR_RECEIVE = 13;
      static constexpr int LED = 25;
    };

    struct Timeouts {
      static constexpr unsigned long INACTIVITY_TIMEOUT = 60000;
    };

  }
}