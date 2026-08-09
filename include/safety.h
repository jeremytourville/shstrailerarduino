#pragma once

#include <Arduino.h>

#include "config.h"
#include "pins.h"
#include "timer.h"
#include "types.h"

namespace shstrailer {

class SafetyController {
   public:
    SafetyController();

    void begin();
    void update();
    void safeStartup();

    void setWinchFault(bool faulted);
    void setWinchCooldown(bool coolingDown);

    const ControllerStatus& status() const;

   private:
    ControllerStatus m_status;

    Timer m_ledTimer;
    Timer m_batteryTimer;

    bool m_ledState;

    void readBattery();
    void updateSystemState();
    void updateStatusLED();
};

}  // namespace shstrailer
