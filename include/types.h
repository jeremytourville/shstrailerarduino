#pragma once

#include <Arduino.h>

namespace shstrailer {

enum class WinchDirection : uint8_t { STOP = 0, UP, DOWN };

enum class WinchState : uint8_t {
    IDLE = 0,
    RUNNING_UP,
    RUNNING_DOWN,
    DIRECTION_DELAY,
    FAULT
};

enum class SystemState : uint8_t { STARTUP = 0, READY, LOW_BATTERY, FAULT };

struct Battery {
    float voltage;
    bool warning;
    bool critical;
};

struct ControllerStatus {
    SystemState systemState;
    Battery battery;
    bool winchFault;
    bool winchCooldown;
};

}  // namespace shstrailer
