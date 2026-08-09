#pragma once

#include <Arduino.h>

#include "config.h"
#include "pins.h"
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
    bool isFaulted() const;
    bool isCoolingDown() const;
    WinchState state() const;
    uint32_t cooldownRemainingMs() const;

   private:
    WinchState m_state;
    WinchDirection m_requested;
    uint32_t m_stateTimer;
    uint32_t m_runStartTime;
    uint32_t m_cooldownStartTime;
    uint32_t m_requiredCooldownMs;
    bool m_coolingDown;

    void setOutputs(bool up, bool down);
    void beginRun(WinchDirection direction, uint32_t now);
    void endRunAndStartCooldown(uint32_t now);
    void enterFault(uint32_t now);
    void updateCooldown(uint32_t now);
};

}  // namespace shstrailer
