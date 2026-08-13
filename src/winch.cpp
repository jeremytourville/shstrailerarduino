#include "winch.h"

#include "abort.h"

namespace shstrailer {

namespace {

// Winch protection
constexpr Timer::Duration WINCH_DIRECTION_DELAY_MS = 250;

// Manufacturer duty-cycle limit:
// Maximum continuous run: 45 seconds.
// 5% duty cycle means 19 seconds OFF for every 1 second ON.
// Therefore, a full 45-second run requires 855 seconds (14 min 15 sec) OFF.
constexpr Timer::Duration WINCH_MAX_CONTINUOUS_RUNTIME_MS = 45000UL;
constexpr uint8_t WINCH_DUTY_PERCENT = 5;
constexpr uint8_t WINCH_OFF_TO_ON_RATIO =
    (100U - WINCH_DUTY_PERCENT) / WINCH_DUTY_PERCENT;

}  // namespace

void WinchController::begin() {
    pinMode(WINCH_UP_OUT, OUTPUT);
    pinMode(WINCH_DN_OUT, OUTPUT);
    setOutputs(false, false);

    requested_ = WinchDirection::STOP;
    const Timer now;
    stateTimer_ = now;
    runStartTime_ = now;
    cooldownStartTime_ = now;
    requiredCooldownMs_ = 0;
    coolingDown_ = false;

    setState(WinchState::IDLE);
}

void WinchController::commandUp() { requested_ = WinchDirection::UP; }

void WinchController::commandDown() { requested_ = WinchDirection::DOWN; }

void WinchController::stop() { requested_ = WinchDirection::STOP; }

void WinchController::setOutputs(bool up, bool down) {
    if (up && down) {
        up = false;
        down = false;
    }

    digitalWrite(WINCH_UP_OUT, up ? OUTPUT_ON : OUTPUT_OFF);
    digitalWrite(WINCH_DN_OUT, down ? OUTPUT_ON : OUTPUT_OFF);
}

void WinchController::beginRun(const WinchDirection direction,
                               const Timer& now) {
    if (coolingDown_ || direction == WinchDirection::STOP) {
        setOutputs(false, false);
        return;
    }

    runStartTime_ = now;

    if (direction == WinchDirection::UP) {
        setOutputs(true, false);
        setState(WinchState::RUNNING_UP);
    } else {
        setOutputs(false, true);
        setState(WinchState::RUNNING_DOWN);
    }
}

void WinchController::endRunAndStartCooldown(const Timer& now) {
    setOutputs(false, false);

    const uint32_t runTimeMs = now - runStartTime_;

    if (runTimeMs > (UINT32_MAX / WINCH_OFF_TO_ON_RATIO)) {
        requiredCooldownMs_ = UINT32_MAX;
    } else {
        requiredCooldownMs_ = runTimeMs * WINCH_OFF_TO_ON_RATIO;
    }

    cooldownStartTime_ = now;
    coolingDown_ = (requiredCooldownMs_ > 0);
}

void WinchController::enterFault(const Timer& now) {
    endRunAndStartCooldown(now);
    setState(WinchState::FAULT);
}

void WinchController::updateCooldown(const Timer& now) {
    if (!coolingDown_) {
        return;
    }

    if (now - cooldownStartTime_ >= requiredCooldownMs_) {
        coolingDown_ = false;
        requiredCooldownMs_ = 0;
    }

    notify();
}

void WinchController::update() {
    const Timer now;

    updateCooldown(now);

    switch (state_) {
        case WinchState::IDLE:
            setOutputs(false, false);
            if (!coolingDown_) {
                if (requested_ == WinchDirection::UP)
                    beginRun(WinchDirection::UP, now);
                else if (requested_ == WinchDirection::DOWN)
                    beginRun(WinchDirection::DOWN, now);
            }

            break;

        case WinchState::RUNNING_UP:
            if (requested_ == WinchDirection::STOP) {
                endRunAndStartCooldown(now);
                setState(WinchState::IDLE);
            } else if (requested_ == WinchDirection::DOWN) {
                endRunAndStartCooldown(now);
                setState(WinchState::DIRECTION_DELAY);
                stateTimer_ = now;
            } else if ((now - runStartTime_) >=
                       WINCH_MAX_CONTINUOUS_RUNTIME_MS) {
                enterFault(now);
            }

            break;

        case WinchState::RUNNING_DOWN:
            if (requested_ == WinchDirection::STOP) {
                endRunAndStartCooldown(now);
                setState(WinchState::IDLE);
            } else if (requested_ == WinchDirection::UP) {
                endRunAndStartCooldown(now);
                setState(WinchState::DIRECTION_DELAY);
                stateTimer_ = now;
            } else if ((now - runStartTime_) >=
                       WINCH_MAX_CONTINUOUS_RUNTIME_MS) {
                enterFault(now);
            }

            break;

        case WinchState::DIRECTION_DELAY:
            setOutputs(false, false);

            if (requested_ == WinchDirection::STOP) {
                setState(WinchState::IDLE);
            } else if ((now - stateTimer_) >= WINCH_DIRECTION_DELAY_MS) {
                setState(WinchState::IDLE);
            }

            break;

        case WinchState::FAULT:
            setOutputs(false, false);

            if (requested_ == WinchDirection::STOP && !coolingDown_) {
                setState(WinchState::IDLE);
            }

            break;
    }
}

Timer::Duration WinchController::cooldownRemainingMs() const {
    if (!coolingDown_) {
        return 0;
    }

    const auto elapsed = Timer() - cooldownStartTime_;

    if (elapsed >= requiredCooldownMs_) {
        return 0;
    }

    return requiredCooldownMs_ - elapsed;
}

void WinchController::onButtonContinuousPress(const uint8_t pin) {
    // Winch is hold-to-run. Both buttons cannot be pressed because it uses a
    // momentary rocker.
    switch (pin) {
        case WINCH_UP_SW:
            commandUp();
            break;
        case WINCH_DN_SW:
            commandDown();
            break;
    }
}

void WinchController::onButtonReleased([[maybe_unused]] const uint8_t pin) {
    // Winch is subscribed only to WINCH_UP_SW and WINCH_DN_SW, no other
    // button will cause this to be called.

    stop();
}

void WinchController::setState(const WinchState state) {
    if (state != state_) {
        state_ = state;
        notify();
    }
}

void WinchController::registerObserver(WinchObserver* observer) {
    if (nullptr == observer) {
        Abort(F("winch observer nullptr"));
    }

    observers_.push_back(observer);

    observer->onWinchState(state_, cooldownRemainingMs());
}

void WinchController::notify() {
    for (auto* observer : observers_) {
        observer->onWinchState(state_, cooldownRemainingMs());
    }
}

}  // namespace shstrailer
