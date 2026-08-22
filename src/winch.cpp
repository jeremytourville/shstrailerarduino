#include "winch.h"

#include "abort.h"
#include "algorithm.h"
#include "console.h"
#include "pins.h"

namespace shstrailer {

namespace {

// Manufacturer duty-cycle limit:
// Maximum continuous run: 45 seconds.
// 5% duty cycle means 19 seconds OFF for every 1 second ON.
// Therefore, a full 45-second run requires 855 seconds (14 min 15 sec) OFF.
constexpr Timer::Duration kWinchMaxContinuousRuntimeMS = 45000UL;
constexpr uint8_t kWinchDutyPercent = 5;
constexpr uint8_t kWinchOffToOnRatio =
    (100U - kWinchDutyPercent) / kWinchDutyPercent;

}  // namespace

void Winch::begin() {
    pinMode(WINCH_UP_OUT, OUTPUT);
    pinMode(WINCH_DN_OUT, OUTPUT);
    digitalWrite(WINCH_UP_OUT, LOW);
    digitalWrite(WINCH_DN_OUT, LOW);
    timer_.start();
}

void Winch::update() {
    // Allowed state transitions:
    //
    // IDLE -> RUNNING_UP/RUNNING_DOWN
    // RUNNING_UP/RUNNING_DOWN -> COOLING_DOWN
    // COOLING_DOWN -> IDLE
    //

    const auto elapsed = timer_.elapsed();

    // cout << F("elapsed: ") << elapsed << F(" requested: ") << int(requested_)
    //    << endl;

    switch (state_) {
        case WinchState::IDLE:
            if (WinchDirection::UP == requested_) {
                setState(WinchState::RUNNING_UP, elapsed);
            } else if (WinchDirection::DOWN == requested_) {
                setState(WinchState::RUNNING_DOWN, elapsed);
            }

            break;

        case WinchState::RUNNING_UP:
        case WinchState::RUNNING_DOWN:
            // intentional fall through

            if (elapsed >= kWinchMaxContinuousRuntimeMS ||
                WinchDirection::STOP == requested_) {
                computeRequiredCooldownTime(elapsed);
                setState(WinchState::COOLING_DOWN, elapsed);
            }

            break;

        case WinchState::COOLING_DOWN:
            notify(elapsed);

            if (0 == cooldownTimeRemaining(elapsed)) {
                setState(WinchState::IDLE, elapsed);
            }

            break;
    }
}

void Winch::computeRequiredCooldownTime(const Timer::Duration runTime) {
    // Prevent someone from cycling the rocker switch back and forth quickly by
    // requiring at least one second of runtime.
    requiredCooldownMs_ =
        maximum(kWinchOffToOnRatio * runTime, kWinchOffToOnRatio * 1000UL);
}

Timer::Duration Winch::cooldownTimeRemaining(
    const Timer::Duration elapsed) const {
    if (WinchState::COOLING_DOWN != state_ || elapsed >= requiredCooldownMs_) {
        return 0;
    }

    return requiredCooldownMs_ - elapsed;
}

void Winch::onButtonDown(const uint8_t pin) {
    // Winch is hold-to-run. Both buttons cannot be pressed because it uses a
    // momentary rocker.
    switch (pin) {
        case WINCH_UP_SW:
            requested_ = WinchDirection::UP;
            break;
        case WINCH_DN_SW:
            requested_ = WinchDirection::DOWN;
            break;
    }
}

void Winch::onButtonReleased([[maybe_unused]] const uint8_t pin) {
    // Winch is subscribed only to WINCH_UP_SW and WINCH_DN_SW, no other
    // button will cause this to be called.

    requested_ = WinchDirection::STOP;
}

void Winch::setState(const WinchState state, const Timer::Duration elapsed) {
    if (state == state_) {
        return;
    }

    if (WinchState::RUNNING_UP == state) {
        digitalWrite(WINCH_UP_OUT, HIGH);
        digitalWrite(WINCH_DN_OUT, LOW);
    } else if (WinchState::RUNNING_DOWN == state) {
        digitalWrite(WINCH_UP_OUT, LOW);
        digitalWrite(WINCH_DN_OUT, HIGH);
    } else {
        // This would be IDLE or COOLING_DOWN
        digitalWrite(WINCH_UP_OUT, LOW);
        digitalWrite(WINCH_DN_OUT, LOW);
        requested_ = WinchDirection::STOP;
    }

    // Always restart the timer even though for IDLE it isnt used.
    timer_.start();

    state_ = state;

    notify(elapsed);
}

void Winch::registerObserver(WinchObserver* observer) {
    if (nullptr == observer) {
        Abort(F("winch observer nullptr"));
    }

    observers_.push_back(observer);

    observer->onWinchState(state_, cooldownTimeRemaining(timer_.elapsed()));
}

void Winch::notify(const Timer::Duration elapsed) {
    const auto cooldownTimeRemainingLocal = cooldownTimeRemaining(elapsed);

    for (auto* observer : observers_) {
        observer->onWinchState(state_, cooldownTimeRemainingLocal);
    }
}

}  // namespace shstrailer
