#pragma once

#include "config.h"
#include "observers/button_observer.h"
#include "observers/winch_observer.h"
#include "pins.h"
#include "timer.h"
#include "types.h"
#include "vector.h"

namespace shstrailer {

class WinchController : public ButtonObserver {
   public:
    void begin();

    void update();

    void commandUp();

    void commandDown();

    void stop();

    [[nodiscard]] Timer::Duration cooldownRemainingMs() const;

    void onButtonContinuousPress(uint8_t pin) override;

    void onButtonReleased(uint8_t pin) override;

    void registerObserver(WinchObserver* observer);

   private:
    void setOutputs(bool up, bool down);

    void beginRun(WinchDirection direction, const Timer& now);

    void endRunAndStartCooldown(const Timer& now);

    void enterFault(const Timer& now);

    void updateCooldown(const Timer& now);

    void setState(WinchState state);

    void notify();

    WinchState state_ = WinchState::IDLE;
    WinchDirection requested_ = WinchDirection::STOP;
    Timer stateTimer_;
    Timer runStartTime_;
    Timer cooldownStartTime_;
    Timer::Duration requiredCooldownMs_;
    bool coolingDown_ = false;
    Vector<WinchObserver*, 2> observers_;
};

}  // namespace shstrailer
