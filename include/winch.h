#pragma once

#include <Arduino.h>

#include "button_observer.hpp"
#include "config.h"
#include "pins.h"
#include "types.h"

namespace shstrailer {

class WinchController : public ButtonObserver {
   public:
    WinchController();
    void begin();
    void update();
    void commandUp();
    void commandDown();
    void stop();
    [[nodiscard]] bool isFaulted() const;
    [[nodiscard]] bool isCoolingDown() const;
    [[nodiscard]] WinchState state() const;
    [[nodiscard]] uint32_t cooldownRemainingMs() const;

    void onContinuousPress(uint8_t pin) override;

    void onReleased(uint8_t pin) override;

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
