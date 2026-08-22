#pragma once

#include <stdint.h>

namespace shstrailer {

enum class WinchDirection : uint8_t { STOP = 0, UP, DOWN };

enum class WinchState : uint8_t {
    IDLE = 0,
    RUNNING_UP,
    RUNNING_DOWN,
    COOLING_DOWN
};

enum class BatteryState : uint8_t { OK = 0, WARNING, CRITICAL };

}  // namespace shstrailer
