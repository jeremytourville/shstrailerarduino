#include "battery.h"

#include "abort.h"
#include "pins.h"

namespace shstrailer {

namespace {

constexpr Timer::Duration kSampleInterval = 250;
constexpr Timer::Duration kStableStateDuration =
    1000UL * 60UL * 30UL;  // 30 minutes
constexpr float kWarningVoltage = 12.20f;
constexpr float kCriticalVoltage = 11.80f;

}  // namespace

Battery::Battery(BatteryReader& reader) : reader_(reader) {}

void Battery::update() {
    if (sampleTimer_.elapsed() < kSampleInterval) {
        return;
    }

    sampleTimer_.start();

    voltage_ = reader_.readVoltage();

    notifyVoltage();

    if (voltage_ < kCriticalVoltage) {
        if (BatteryState::CRITICAL != potentialState_) {
            stateTimer_.start();
            potentialState_ = BatteryState::CRITICAL;
        }
    } else if (voltage_ < kWarningVoltage) {
        if (BatteryState::WARNING != potentialState_) {
            stateTimer_.start();
            potentialState_ = BatteryState::WARNING;
        }
    } else if (BatteryState::OK != potentialState_) {
        stateTimer_.start();
        potentialState_ = BatteryState::OK;
    }

    // The battery has been in a new potential state long enough
    // to set it as that state.
    if (state_ != potentialState_ &&
        stateTimer_.elapsed() > kStableStateDuration) {
        state_ = potentialState_;
        notifyState();
    }
}

void Battery::registerObserver(BatteryObserver* observer) {
    if (nullptr == observer) {
        Abort(F("battery observer nullptr"));
    }

    observers_.push_back(observer);

    // provide initial state
    observer->onBatteryState(state_);
}

void Battery::notifyVoltage() {
    for (auto* observer : observers_) {
        observer->onBatteryVoltage(voltage_);
    }
}

void Battery::notifyState() {
    for (auto* observer : observers_) {
        observer->onBatteryState(state_);
    }
}

}  // namespace shstrailer
