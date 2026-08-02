#pragma once

#include <Arduino.h>

#include "config.h"
#include "pins.h"
#include "types.h"

namespace shstrailer {

class SafetyController {
   public:
    SafetyController();

    void begin();

    void update();

    void safeStartup();

   private:
    ControllerStatus m_status;
    uint32_t m_ledTimer;
    bool m_ledState;
    void readBattery();
    void updateStatusLED();
};

}  // namespace shstrailer
