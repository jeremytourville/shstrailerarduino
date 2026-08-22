#pragma once

#include "observers/button_observer.h"
#include "observers/winch_observer.h"
#include "timer.h"
#include "types.h"
#include "vector.h"

namespace shstrailer {

class Winch : public ButtonObserver {
   public:
    void begin();

    void update();

    void onButtonDown(uint8_t pin) override;

    void onButtonReleased(uint8_t pin) override;

    void registerObserver(WinchObserver* observer);

   private:
    void setState(WinchState state, Timer::Duration elapsed);

    void notify(Timer::Duration elapsed);

    void computeRequiredCooldownTime(Timer::Duration elapsed);

    [[nodiscard]] Timer::Duration cooldownTimeRemaining(
        Timer::Duration elapsed) const;

    WinchState state_ = WinchState::IDLE;
    WinchDirection requested_ = WinchDirection::STOP;
    Timer timer_;
    Timer::Duration requiredCooldownMs_ = 0;
    Vector<WinchObserver*, 2> observers_;
};

}  // namespace shstrailer
