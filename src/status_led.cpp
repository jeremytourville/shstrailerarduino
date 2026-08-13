#include "status_led.h"

#include "algorithm.h"
#include "console.h"
#include "pins.h"

namespace shstrailer {

void StatusLED::update() {
    if (!initialized_) {
        initialized_ = true;
        timer_.start();
        pinMode(STATUS_LED, OUTPUT);
        digitalWrite(STATUS_LED, LOW);
    }

    if (const auto delay = minimum(winchDelay_, batteryDelay_);
        timer_.elapsed() > delay) {
        timer_.start();

        toggle();
    }
}

void StatusLED::onHeartBeat() {
    // If there is neither an active winch/battery state then use heartbeat.
    if (ULONG_MAX == minimum(winchDelay_, batteryDelay_)) {
        toggle();
    }
}

void StatusLED::onWinchState(
    const WinchState state,
    [[maybe_unused]] const Timer::Stamp cooldownTimeRemaining) {
    // very fast blink for fault
    if (WinchState::FAULT == state) {
        winchDelay_ = 100UL;
    } else {
        winchDelay_ = ULONG_MAX;
    }
}

void StatusLED::onBatteryState(const BatteryState state) {
    // fast blink for critical battery
    if (BatteryState::CRITICAL == state) {
        batteryDelay_ = 250UL;
    } else {
        batteryDelay_ = ULONG_MAX;
    }
}

void StatusLED::toggle() {
    state_ = HIGH == state_ ? LOW : HIGH;
    digitalWrite(STATUS_LED, state_);
}

}  // namespace shstrailer
