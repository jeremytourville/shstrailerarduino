#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "types.h"

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

    uint32_t m_ledTimer;
    uint32_t m_batteryTimer;

    bool m_ledState;

    void readBattery();
    void updateSystemState();
    void updateStatusLED();
};

#endif
