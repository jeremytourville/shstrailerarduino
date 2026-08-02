#pragma once

#include <Arduino.h>

#include "config.h"
#include "pins.h"
#include "safe_timer.h"
#include "types.h"

namespace shstrailer {

class WinchController {
   public:
    WinchController();

    void begin();

    void update();

    void commandUp();

    void commandDown();

    void stop();

   private:
    WinchState m_state;
    WinchDirection m_requested;
    SafeTimer m_timer;

    void setOutputs(bool up, bool down);
};

}  // namespace shstrailer
