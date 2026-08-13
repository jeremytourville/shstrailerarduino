#pragma once

#include "config.h"
#include "observers/button_observer.h"
#include "observers/winch_observer.h"
#include "pins.h"
#include "types.h"
#include "vector.h"

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

    void onButtonContinuousPress(uint8_t pin) override;

    void onButtonReleased(uint8_t pin) override;

    void registerObserver(WinchObserver* observer);

   private:
    void setOutputs(bool up, bool down);

    void beginRun(WinchDirection direction, uint32_t now);

    void endRunAndStartCooldown(uint32_t now);

    void enterFault(uint32_t now);

    void updateCooldown(uint32_t now);

    void setState(WinchState state);

    void notify();

    WinchState state_;
    WinchDirection requested_;
    uint32_t stateTimer_;
    uint32_t runStartTime_;
    uint32_t cooldownStartTime_;
    uint32_t requiredCooldownMs_;
    bool coolingDown_;
    Vector<WinchObserver*, 2> observers_;
};

}  // namespace shstrailer
